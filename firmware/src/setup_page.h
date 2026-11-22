#ifndef SETUP_PAGE_H
#define SETUP_PAGE_H

// Página única, sem nenhum recurso externo (o celular não tem internet ao conectar no ESP32).
// A fonte "Bricolage Grotesque" só aparece se estiver instalada; senão usa a fonte do sistema.
const char PAGINA[] PROGMEM = R"HTML(<!doctype html>
<html lang="pt-BR"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Smart Bin</title>
<style>
:root{--bg:#F2F8F6;--surf:#fff;--ink:#0E2A29;--mute:#5B7775;--line:#D5E5E1;--ok:#14A38F}
@media(prefers-color-scheme:dark){:root{--bg:#0B1A1B;--surf:#122728;--ink:#E4F2EF;--mute:#8AA9A6;--line:#21403F;--ok:#3FD1BD}}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--ink);font:16px/1.5 "Bricolage Grotesque",system-ui,sans-serif}
main{max-width:520px;margin:0 auto;padding:24px 20px 60px}
svg{width:64px;height:80px;color:var(--ok)}
h1{font-size:30px;line-height:1.15;margin:8px 0 4px}
.m{color:var(--mute);font-size:14px}
form{display:grid;gap:16px;margin-top:20px}
fieldset{border:1px solid var(--line);border-radius:18px;padding:14px 18px 18px;margin:0;background:var(--surf);display:grid;gap:12px}
legend{font-weight:700;padding:0 6px}
label{display:grid;gap:4px;font-size:14px;color:var(--mute)}
input{font:inherit;background:var(--bg);color:var(--ink);border:1px solid var(--line);border-radius:10px;padding:9px 12px;width:100%}
button{font:inherit;border:0;border-radius:12px;padding:11px 20px;font-weight:700;cursor:pointer;background:var(--ok);color:var(--bg)}
button.g{background:none;color:var(--ink);border:1px solid var(--line);font-weight:500}
button:disabled{opacity:.6}
.row{display:grid;grid-template-columns:1fr 1fr;gap:12px}
#t{position:fixed;left:50%;bottom:24px;transform:translate(-50%,90px);background:var(--ink);color:var(--bg);padding:10px 18px;border-radius:999px;transition:transform .3s}
#t.s{transform:translate(-50%,0)}
</style></head><body><main>
<svg viewBox="0 0 80 100" aria-hidden="true"><defs><clipPath id="c"><path d="M14 28h52l-5 66H19z"/></clipPath></defs>
<path d="M14 28h52l-5 66H19z" fill="currentColor" fill-opacity=".16"/>
<g clip-path="url(#c)"><rect x="0" y="62" width="80" height="40" fill="currentColor"/><line x1="0" x2="80" y1="62" y2="62" stroke="currentColor" stroke-width="3"/></g>
<path d="M14 28h52l-5 66H19zM8 20h64M31 20v-6h18v6" fill="none" stroke="currentColor" stroke-width="3" stroke-linejoin="round"/></svg>
<h1>Configuração inicial</h1>
<p class="m">Calibre a lixeira. As notificações no Telegram são opcionais.</p>
<form id="f">
<fieldset><legend>1. Calibração</legend>
<p class="m" style="margin:0">Com a lixeira vazia e a tampa fechada, toque em medir.</p>
<button type="button" class="g" id="b">Medir lixeira vazia</button>
<div class="row">
<label>Distância vazia (cm)<input id="v" name="vazia" type="number" step="0.1" min="1" required></label>
<label>Distância cheia (cm)<input id="c2" name="cheia" type="number" step="0.1" min="1" value="5" required></label>
</div></fieldset>
<fieldset><legend>2. Wi-Fi e Telegram (opcional)</legend>
<p class="m" style="margin:0">Deixe em branco para usar só o display. O Telegram direto só é usado se não houver painel.</p>
<label>Nome da rede Wi-Fi<input name="ssid" autocapitalize="none" autocomplete="off"></label>
<label>Senha do Wi-Fi<input name="pass" type="password" autocomplete="new-password"></label>
<label>Token do bot<input name="token" autocapitalize="none" autocomplete="off"></label>
<label>Chat ID<input name="chat" autocomplete="off"></label>
</fieldset>
<fieldset><legend>3. Painel online (opcional)</legend>
<p class="m" style="margin:0">Usa o Wi-Fi acima. Com o painel, ele envia os avisos e guarda o histórico.</p>
<label>Endereço do painel<input name="servidor" type="url" autocapitalize="none" autocomplete="off" placeholder="https://seu-pc.seu-tailnet.ts.net"></label>
<label>Chave da API<input name="chave" type="password" autocomplete="new-password"></label>
</fieldset>
<button type="submit">Salvar e iniciar</button>
</form></main><div id="t" role="status"></div>
<script>
const $=s=>document.querySelector(s);
function toast(x){const t=$('#t');t.textContent=x;t.classList.add('s');setTimeout(()=>t.classList.remove('s'),2600)}
$('#b').onclick=async e=>{
  const b=e.target;b.disabled=true;b.textContent='Medindo…';
  try{const d=parseFloat(await(await fetch('/dist')).text());
    if(d>0)$('#v').value=d.toFixed(1);else toast('Não foi possível medir. Confira o sensor.')}
  catch(_){toast('Sem resposta da lixeira')}
  b.disabled=false;b.textContent='Medir lixeira vazia'};
$('#f').onsubmit=async e=>{
  e.preventDefault();
  if(!(+$('#v').value>+$('#c2').value))return toast('A distância vazia precisa ser maior que a cheia');
  const r=await fetch('/salvar',{method:'POST',body:new URLSearchParams(new FormData(e.target))}).catch(()=>null);
  if(r&&r.ok)document.querySelector('main').innerHTML='<h1>Tudo certo</h1><p class="m">A lixeira está reiniciando. Você já pode fechar esta página.</p>';
  else toast('Não foi possível salvar. Tente de novo.')};
</script></body></html>)HTML";

#endif
