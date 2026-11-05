#include <Wire.h>
#include "medicao.h"
#include "display_utils.h"
#include "notification_utils.h"


// constantes
#define TRIGGER 25
#define ECHO 26 
#define SDA_PIN 21
#define SCL_PIN 22

#define PIN_IR 27 //SAIDA OUT DO SENSOR INFRAVERMELHO

#define NIVEL_ALERTA_LIXEIRA 80 // porcentagem a partir da qual a lixeira emite a notificacao
#define TEMPO_DISPLAY_ACESO 5000  //Tempo que display fica aceso apos a ultima deteccao
#define INTERVALO_MEDICAO 1000    //Tempo entre medicoes

//TROCAR PELAS MEDICOES REAIS
#define ALTURA_LIXEIRA_VAZIA 45
#define ALTURA_LIXEIRA_CHEIA 5


// objetos
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// variaveis globais
bool ja_ocorreu_aviso = false; // indica se a notificacao ja foi enviada
float pct = 0; // armazena a porcentagem de lotacao da lixeira
bool display_ligado = false;
unsigned long ultima_deteccao = 0;
unsigned long ultima_medicao = 0;



void liga_display() {
  lcd.display();
  lcd.backlight();
  display_ligado = true;
  imprime_porcentagem(lcd, pct);  // mostra o valor atual na hora, sem esperar a proxima medicao
}

void desliga_display() {
  lcd.clear();
  lcd.noDisplay();
  lcd.noBacklight();
  display_ligado = false;
}

void mede_e_atualiza() {
  float dist = get_dist(TRIGGER, ECHO);
  Serial.println(dist);

  if (dist < 0) {
    if (display_ligado) imprime_mensagem_erro(lcd);
    return;
  }

  float pct_atual = CalculaPorcentagem(dist, ALTURA_LIXEIRA_VAZIA, ALTURA_LIXEIRA_CHEIA);

  if (!(pct_atual > pct - 5 && pct_atual < pct + 2)) {
    pct = pct_atual;
  }

  if (display_ligado) imprime_porcentagem(lcd, pct);

  if (pct > NIVEL_ALERTA_LIXEIRA && !ja_ocorreu_aviso) {
    envia_mensagem_telegram("Já está na hora tirar o lixo fora!");
    ja_ocorreu_aviso = true;
  }
  else if (pct <= NIVEL_ALERTA_LIXEIRA) {
    ja_ocorreu_aviso = false;
  }
}




void setup(){
  Serial.begin(9600);

  Wire.begin(SDA_PIN, SCL_PIN);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(PIN_IR, INPUT);

  lcd.init();
  imprime_inicializacao(lcd);
  desliga_display();
}

void loop(){

  if (digitalRead(PIN_IR) == LOW) {   // LOW = obstaculo detectado
    ultima_deteccao = millis();
    if (!display_ligado) liga_display();
  }

  if (display_ligado && millis() - ultima_deteccao > TEMPO_DISPLAY_ACESO) {
    desliga_display();
  }

  
  if (millis() - ultima_medicao >= INTERVALO_MEDICAO) {
    ultima_medicao = millis();
    mede_e_atualiza();
  }

}   
