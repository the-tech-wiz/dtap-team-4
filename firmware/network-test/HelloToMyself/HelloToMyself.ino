#include "Arduino.h"
#include <WiFi.h>
#include "ESP32MQTTClient.h"
#include "esp_idf_version.h"  // check IDF version
//NOTE! SET ARDUINO DEBUG LEVEL TO INFO+ TO GET LOGGING
const char ssid[] = "aalto open";
const char pass[] = "";

// Test Mosquitto server, see: https://test.mosquitto.org
const char server[] = "mqtt://broker.hivemq.com:1883";

const char subscribeTopic[] = "techwiz-esp32-001/loopback";
const char publishTopic[] = "techwiz-esp32-001/loopback";

ESP32MQTTClient mqttClient;  // all params are set later


void setup() {
  Serial.begin(115200);
  log_i();
  log_i("setup, ESP.getSdkVersion(): ");
  log_i("%s", ESP.getSdkVersion());

  Serial.println("System start");

  setupMqtt();
}

void setupMqtt() {
  //config
  mqttClient.enableDebuggingMessages();
  mqttClient.setURI(server);
  mqttClient.setMqttClientName("ESP32-WROOM-Paja");
  mqttClient.enableLastWillMessage("lwt", "I am going offline");
  mqttClient.setKeepAlive(30);
  mqttClient.setOnMessageCallback([](const std::string &topic, const std::string &payload) {
    log_i("Global callback: %s: %s", topic.c_str(), payload.c_str());
  });

  //wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("ESP32 - Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
//   WiFi.setHostname("c3test");
  mqttClient.loopStart();
  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqttClient.isConnected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
}

int pubCount = 0;

void loop() {
  std::string msg = "Hello: " + std::to_string(pubCount++);
  mqttClient.publish(publishTopic, msg, 0, false);
  delay(2000);
}

void onMqttConnect(esp_mqtt_client_handle_t client) {
  Serial.println();
  Serial.println("Connected!");
  if (mqttClient.isMyTurn(client))  // can be omitted if only one client
  {
    mqttClient.subscribe(subscribeTopic, [](const std::string &payload) {
      log_i("%s: %s", subscribeTopic, payload.c_str());
    });

    mqttClient.subscribe("bar/#", [](const std::string &topic, const std::string &payload) {
      log_i("%s: %s", topic.c_str(), payload.c_str());
    });
    Serial.println("Subscrizzled");
  }
}

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
esp_err_t handleMQTT(esp_mqtt_event_handle_t event) {
  mqttClient.onEventCallback(event);
  return ESP_OK;
}
#else   // IDF CHECK
void handleMQTT(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
  auto *event = static_cast<esp_mqtt_event_handle_t>(event_data);
  mqttClient.onEventCallback(event);
}
#endif  // // IDF CHECK
