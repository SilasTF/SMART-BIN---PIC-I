#ifndef NOTIFICATION_UTILS_H
#define NOTIFICATION_UTILS_H

// includes
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// constantes
#define WIFI_SSID  "iPhone"    // nome da rede wifi
#define WIFI_PASS  "12365789"   // senha da rede wifi
#define BOT_TOKEN  "9151049192:AAGKf25czvCEmKzeI5uaVc0upaDqzX7DSUY"   // token do bot do telegram (nao vou publicar o meu aqui)
#define CHAT_ID    "8652317542" // id do chat do telegram com o bot

#define TIME_OUT 10000 // em milissegundos

// cabecalhos

/**
 * @brief utiliza a o wifi e a senha definidos nas constantes conecta no wifi
 * @return true se o wifi foi conectado, false caso contrario
 */
bool conecta_wifi();

/**
 * @brief desconecta da rede wifi atual
 */
void desconecta_wifi();

/**
 * @brief envia uma mensagem de alerta leve pelo bot do telegram
 * @return true se foi possivel enviar, false caso contrario
 */
bool envia_mensagem_telegram(const char *msg);


#endif