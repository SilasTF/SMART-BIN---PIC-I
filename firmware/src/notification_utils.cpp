#include "notification_utils.h"

/**
 * @brief utiliza a o wifi e a senha definidos nas constantes conecta no wifi
 */
bool conecta_wifi()
{
  int tempo_esperado = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tempo_esperado += 500;

    if (tempo_esperado >= TIME_OUT)
    {
      Serial.println("\nNao foi possivel conectar a rede!");
      WiFi.mode(WIFI_OFF);
      return false;
    }
  }

  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
  return true;
}

/**
 * @brief desconecta da rede wifi atual
 */
void desconecta_wifi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

/**
 * @brief envia uma mensagem pelo bot do telegram
 * @return true se foi possivel enviar, false caso contrario
 */
bool envia_mensagem_telegram(const char *msg)
{
  if (!conecta_wifi())
  {
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, String("https://api.telegram.org/bot") + BOT_TOKEN + "/sendMessage");
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(String("{\"chat_id\":\"") + CHAT_ID + "\",\"text\":\"" + msg + "\"}");

  http.end();
  desconecta_wifi();
  return code == 200;
}
