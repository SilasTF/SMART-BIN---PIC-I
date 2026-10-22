
// constantes
#define TRIGGER 25
#define ECHO 26 
float duracao;
float dist;

void setup(){
  Serial.begin(9600);
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
}

void loop(){

  // Limpa o trigPin configurando-o como LOW: 
  digitalWrite(TRIGGER,LOW);
  delayMicroseconds(5);  
  
  // Aciona o sensor definindo o trigPin alto por 10 microssegundos
  digitalWrite(TRIGGER,HIGH);  
  delayMicroseconds(10);      
  digitalWrite(TRIGGER,LOW); 
  // Leia o echoPin, pulseIn() 
  //retorna a duração (comprimento do pulso) em microssegundos:
  duracao=pulseIn(ECHO,HIGH);      
  
  dist = duracao *0.0172; //ou colocar direto 0.017 ou duracao/58; 
  Serial.print ("Distancia = ");
  Serial.print (dist);         
  Serial.print (" cm\n");         
  //Serial.write (10);            
  delay (2000);                
}                             