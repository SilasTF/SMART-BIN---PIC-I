#include "medicao.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// constantes
#define TRIGGER 25
#define ECHO 26 
#define SDA_PIN 21
#define SCL_PIN 22

//TROCAR PELAS MEDICOES REAIS
#define ALTURA_LIXEIRA_VAZIA 45
#define ALTURA_LIXEIRA_CHEIA 5


LiquidCrystal_I2C lcd(0x27, 16, 2);  


//Calcula a porcentgem considerando as medicoes pre definidas
float CalculaPorcentagem(float dist){
  float p = (ALTURA_LIXEIRA_VAZIA - dist) * 100.0 / (ALTURA_LIXEIRA_VAZIA - ALTURA_LIXEIRA_CHEIA);
  if (p > 100) p = 100;
  else if (p < 0) p = 0;

  return p;
}

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
    lcd.setCursor(0, 0);
    lcd.print("Erro na leitura ");
    lcd.setCursor(0, 1);
    lcd.print("Vericar sensor");
  }
  
  else{

    float pct = CalculaPorcentagem(dist);

    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(dist, 1);
    lcd.print(" cm   ");

    lcd.setCursor(0, 1);
    lcd.print("Nivel: ");
    lcd.print(pct, 0);
    lcd.print("%       ");

  }
  
  delay(1000);

}   
