#include "medicao.h"
#include "display_utils.h"
#include <Wire.h>


// constantes
#define TRIGGER 25
#define ECHO 26 
#define SDA_PIN 21
#define SCL_PIN 22

//TROCAR PELAS MEDICOES REAIS
#define ALTURA_LIXEIRA_VAZIA 45
#define ALTURA_LIXEIRA_CHEIA 5


LiquidCrystal_I2C lcd(0x27, 16, 2);  


void setup(){
  Serial.begin(9600);

  Wire.begin(SDA_PIN, SCL_PIN);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("SmartBin");
  delay(1000);
  lcd.clear();
}

void loop(){

  Serial.println(get_dist(TRIGGER, ECHO));

  float dist = get_dist(TRIGGER, ECHO);
  

  if (dist < 0){
    imprime_mensagem_erro(lcd);
  }
  
  else{

    float pct = CalculaPorcentagem(dist, ALTURA_LIXEIRA_VAZIA, ALTURA_LIXEIRA_CHEIA);

    imprime_distancia_porcentagem(lcd, dist, pct);
  }
  
  delay(1000);

}   
