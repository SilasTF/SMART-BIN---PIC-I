#include <Wire.h>
#include "medicao.h"
#include "display_utils.h"
#include "notification_utils.h"


// constantes
#define TRIGGER 25
#define ECHO 26 
#define SDA_PIN 21
#define SCL_PIN 22

#define NIVEL_ALERTA_LIXEIRA 80 // porcentagem a partir da qual a lixeira emite a notificacao

//TROCAR PELAS MEDICOES REAIS
#define ALTURA_LIXEIRA_VAZIA 41
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
}

void loop(){

  Serial.println(get_dist(TRIGGER, ECHO));

  float dist = get_dist(TRIGGER, ECHO);
  

  if (dist < 0){
    imprime_mensagem_erro(lcd);
  }
  else{
    float pct_atual = CalculaPorcentagem(dist, ALTURA_LIXEIRA_VAZIA, ALTURA_LIXEIRA_CHEIA);

    if (!(pct_atual > pct - 5 && pct_atual < pct + 2))
    {
      pct = pct_atual;
    }

    imprime_porcentagem(lcd, pct);

    if (pct > NIVEL_ALERTA_LIXEIRA && !ja_ocorreu_aviso)
    {
      envia_mensagem_telegram("Já está na hora tirar o lixo fora!");
      ja_ocorreu_aviso = 1;
    }
    else
    {
      ja_ocorreu_aviso = 0;
    }
  }
  
  delay(1000);

}   
