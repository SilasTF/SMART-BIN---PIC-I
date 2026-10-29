#include <Arduino.h>
#include "medicao.h"
#include <math.h>


//FUNCAO PRINCIPAL
float get_dist(byte trig_pin, byte echo_pin)
{
  // Variaveis
  float duracao;
  float distancias[NUM_MEDICOES];
  int n = 0;

  // Coleta dos dados
  for (int i = 0; i < NUM_MEDICOES; i ++)
  {
    float d = medir_dist_uma_vez(trig_pin, echo_pin);
    if (d > 0){
      distancias[n] = d;
      n++;
    }
    
    delay(60);
  }

  if (n < 3){
    Serial.print("Num de leituras insuficientes");
    return -1;
  }

  //Mediana
  ordena_vetor_dist(distancias, n);
  float mediana = mediana_do_vetor_ordenado(distancias, n);

  //Mediana dos desvios absolutos em relacao a mediana (variacao em relacao a mediana)
  float desv[NUM_MEDICOES];
  for (int i = 0; i < n; i++){
    desv[i] = fabs(distancias[i] - mediana);
  }
  ordena_vetor_dist(desv, n);
  float mad = mediana_do_vetor_ordenado(desv, n);
    //Limite da discrepancia entre valores (Ajustar o 3 caso necessario)
  float limite = 3 * 1.4826 * mad;

  //Mantem apenas os valores aceitaveis
  float normais[NUM_MEDICOES];
  int nNorm = 0;
  for (int i = 0; i < n; i++){
    if (mad == 0 || fabs(distancias[i] - mediana) <= limite){
      normais[nNorm] = distancias[i];
      nNorm++;
    }
  }

  //Media dos valores normais
  float result = (nNorm > 0) ? media(normais, nNorm) : mediana;

  //Desvio
  float dp = desvio_padrao(normais, nNorm, result);

  Serial.print("Validas: ");  
  Serial.println(n);
  Serial.print("Usadas: "); 
  Serial.println(nNorm);
  Serial.print("Resultado: "); 
  Serial.println(result);
  Serial.print("DP: "); 
  Serial.println(dp);

  return result;
}



float medir_dist_uma_vez(byte trig_pin, byte echo_pin){
  // limpa o trigger
    digitalWrite(trig_pin,LOW);
    delayMicroseconds(5);  
  
    // gera um pulso do trigger 
    digitalWrite(trig_pin,HIGH);  
    delayMicroseconds(10);      
    digitalWrite(trig_pin,LOW); 
    
    float duracao = pulseIn(echo_pin, HIGH, 30000);
    if (duracao == 0) return -1;

    return duracao * CTE_VEL_SOM / 2;
}


void ordena_vetor_dist(float vetor[], int tamanho){
  int i, j, trocou;
  float aux;

    for (i = 0; i < tamanho - 1; i++) {

        trocou = 0;

        for (j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                aux = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
                trocou = 1;
            }
        }

        if (trocou == 0) break;
    }
}


float mediana_do_vetor_ordenado(float vetor[], int tamanho){
  if (tamanho % 2) return vetor[tamanho/2];
  return (vetor[tamanho / 2 - 1] + vetor[tamanho / 2]) / 2.0;
}

float media(float *vetor, int tamanho){
  float soma = 0;
  for (int i = 0; i < tamanho; i++){
    soma += vetor[i];
  }
  return soma / tamanho;
}

float desvio_padrao(const float *vetor, int tamanho, float med)
{
  if ( tamanho< 2) return 0;
  float soma = 0;
  for (int i = 0; i < tamanho; i++)
  {
    float d = vetor[i] - med;
    soma += d * d;
  }
  return sqrt(soma / (tamanho - 1));
}


float CalculaPorcentagem(float dist, int h_lixeira_vazia, int h_lixeira_cheia){
  float p = (h_lixeira_vazia - dist) * 100.0 / (h_lixeira_vazia - h_lixeira_cheia);
  if (p > 100) p = 100;
  else if (p < 0) p = 0;

  return p;
}