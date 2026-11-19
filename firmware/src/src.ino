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

#define NIVEL_ALERTA_LIXEIRA 80 // porcentagem a partir da qual a lixeira emite a notificacao

//TROCAR PELAS MEDICOES REAIS
float ALTURA_LIXEIRA_VAZIA;
#define ALTURA_LIXEIRA_CHEIA 5


// objetos
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// variaveis globais
bool ja_ocorreu_aviso; // indica se a notificacao ja foi enviada
float pct; // armazena a porcentagem de lotacao da lixeira


void setup(){
  Serial.begin(9600);

  Wire.begin(SDA_PIN, SCL_PIN);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);

  lcd.init();
  imprime_inicializacao(lcd);

  ALTURA_LIXEIRA_VAZIA = get_dist(TRIGGER, ECHO);
  Serial.printf("Altura vazia: %.2f\n", ALTURA_LIXEIRA_VAZIA);

  // setup inicial se nao foi feito ainda
  bool calibrado = cfg_carregar();
  float d = get_dist(TRIGGER, ECHO);
  if (!calibrado || (d > 0 && d < 3))
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

    if (pct > NIVEL_ALERTA_LIXEIRA && !ja_ocorreu_aviso)
    {
      if (notificacoes_ativas() && envia_mensagem_telegram("Já está na hora tirar o lixo fora!"))
        ja_ocorreu_aviso = true;
    }
    else if (pct < NIVEL_ALERTA_LIXEIRA - 10)
    {
      ja_ocorreu_aviso = false;   // lixeira esvaziada: libera um novo aviso
    }
  }

  delay(1000);
}