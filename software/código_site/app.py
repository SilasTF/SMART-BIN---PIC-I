import os, json, sqlite3, time, hmac, threading, urllib.request, urllib.parse
from datetime import datetime
from flask import Flask, g, request, Response, send_from_directory

DB = os.environ.get("DB_PATH", "lixeira.db")
API_KEY = os.environ.get("API_KEY", "troque-esta-chave")          # usada pelo ESP32
PASSWORD = os.environ.get("DASHBOARD_PASSWORD", "")               # senha do site (vazia = sem senha)
TG_TOKEN = os.environ.get("TELEGRAM_TOKEN", "")                   # opcional: alertas pelo servidor
TG_CHAT = os.environ.get("TELEGRAM_CHAT_ID", "")

# Regras de notificação. Tipos: "capacity" (nível %), "elapsed" (horas desde o último esvaziamento),
# "clock" (horário do dia, com mínimo opcional de ocupação).
DEFAULT_RULES = [
    {"id": "r1", "type": "capacity", "enabled": True, "label": "Primeiro aviso", "level": 70},
    {"id": "r2", "type": "capacity", "enabled": True, "label": "Quase cheia", "level": 90},
    {"id": "r3", "type": "capacity", "enabled": True, "label": "Cheia", "level": 100},
]

DEFAULTS = {
    "name": "Lixeira", "wifi_ssid": "", "wifi_pass": "",
    "empty_cm": 50.0, "full_cm": 8.0,            # calibração: distância com a lixeira vazia / cheia
    "rules": DEFAULT_RULES,                      # regras de notificação (editáveis no site)
    "quiet_start": "22:00", "quiet_end": "07:00", # horário sem notificações
    "sync_minutes": 15,                          # de quanto em quanto tempo o ESP envia dados
}

app = Flask(__name__, static_folder="static")


def db():
    if "db" not in g:
        g.db = sqlite3.connect(DB)
        g.db.row_factory = sqlite3.Row
    return g.db


@app.teardown_appcontext
def close_db(_):
    if "db" in g:
        g.db.close()


def init_db():
    c = sqlite3.connect(DB)
    c.executescript("""
    CREATE TABLE IF NOT EXISTS bins(id TEXT PRIMARY KEY, config TEXT NOT NULL DEFAULT '{}', alert_level INTEGER NOT NULL DEFAULT 0);
    CREATE TABLE IF NOT EXISTS readings(id INTEGER PRIMARY KEY, bin_id TEXT, ts INTEGER, distance REAL, battery REAL);
    CREATE TABLE IF NOT EXISTS rule_state(bin_id TEXT, rule_id TEXT, state TEXT, PRIMARY KEY(bin_id, rule_id));
    CREATE TABLE IF NOT EXISTS events(id INTEGER PRIMARY KEY, bin_id TEXT, ts INTEGER, kind TEXT);
    CREATE INDEX IF NOT EXISTS idx_r ON readings(bin_id, ts);
    """)
    c.commit(); c.close()


@app.before_request
def auth():
    if request.path == "/api/ingest" or not PASSWORD:
        return
    a = request.authorization
    if not a or not hmac.compare_digest(a.password or "", PASSWORD):
        return Response("Acesso restrito", 401, {"WWW-Authenticate": 'Basic realm="Lixeira"'})


def get_cfg(bid):
    row = db().execute("SELECT config FROM bins WHERE id=?", (bid,)).fetchone()
    return {**DEFAULTS, **json.loads(row["config"])} if row else None


def percent(cfg, dist):
    span = cfg["empty_cm"] - cfg["full_cm"]
    if span <= 0:
        return 0.0
    return max(0.0, min(100.0, (cfg["empty_cm"] - dist) / span * 100))


def in_quiet(cfg):
    now = datetime.now().strftime("%H:%M")
    a, b = cfg["quiet_start"], cfg["quiet_end"]
    if a == b:
        return False
    return (a <= now < b) if a < b else (now >= a or now < b)


def notify(text):
    if not (TG_TOKEN and TG_CHAT):
        return
    try:
        data = urllib.parse.urlencode({"chat_id": TG_CHAT, "text": text}).encode()
        urllib.request.urlopen(f"https://api.telegram.org/bot{TG_TOKEN}/sendMessage", data, timeout=10)
    except Exception as e:
        print("Falha no Telegram:", e)


def eta_hours(bid, pct_now):
    """Previsão linear: usa as leituras das últimas 24 h depois do último esvaziamento."""
    d = db()
    ev = d.execute("SELECT MAX(ts) t FROM events WHERE bin_id=? AND kind='emptied'", (bid,)).fetchone()["t"] or 0
    since = max(ev, int(time.time()) - 86400)
    cfg = get_cfg(bid)
    rows = d.execute("SELECT ts, distance FROM readings WHERE bin_id=? AND ts>=? ORDER BY ts", (bid, since)).fetchall()
    if len(rows) < 4:
        return None
    xs = [(r["ts"] - rows[0]["ts"]) / 3600 for r in rows]
    ys = [percent(cfg, r["distance"]) for r in rows]
    n = len(xs); mx = sum(xs) / n; my = sum(ys) / n
    var = sum((x - mx) ** 2 for x in xs)
    if var == 0:
        return None
    slope = sum((x - mx) * (y - my) for x, y in zip(xs, ys)) / var   # % por hora
    return round((100 - pct_now) / slope, 1) if slope > 0.05 and pct_now < 100 else None


