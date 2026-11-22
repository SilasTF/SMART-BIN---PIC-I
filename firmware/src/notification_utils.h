#ifndef NOTIFICATION_UTILS_H
#define NOTIFICATION_UTILS_H

// includes
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// constantes
#define TIME_OUT 10000        // em milissegundos
#define BIN_ID   "lixeira-1"  // identificador desta lixeira no painel

// cabecalhos

/**
 * @brief conecta numa rede wifi especifica
 * @return true se conectou, false caso contrario
 */
bool conecta_wifi_com(const char *ssid, const char *pass);

/**
 * @brief conecta no wifi salvo na configuracao (cfg)
 * @return true se o wifi foi conectado, false caso contrario
 */
bool conecta_wifi();

/**
 * @brief desconecta da rede wifi atual
 */
void desconecta_wifi();

/**
 * @brief envia uma mensagem de alerta leve pelo bot do telegram (usado quando nao ha painel)
 * @return true se foi possivel enviar, false caso contrario
 */
bool envia_mensagem_telegram(const char *msg);

/**
 * @brief envia a leitura ao painel online e aplica a configuracao que ele devolve
 *        (calibracao, intervalo de envio e wifi)
 * @param dist distancia medida pelo sensor (cm)
 * @return true se o painel confirmou o recebimento, false caso contrario
 */
bool envia_leitura(float dist);

#endif
