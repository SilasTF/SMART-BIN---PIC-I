#include <ArduinoJson.h>
#include "notification_utils.h"
#include "setup_portal.h"

bool conecta_wifi_com(const char *ssid, const char *pass)
{
  int tempo_esperado = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

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

bool conecta_wifi()
{
  return conecta_wifi_com(cfg.ssid, cfg.pass);
}

void desconecta_wifi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

bool envia_mensagem_telegram(const char *msg)
{
  if (!conecta_wifi())
  {
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, String("https://api.telegram.org/bot") + cfg.token + "/sendMessage");
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(String("{\"chat_id\":\"") + cfg.chat + "\",\"text\":\"" + msg + "\"}");

  http.end();
  desconecta_wifi();
  return code == 200;
}

/**
 * @brief aplica a configuracao devolvida pelo painel e salva na flash se algo mudou
 */
static void aplica_config_painel(JsonObjectConst c)
{
  bool mudou = false;

  // calibracao
  float vazia = c["empty_cm"] | cfg.altura_vazia;
  float cheia = c["full_cm"] | cfg.altura_cheia;
  if (vazia > cheia && (fabs(vazia - cfg.altura_vazia) > 0.05 || fabs(cheia - cfg.altura_cheia) > 0.05))
  {
    cfg.altura_vazia = vazia;
    cfg.altura_cheia = cheia;
    mudou = true;
  }

  // intervalo entre envios
  uint32_t sync = c["sync_minutes"] | cfg.sync_min;
  if (sync >= 1 && sync != cfg.sync_min)
  {
    cfg.sync_min = sync;
    mudou = true;
  }

  // wifi: so troca se a nova rede realmente conectar (senao a lixeira ficaria inacessivel)
  const char *ssid = c["wifi_ssid"] | "";
  const char *pass = c["wifi_pass"] | "";
  if (ssid[0] && (strcmp(ssid, cfg.ssid) != 0 || strcmp(pass, cfg.pass) != 0) && conecta_wifi_com(ssid, pass))
  {
    strlcpy(cfg.ssid, ssid, sizeof(cfg.ssid));
    strlcpy(cfg.pass, pass, sizeof(cfg.pass));
    mudou = true;
  }

  if (mudou) cfg_salvar();
}

bool envia_leitura(float dist)
{
  if (!conecta_wifi())
  {
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(10000);
  http.begin(client, String(cfg.servidor) + "/api/ingest");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", cfg.chave);

  char corpo[128];
  snprintf(corpo, sizeof(corpo),
           "{\"bin_id\":\"%s\",\"distance_cm\":%.1f,\"empty_cm\":%.1f,\"full_cm\":%.1f}",
           BIN_ID, dist, cfg.altura_vazia, cfg.altura_cheia);

  int code = http.POST(corpo);
  String resposta = (code == 200) ? http.getString() : "";
  http.end();
  Serial.printf("Painel: HTTP %d\n", code);

  if (code == 200)
  {
    JsonDocument doc;
    if (deserializeJson(doc, resposta) == DeserializationError::Ok)
    {
      aplica_config_painel(doc["config"].as<JsonObjectConst>());
    }
  }

  desconecta_wifi();
  return code == 200;
}
