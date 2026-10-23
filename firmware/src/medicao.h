#ifndef MEDICAO_H
#define MEDICAO_H

// constantes
#define NUM_MEDICOES 10
#define CTE_VEL_SOM 0.0344

// cabecalhos
float get_dist(byte trig_pin, byte echo_pin);
float medir_dist_uma_vez(byte trig_pin, byte echo_pin);
void ordena_vetor_dist(float vetor[], int tamanho);
float mediana_do_vetor_ordenado(float vetor[], int tamanho);
float media(float *vetor, int tamanho);
float desvio_padrao(const float *vetor, int tamanho, float med);



#endif MEDICAO_H