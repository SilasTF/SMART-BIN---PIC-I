#ifndef MEDICAO_H
#define MEDICAO_H

// constantes
#define NUM_MEDICOES 10
#define CTE_VEL_SOM 0.0344

// cabecalhos

/**
 * @brief realiza medicoes de distancia e trata para amenizar medidas discrepantes
 * @param trig_pin pino do esp32 ligado ao trigger do HC-SR04
 * @param echo_pin pino do esp32 ligado ao echo do HC-SR04
 * @return float da distancia medida apos tratamento
 */
float get_dist(byte trig_pin, byte echo_pin);

/**
 * @brief realiza uma unica medicao de distancia usando o HC-SR04
 * @param trig_pin pino do esp32 ligado ao trigger do sensor
 * @param echo_pin pino do esp32 ligado ao echo do sensor
 * @return float da distancia medida sem nenhum tratamento
 */
float medir_dist_uma_vez(byte trig_pin, byte echo_pin);

/**
 * @brief ordena um vetor de float em ordem crescente
 * @param vetor vetor que armazena numeros float
 * @param tamanho quantidade de numeros contidos no vetor
 */
void ordena_vetor_dist(float vetor[], int tamanho);

/**
 * @brief obtem a mediana de um vetor float
 * @param vetor ponteiro para o vetor de float
 * @param tamanho tamanho do vetor
 * @return mediana do vetor passado como parametro
 */
float mediana_do_vetor_ordenado(float vetor[], int tamanho);

/**
 * @brief obtem a media de um vetor de float
 * @param vetor ponteiro para o vetor float
 * @param tamanho tamanho do vetor
 * @return media do vetor passado como parametro
 */
float media(float *vetor, int tamanho);

/**
 * @brief calcula o desvio padrao dos elementos de um vetor float
 * @param vetor ponteiro para o vetor float
 * @param tamanho tamanho do vetor
 * @param med media dos elementos do vetor
 * @return float do desvio padrao dos elementos do vetor
 */
float desvio_padrao(const float *vetor, int tamanho, float med);


/**
 * @brief calcula a porcentagem de lotacao da lixeira
 * @param dist distancia medida pelo sensor na tampa
 * @param h_lixeira_vazia distancia da tampa da lixeira ate a base
 * @param h_lixeira_cheia distancia minima que o conteudo da lixeira pode chegar do sensor
 * @return float da porcentagem de lotacao da lixeira
 */
float CalculaPorcentagem(float dist, int h_lixeira_vazia, int h_lixeira_cheia);

#endif MEDICAO_H