#include "medicao.h"

// constantes
#define TRIGGER 25
#define ECHO 26 

void setup(){

  Serial.begin(9600);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
}

void loop(){
  Serial.println(get_dist(TRIGGER, ECHO));
}   