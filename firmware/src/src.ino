#include "medicao.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



// constantes
#define TRIGGER 25
#define ECHO 26 
#define SDA_PIN 17
#define SCL_PIN 16

LiquidCrystal_I2C lcd(0x27, 16, 2);  // endereço, colunas, linhas

void setup(){
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
}

void loop(){
  float ocupacao=1;
  if(get_dist(TRIGGER, ECHO)>45){
    float dist=0;
  }else{
    ocupacao = get_dist(TRIGGER, ECHO)/45;
  }
  

  lcd.setCursor(0, 0);
  lcd.print("Smart Bin");
  lcd.setCursor(0, 1);
  lcd.print("Ocupacao: ");
  lcd.print(100-ocupacao*100);
  lcd.print("%  ");

  delay(1000);

}   