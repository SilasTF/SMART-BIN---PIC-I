#include <Arduino.h>
#include "medicao.h"

float get_dist(byte trig_pin, byte echo_pin)
{
  // variaveis
  float duracao;
  float distancias[NUM_MEDICOES];

  // realiza as medicoes
  for (int i = 0; i < NUM_MEDICOES; i ++)
  {
    // limpa o trigger
    digitalWrite(trig_pin,LOW);
    delayMicroseconds(5);  
  
    // gera um pulso do trigger 
    digitalWrite(trig_pin,HIGH);  
    delayMicroseconds(10);      
    digitalWrite(trig_pin,LOW); 

    duracao=pulseIn(echo_pin, HIGH, 1000000);      
  
    distancias[i] = duracao * CTE_VEL_SOM / 2;
    Serial.print ("Distancia = ");
    Serial.print (distancias[i]);         
    Serial.print (" cm\n");         
          
    delay (200);     
  }

  return 0;
}