// project-wide includes to work around weird arduino compilation and ordering
#include "ESP32MQTTClient.h"
#include <string>
using std::string; // in place of Arduino String project-wide for ESP32 MQTT
                   // purposes

// default audio file to play (testing/fallback)
constexpr char DEFAULT_TRACK_ID[] = "0";

// status variables (also transmitted via code)
bool online = false;
string trackId = DEFAULT_TRACK_ID;
bool playing = false;
int volume = 50; // [0-100] range
