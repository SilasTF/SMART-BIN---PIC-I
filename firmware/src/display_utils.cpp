#include <Arduino.h>
#include "display_utils.h"

/**
 * @brief imprime uma mensagem de erro no display 16x2
 * @param lcd objeto display para a impressao
 */
void imprime_mensagem_erro(LiquidCrystal_I2C lcd)
{
  lcd.clearWriteError();
  lcd.clear();

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Erro na leitura!");
  lcd.setCursor(0, 1);
  lcd.print("Verificar sensor");
}

/**
 * @brief imprime a distancia e a porcentagem de lotacao no display
 * @param lcd objeto display para a impressao
 * @param dist distancia medida pelo sensor
 * @param prctg porcentagem de lotacao da lixeira
 */
void imprime_distancia_porcentagem(LiquidCrystal_I2C lcd, float dist, float prctg)
{
  lcd.clearWriteError();
  lcd.clear();

  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.printf("Dist: %.0f", dist);
  lcd.setCursor(0, 1);
  lcd.printf("Nivel: %.2f", prctg);
}

/**
 * @brief imprime "Smart Bin" na linha de cima e a porcentagem de lotacao abaixo
 * @param lcd objeto display para a impressao
 * @param prctg porcentagem de lotacao da lixeira
 */
void imprime_porcentagem(LiquidCrystal_I2C lcd, float prctg)
{
  lcd.clearWriteError();
  lcd.clear();

  lcd.backlight();

  lcd.setCursor(3, 0);
  lcd.print("Smart Bin");
  lcd.setCursor(0, 1);
  lcd.printf("Nivel: %.2f", prctg);
}

/**
 * @brief apaga o backlight do display
 * @param lcd objeto display a ser apagado
 */
void apaga_display(LiquidCrystal_I2C lcd)
{
  lcd.clearWriteError();
  lcd.clear();

  lcd.noBacklight();
}