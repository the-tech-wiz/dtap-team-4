// Code concerning all output modules, e.g. audio players and vibration motors
// ============================================================================
// OUTPUT
// - VIBRATION: 2 vibration motors
// - AUDIO: MAX98357A Audio module
// INPUT:
// - MicroSD card + reader component
// ============================================================================
// =============== SETUP ===================
// --- VIBRATING STEPPER MOTOR ---
#define MOTOR_PIN 38

// --- AUDIO-SD MODULE ---
#include <BackgroundAudioMP3.h>
#include <ESP32I2SAudio.h>
#include <SD.h>

// I/O for DAC
#define DAC_LRCK 6
#define DAC_BCK 5
#define DAC_DIN 4

// volume / VOL_TO_GAIN = gain
const float VOL_TO_GAIN = 50.0;

ESP32I2SAudio audio(DAC_BCK, DAC_LRCK, DAC_DIN);
File f;
uint8_t filebuff[512];
BackgroundAudioMP3Class<RawDataBuffer<64 *1024>> player(audio);

// handling failure
void fail() {
  while (1) {
    online = false;
    sendStatus();
    if (Serial.available()) Serial.println("Restarting");
    ESP.restart();
  }
}

void setupOutput() {
  Serial.println("=== Init outputs ===");
  //VIBRATOR
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);

  // SD CARD AUDIO
  if (!SD.begin()) {
    Serial.println("Fail to read SD card");
    fail();
  } else Serial.println("Opened SD card");

  player.begin();
  player.setGain(volume / VOL_TO_GAIN);
  Serial.println("Init outputs complete");
}

constexpr long TIMEOUT_INTERVAL = 6000; //6s

/**
 * Trigger outputs, such as vibration motors and speakers, etc.
 * Lasts for as long as the sound is playing.
 */

void triggerOutput() {
  //for timeout
  unsigned long playStartTime = 0;
  unsigned long playCurrTime = 0;
  //flush everything
  memset(filebuff,0,sizeof(filebuff));
  player.flush();
  f.close();

  Serial.println("Playing audio and vibrating...");
  player.setGain(volume / VOL_TO_GAIN);

  f = SD.open(("/" + trackId + ".mp3").c_str());
  if (!f) {
    Serial.printf("Unable to open %s\n", trackId.c_str());
    fail();
  } else Serial.printf("Opened file %s\n", trackId.c_str());

  playing = true;
  sendStatus();

  digitalWrite(MOTOR_PIN, HIGH);
  playStartTime = millis();
  playCurrTime = millis();

  while (f && player.availableForWrite()>512 && (playCurrTime - playStartTime <= TIMEOUT_INTERVAL)) {
    playCurrTime = millis();
    log_i("Can write: %d", player.availableForWrite());
    int len = f.read(filebuff, 512);
    player.write(filebuff, len);
    log_i("Len: %d", len);
    if (len != 512) {
      f.close();
    }
  }
  // f.close();

  digitalWrite(MOTOR_PIN, LOW);

  playing = false;
  sendStatus();
}
