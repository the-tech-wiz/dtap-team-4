#include "Arduino.h"
#include <WiFi.h>
#include "ESP32MQTTClient.h"
#include "esp_idf_version.h"  // check IDF version
#include <ArduinoJson.h>
#include <string>
using std::string;

constexpr char ssid[] = "aalto open";
constexpr char pass[] = "";
// TODO: Receive statuses and parse
// TODO: Drive components
// Test Mosquitto server, see: https://test.mosquitto.org
constexpr char server[] = "mqtt://35.228.212.78:1883";

constexpr char deviceId[] = "device-1";  // repeat bc lazy and string concat annoying
constexpr char subscribeTopic[] = "device/device-1/command";
constexpr char publishTopic[] = "device/device-1/status";  //test

bool online = true;
bool playing = false;
string trackId = "";
int volume = 0;

ESP32MQTTClient mqttClient;  // all params are set later

//NOTE! SET ARDUINO CORE DEBUG LEVEL TO INFO+ TO GET LOGGING
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
  mqttClient.setOnMessageCallback([](const string &topic, const string &payload) {
    log_i("Global callback: %s: %s", topic.c_str(), payload.c_str());
    if (topic.ends_with("command")) {
      handleCommand(payload);
    } else {
      log_e("Message not a command");
    }
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
  WiFi.setHostname("c3test");
  mqttClient.loopStart();
  Serial.print("ESP32 - Connecting to MQTT broker");

  while (!mqttClient.isConnected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
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
  log_i("Sent status: %s", output.c_str());
}

// TODO: telemetry? what telemetry?
void sendTelemetry() {
  // JsonDocument telemetry;
  // telemetry["online"] = online;

  // string output;
  // serializeJson(telemetry,output);
  // mqttClient.publish(publishTopic,output,0,false);
  // log_i((string("Sent telemetry: ") + output).c_str());
}

void handleCommand(const std::string &payload) {
  log_i("Executing command: %s", payload.c_str());
  JsonDocument command;
  auto err = deserializeJson(command, payload);
  if (err) {
    log_e("Command not in valid JSON");
    return;
  }
  string type = command["command"];
  if (type == "playTrack") {
    trackId = command["payload"]["trackId"].as<string>();
    log_i("Now playing %s", trackId.c_str());
    playing = true;
  } else if (type == "stopPlayback") {
    log_i("Stopped playing");
    playing = false;
  } else if (type == "setVolume") {
    volume = command["payload"]["volume"].as<int>();
    log_i("Volume is now: %i", volume);
  } else
    log_e("Command not recognized");
}

int pubCount = 0;

void loop() {
  // string msg = "Hello: " + std::to_string(pubCount++);
  // string msg;
  // serializeJson(doc,msg);
  // mqttClient.publish(publishTopic, msg, 0, false);
  sendStatus();
  delay(3000);
}

void onMqttConnect(esp_mqtt_client_handle_t client) {
  Serial.println();
  Serial.println("Connected!");
  if (mqttClient.isMyTurn(client))  // can be omitted if only one client
  {
    mqttClient.subscribe(subscribeTopic, [](const string &payload) {
      log_i("%s: %s", subscribeTopic, payload.c_str());
    });

    Serial.println("Subscribed");
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
