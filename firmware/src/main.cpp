
// constantes
#define TRIGGER 25
#define ECHO 26 

#define NUM_MEDICOES 10

#define CTE_VEL_SOM 0.0344

// variaveis globais
float duracao;
float dist;

void setup(){

  Serial.begin(9600);

  // pinmodes
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
}

void loop(){
  Serial.println(get_distancia);
}   

float get_distancia()
{
  // variaveis
  float distancias[NUM_MEDICOES];

  // realiza as medicoes
  for (int i = 0; i < NUM_MEDICOES; i ++)
  {
    // limpa o trigger
    digitalWrite(TRIGGER,LOW);
    delayMicroseconds(5);  
  
    // gera um pulso do trigger 
    digitalWrite(TRIGGER,HIGH);  
    delayMicroseconds(10);      
    digitalWrite(TRIGGER,LOW); 

    duracao=pulseIn(ECHO,HIGH);      
  
    distancias[i] = duracao * CTE_VEL_SOM / 2;
    Serial.print ("Distancia = ");
    Serial.print (distancias[i]);         
    Serial.print (" cm\n");         
          
    delay (200);     
  }
}