def levels_of(cfg):
    """Dois limites para colorir a lixeira no site, derivados das regras de capacidade ativas."""
    lv = sorted({int(r["level"]) for r in cfg["rules"] if r["type"] == "capacity" and r.get("enabled") and r["level"] < 100})[:2]
    while len(lv) < 2:
        lv.append(lv[-1] + 20 if lv else 70)
    return lv


def evaluate_rules(bid):
    """Avalia as regras ativas da lixeira e envia as notificações devidas (chamada a cada leitura e a cada minuto)."""
    con, cfg, now = db(), get_cfg(bid), time.time()
    r = con.execute("SELECT ts, distance FROM readings WHERE bin_id=? ORDER BY ts DESC LIMIT 1", (bid,)).fetchone()
    if not r:
        return
    pct, name = percent(cfg, r["distance"]), cfg["name"]
    last_empty = con.execute("SELECT MAX(ts) t FROM events WHERE bin_id=? AND kind='emptied'", (bid,)).fetchone()["t"]
    since = last_empty or con.execute("SELECT MIN(ts) t FROM readings WHERE bin_id=?", (bid,)).fetchone()["t"]
    hours = (now - since) / 3600
    cycle = str(last_empty or 0)                      # muda a cada esvaziamento: rearma as regras
    age_min = (now - r["ts"]) / 60
    stale = f" (última leitura há {age_min:.0f} min)" if age_min > cfg["sync_minutes"] * 2.5 else ""

    for rule in cfg["rules"]:
        if not rule.get("enabled"):
            continue
        row = con.execute("SELECT state FROM rule_state WHERE bin_id=? AND rule_id=?", (bid, rule["id"])).fetchone()
        st, new, msg, t = (row["state"] if row else ""), None, None, rule["type"]
        title = f"{name} — {rule['label']}"
        if t == "capacity":
            if pct >= rule["level"] and st != cycle:
                new, msg = cycle, f"🗑️ {title}: {pct:.0f}% de ocupação"
            elif st == cycle and pct < rule["level"] - 10:
                new = ""                              # nível caiu: rearma
        elif t == "elapsed":
            if hours >= rule["hours"] and st != cycle:
                new, msg = cycle, f"⏱️ {title}: {hours:.0f} h sem esvaziamento"
        elif t == "clock":
            n = datetime.now()
            hh, mm = map(int, rule["time"].split(":"))
            delta = (n - n.replace(hour=hh, minute=mm, second=0, microsecond=0)).total_seconds()
            today = n.strftime("%Y-%m-%d")
            if 0 <= delta < 3600 and st != today and pct >= rule.get("min_pct", 0):
                new, msg = today, f"🕗 {title}: {pct:.0f}% de ocupação{stale}"
        if msg and in_quiet(cfg):
            continue                                  # horário silencioso: tenta de novo depois
        if new is not None:
            if msg:
                notify(msg)
            con.execute("INSERT OR REPLACE INTO rule_state VALUES(?,?,?)", (bid, rule["id"], new))
    con.commit()


def ticker():
    while True:
        time.sleep(60)
        try:
            with app.app_context():
                for b in db().execute("SELECT id FROM bins").fetchall():
                    evaluate_rules(b["id"])
        except Exception as e:
            print("Erro nas regras:", e)


@app.post("/api/ingest")
def ingest():
    if not hmac.compare_digest(request.headers.get("X-API-Key", ""), API_KEY):
        return {"erro": "chave inválida"}, 401
    d = request.get_json(force=True)
    bid, dist, now = str(d.get("bin_id", "lixeira-1")), float(d["distance_cm"]), int(time.time())
    con = db()
    created = con.execute("INSERT OR IGNORE INTO bins(id) VALUES(?)", (bid,)).rowcount == 1
    cfg = get_cfg(bid)
    if created and "empty_cm" in d and "full_cm" in d:       # primeira vez: adota a calibração feita no ESP32
        cfg.update(empty_cm=float(d["empty_cm"]), full_cm=float(d["full_cm"]))
        con.execute("UPDATE bins SET config=? WHERE id=?", (json.dumps(cfg), bid))
    prev = con.execute("SELECT distance FROM readings WHERE bin_id=? ORDER BY ts DESC LIMIT 1", (bid,)).fetchone()
    con.execute("INSERT INTO readings(bin_id,ts,distance,battery) VALUES(?,?,?,?)", (bid, now, dist, d.get("battery_v")))
    pct = percent(cfg, dist)
    if prev and percent(cfg, prev["distance"]) - pct >= 30:       # queda brusca = esvaziada
        con.execute("INSERT INTO events(bin_id,ts,kind) VALUES(?,?,'emptied')", (bid, now))
    con.commit()
    evaluate_rules(bid)
    # o ESP32 aplica estes campos a cada envio (o resto da configuração só interessa ao servidor)
    return {"config": {k: cfg[k] for k in ("empty_cm", "full_cm", "sync_minutes", "wifi_ssid", "wifi_pass")}}


