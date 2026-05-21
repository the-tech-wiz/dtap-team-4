
#include "esp_idf_version.h"  // check IDF version
#include <WiFi.h>
#include <ArduinoJson.h>

constexpr char ssid[] = "aalto open";
constexpr char pass[] = "";

// Static IP of the GCloud server
constexpr char server[] = "mqtt://35.228.212.78:1883";

const string deviceId = "device-1";
const string subscribeTopic = "device/" + deviceId + "/command";
const string publishTopic = "device/" + deviceId + "/status";

ESP32MQTTClient mqttClient;

void setupCloud() {
  // == Wifi ==
  // setup
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("Laugh-With-Me Buddy");
  WiFi.begin(ssid, pass);

  // connect
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // == MQTT ==
  // setup
  mqttClient.enableDebuggingMessages();
  mqttClient.setURI(server);
  mqttClient.setMqttClientName("Laugh-With-Me Buddy ESP32S3");
  mqttClient.enableLastWillMessage("lwt", "I am going offline");
  mqttClient.setKeepAlive(30);
  mqttClient.setOnMessageCallback([](const string &topic, const string &payload) {
    log_i("Global callback: %s: %s", topic.c_str(), payload.c_str());
    if (topic.ends_with("command")) {
      handleCommand(payload);
    } else {
      log_e("Message not a command");
    }
  });

  // connect
  mqttClient.loopStart();
  Serial.print("ESP32 - Connecting to MQTT broker");
  while (!mqttClient.isConnected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.println("Online setup complete");
  online = true;
  // TODO: handle disconnects?
}

void sendStatus() {
  JsonDocument status;
  status["online"] = online;
  status["playing"] = playing;
  status["trackId"] = trackId.c_str();
  status["volume"] = volume;
  string output;
  serializeJson(status, output);
  mqttClient.publish(publishTopic, output, 0, false);
  Serial.printf("Sent status: %s\n", output.c_str());
}

void handleCommand(const std::string &payload) {
  log_i("Executing command: %s", payload.c_str());
  JsonDocument command;
  auto err = deserializeJson(command, payload);
  if (err) {
    log_e("Command not in valid JSON");
    return;
  }

  // TODO: drive components
  string type = command["command"];
  if (type == "playTrack") {
    trackId = command["payload"]["trackId"].as<string>();
    log_i("Now playing %s\n", trackId.c_str());
    triggerOutput();

  } else if (type == "stopPlayback") {  // TODO stopper
    Serial.println("Stopped playing");
    playing = false;
    sendStatus();

  } else if (type == "setVolume") {
    volume = command["payload"]["volume"].as<int>();
    Serial.printf("Volume is now: %i\n", volume);
    sendStatus();
  } else {
    Serial.println("Command not recognized");
  }
}

// boilerplate
void onMqttConnect(esp_mqtt_client_handle_t client) {
  Serial.println();
  Serial.println("Connected!");
  // // can be omitted if only one client
  // if (mqttClient.isMyTurn(client)) {
    mqttClient.subscribe(subscribeTopic, [](const std::string &payload) {
      log_i("%s: %s", subscribeTopic, payload.c_str());
    });
    Serial.println("Subscribed");
  // }
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
#endif  // IDF CHECK