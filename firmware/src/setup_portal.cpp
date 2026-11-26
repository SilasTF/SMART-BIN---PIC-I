#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "setup_portal.h"
#include "setup_page.h"
#include "medicao.h"

Config cfg;
static Preferences prefs;

bool cfg_carregar()
{
  prefs.begin("smartbin", true);  // true = somente leitura
  bool calibrado = prefs.getBool("ok", false);
  prefs.getString("ssid", "").toCharArray(cfg.ssid, sizeof(cfg.ssid));
  prefs.getString("pass", "").toCharArray(cfg.pass, sizeof(cfg.pass));
  prefs.getString("token", "").toCharArray(cfg.token, sizeof(cfg.token));
  prefs.getString("chat", "").toCharArray(cfg.chat, sizeof(cfg.chat));
  cfg.altura_vazia = prefs.getFloat("vazia", 0);
  cfg.altura_cheia = prefs.getFloat("cheia", 5);
  prefs.getString("srv", "").toCharArray(cfg.servidor, sizeof(cfg.servidor));
  prefs.getString("key", "").toCharArray(cfg.chave, sizeof(cfg.chave));
  cfg.sync_min = prefs.getUInt("sync", 15);
  prefs.end();
  return calibrado;
}

void cfg_salvar()
{
  prefs.begin("smartbin", false);
  prefs.putString("ssid", cfg.ssid);
  prefs.putString("pass", cfg.pass);
  prefs.putString("token", cfg.token);
  prefs.putString("chat", cfg.chat);
  prefs.putFloat("vazia", cfg.altura_vazia);
  prefs.putFloat("cheia", cfg.altura_cheia);
  prefs.putString("srv", cfg.servidor);
  prefs.putString("key", cfg.chave);
  prefs.putUInt("sync", cfg.sync_min);
  prefs.putBool("ok", true);
  prefs.end();
}

bool servidor_ativo()
{
  return cfg.servidor[0] && cfg.chave[0] && cfg.ssid[0];
}

bool notificacoes_ativas()
{
  return cfg.ssid[0] && cfg.token[0] && cfg.chat[0];
}

void portal_executa(LiquidCrystal_I2C lcd, byte trig_pin, byte echo_pin)
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(PORTAL_SSID, PORTAL_SENHA);   // o IP do ESP32 nessa rede e 192.168.4.1
  imprime_portal(lcd);

  WebServer server(80);

  server.on("/", HTTP_GET, [&]() {
    server.send_P(200, "text/html; charset=utf-8", PAGINA);
  });

  // botao "Medir lixeira vazia": devolve a distancia medida (ou -1 se falhou)
  server.on("/dist", HTTP_GET, [&]() {
    server.send(200, "text/plain", String(get_dist(trig_pin, echo_pin), 1));
  });

  server.on("/salvar", HTTP_POST, [&]() {
    cfg.altura_vazia = server.arg("vazia").toFloat();
    cfg.altura_cheia = server.arg("cheia").toFloat();

    if (cfg.altura_vazia <= cfg.altura_cheia)
    {
      server.send(400, "text/plain", "valores invalidos");
      return;
    }

    server.arg("ssid").toCharArray(cfg.ssid, sizeof(cfg.ssid));
    server.arg("pass").toCharArray(cfg.pass, sizeof(cfg.pass));
    server.arg("token").toCharArray(cfg.token, sizeof(cfg.token));
    server.arg("chat").toCharArray(cfg.chat, sizeof(cfg.chat));
    server.arg("servidor").toCharArray(cfg.servidor, sizeof(cfg.servidor));
    server.arg("chave").toCharArray(cfg.chave, sizeof(cfg.chave));

    size_t n = strlen(cfg.servidor);   // tira a barra do fim do endereco, se houver
    while (n > 0 && cfg.servidor[n - 1] == '/') cfg.servidor[--n] = 0;

    cfg_salvar();
    server.send(200, "text/plain", "ok");
    delay(1500);      // da tempo da resposta chegar ao celular
    ESP.restart();
  });

  server.begin();
  while (true)
  {
    server.handleClient();
    delay(2);
  }
}
