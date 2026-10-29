#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

// includes
#include <LiquidCrystal_I2C.h>

// cabecalhos

/**
 * @brief imprime uma mensagem de erro no display 16x2
 * @param lcd objeto display para a impressao
 */
void imprime_mensagem_erro(LiquidCrystal_I2C lcd);

/**
 * @brief imprime a distancia e a porcentagem de lotacao no display
 * @param lcd objeto display para a impressao
 * @param dist distancia medida pelo sensor
 * @param prctg porcentagem de lotacao da lixeira
 */
void imprime_distancia_porcentagem(LiquidCrystal_I2C lcd, float dist, float prctg);

/**
 * @brief imprime "Smart Bin" na linha de cima e a porcentagem de lotacao abaixo
 * @param lcd objeto display para a impressao
 * @param prctg porcentagem de lotacao da lixeira
 */
void imprime_porcentagem(LiquidCrystal_I2C lcd, float prctg);

/**
 * @brief apaga o backlight do display
 * @param lcd objeto display a ser apagado
 */
void apaga_display(LiquidCrystal_I2C lcd);

#endif DISPLAY_UTILS_H