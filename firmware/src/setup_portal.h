#ifndef SETUP_PORTAL_H
#define SETUP_PORTAL_H

// includes
#include <Arduino.h>
#include "display_utils.h"

// constantes
#define PORTAL_SSID   "SmartBin-Setup"   // rede criada pela lixeira durante a configuracao
#define PORTAL_SENHA  "smartbin123"      // troque por outra (minimo 8 caracteres)

// configuracao salva na flash (substitui as constantes fixas no codigo)
struct Config {
  char ssid[33];       // nome da rede wifi
  char pass[64];       // senha da rede wifi
  char token[64];      // token do bot do telegram
  char chat[24];       // id do chat do telegram
  float altura_vazia;  // distancia sensor -> fundo, com a lixeira vazia (cm)
  float altura_cheia;  // distancia minima sensor -> lixo (cm)
};

extern Config cfg;

// cabecalhos

/**
 * @brief le a configuracao salva na flash para a variavel global cfg
 * @return true se a lixeira ja foi calibrada, false se e a primeira vez
 */
bool cfg_carregar();

/**
 * @brief indica se wifi, token e chat id foram preenchidos
 * @return true se da para enviar notificacoes, false caso contrario
 */
bool notificacoes_ativas();

/**
 * @brief cria a rede PORTAL_SSID e serve a pagina de configuracao.
 *        Ao salvar, grava na flash e reinicia o ESP32 (nao retorna).
 * @param lcd display para mostrar as instrucoes
 * @param trig_pin pino do trigger do HC-SR04 (usado no botao de medir)
 * @param echo_pin pino do echo do HC-SR04
 */
void portal_executa(LiquidCrystal_I2C lcd, byte trig_pin, byte echo_pin);

#endif