def summary(bid):
    cfg = get_cfg(bid)
    r = db().execute("SELECT ts, distance, battery FROM readings WHERE bin_id=? ORDER BY ts DESC LIMIT 1", (bid,)).fetchone()
    out = {"id": bid, "name": cfg["name"], "levels": levels_of(cfg), "ts": None, "pct": 0, "distance": None, "battery": None, "eta_h": None, "offline": True}
    if r:
        pct = percent(cfg, r["distance"])
        out.update(ts=r["ts"], pct=round(pct, 1), distance=r["distance"], battery=r["battery"], eta_h=eta_hours(bid, pct),
                   offline=time.time() - r["ts"] > cfg["sync_minutes"] * 150)
    return out


@app.get("/api/bins")
def bins():
    return {"bins": [summary(r["id"]) for r in db().execute("SELECT id FROM bins ORDER BY id")]}


@app.get("/api/bins/<bid>")
def bin_detail(bid):
    cfg = get_cfg(bid)
    if cfg is None:
        return {"erro": "lixeira não encontrada"}, 404
    hours = min(int(request.args.get("hours", 24)), 24 * 30)
    rows = db().execute("SELECT ts, distance FROM readings WHERE bin_id=? AND ts>=? ORDER BY ts", (bid, int(time.time()) - hours * 3600)).fetchall()
    step = max(1, len(rows) // 300)
    ev = db().execute("SELECT ts FROM events WHERE bin_id=? AND kind='emptied' ORDER BY ts DESC LIMIT 8", (bid,)).fetchall()
    return {**summary(bid), "config": cfg, "emptied": [e["ts"] for e in ev],
            "history": [{"ts": r["ts"], "pct": round(percent(cfg, r["distance"]), 1)} for r in rows[::step]]}


def clean_rules(raw):
    out = []
    for i, r in enumerate(raw[:30]):
        t = r.get("type")
        if t not in ("capacity", "elapsed", "clock"):
            continue
        c = {"id": str(r.get("id") or f"r{int(time.time() * 1000)}{i}")[:24], "type": t,
             "enabled": bool(r.get("enabled", True)), "label": str(r.get("label") or "Aviso")[:40]}
        if t == "capacity":
            c["level"] = max(1, min(100, int(r["level"])))
        elif t == "elapsed":
            c["hours"] = max(1, min(1440, int(r["hours"])))
        else:
            hh, mm = str(r["time"]).split(":")
            c["time"] = f"{int(hh) % 24:02d}:{int(mm) % 60:02d}"
            c["min_pct"] = max(0, min(100, int(r.get("min_pct", 0))))
        out.append(c)
    return out


@app.put("/api/bins/<bid>/config")
def put_config(bid):
    cfg = get_cfg(bid)
    if cfg is None:
        return {"erro": "lixeira não encontrada"}, 404
    body = request.get_json(force=True)
    old = {r["id"]: r for r in cfg["rules"]}
    try:
        if "rules" in body:
            body["rules"] = clean_rules(body["rules"])
        for k, v in body.items():
            if k in DEFAULTS:
                cfg[k] = v if k == "rules" else type(DEFAULTS[k])(v)
    except (KeyError, ValueError, TypeError, AttributeError):
        return {"erro": "dados inválidos"}, 400
    for r in cfg["rules"]:                            # regra nova ou alterada volta a valer do zero
        if old.get(r["id"]) != r:
            db().execute("DELETE FROM rule_state WHERE bin_id=? AND rule_id=?", (bid, r["id"]))
    db().execute("UPDATE bins SET config=? WHERE id=?", (json.dumps(cfg), bid))
    db().commit()
    return {"config": cfg}


@app.post("/api/bins/<bid>/emptied")
def mark_emptied(bid):
    if get_cfg(bid) is None:
        return {"erro": "lixeira não encontrada"}, 404
    db().execute("INSERT INTO events(bin_id,ts,kind) VALUES(?,?,'emptied')", (bid, int(time.time())))
    db().commit()
    return {"ok": True}


@app.get("/")
def index():
    return send_from_directory("static", "index.html")


if __name__ == "__main__":
    init_db()
    threading.Thread(target=ticker, daemon=True).start()
    app.run(host="0.0.0.0", port=int(os.environ.get("PORT", 8000)))
