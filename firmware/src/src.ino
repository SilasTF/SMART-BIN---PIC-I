#include <Wire.h>
#include "medicao.h"
#include "display_utils.h"
#include "notification_utils.h"
#include "setup_portal.h"

// constantes
#define TRIGGER 25
#define ECHO 26
#define SDA_PIN 21
#define SCL_PIN 22

#define NIVEL_ALERTA_LIXEIRA 80 // usado so no Telegram direto (sem painel)

// objetos
LiquidCrystal_I2C lcd(0x27, 16, 2);

// variaveis globais
bool ja_ocorreu_aviso = false;     // indica se o aviso (Telegram direto) ja foi enviado
float pct = 0;                     // porcentagem de lotacao da lixeira
unsigned long proxima_tentativa = 0; // millis() a partir do qual pode enviar de novo

void setup(){
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  lcd.init();
  imprime_inicializacao(lcd);

  // primeira vez (sem calibracao salva) ou mao a menos de 3 cm do sensor ao ligar: abre o portal
  bool calibrado = cfg_carregar();
  float d = get_dist(TRIGGER, ECHO);
  if (!calibrado || (d > 0 && d < 7))
  {
    portal_executa(lcd, TRIGGER, ECHO);   // nao retorna: salva e reinicia
  }
}

void loop(){
  float dist = get_dist(TRIGGER, ECHO);

  if (dist < 0){
    imprime_mensagem_erro(lcd);
  }
  else{
    float pct_atual = CalculaPorcentagem(dist, cfg.altura_vazia, cfg.altura_cheia);

    if (!(pct_atual > pct - 5 && pct_atual < pct + 2))
    {
      pct = pct_atual;
    }

    imprime_porcentagem(lcd, pct);

    bool pode_enviar = (long)(millis() - proxima_tentativa) >= 0;

    if (servidor_ativo())
    {
      // o painel recebe as leituras, decide os avisos e devolve a configuracao
      if (pode_enviar)
      {
        bool ok = envia_leitura(dist);
        proxima_tentativa = millis() + (ok ? cfg.sync_min * 60000UL : 60000UL);  // falhou: tenta em 1 min
      }
    }
    else
    {
      // sem painel: aviso simples pelo Telegram direto
      if (pct > NIVEL_ALERTA_LIXEIRA && !ja_ocorreu_aviso && pode_enviar)
      {
        if (notificacoes_ativas() && envia_mensagem_telegram("Já está na hora tirar o lixo fora!"))
          ja_ocorreu_aviso = true;
        else
          proxima_tentativa = millis() + 60000;
      }
      else if (pct < NIVEL_ALERTA_LIXEIRA - 10)
      {
        ja_ocorreu_aviso = false;
      }
    }
  }

  delay(1000);
}
