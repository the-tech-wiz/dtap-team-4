#include "ESP32MQTTClient.h"
#include <string>
using std::string;  //in place of Arduino String project-wide for ESP32 MQTT purposes

// default audio file to play (testing/fallback)
constexpr char DEFAULT_AUDIO[]= "/default_laugh.mp3";

// status variables (also transmitted via code)
bool online = false;
string trackId = DEFAULT_AUDIO;
bool playing = false;
int volume = 30;  // [1-100] range