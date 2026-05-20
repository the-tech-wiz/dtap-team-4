// Code concerning all output modules, e.g. audio players and vibration motors
// ============================================================================
// OUTPUT
// - VIBRATION: Stepper motor
// TODO: what it called?
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
#define VOL_TO_GAIN 300.0

ESP32I2SAudio audio(DAC_BCK, DAC_LRCK, DAC_DIN);
File f;
uint8_t filebuff[512];
BackgroundAudioMP3 player(audio);

// handling failure
void fail() {
  while (1) {
    if (Serial.available()) Serial.println("Restarting");
    delay(1000);
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

/**
 * Trigger outputs, such as vibration motors and speakers, etc.
 * Lasts for as long as the sound is playing.
 */
void triggerOutput() {

  Serial.println("Playing audio and vibrating...");

  f = SD.open(trackId.c_str());
  if (!f) {
    Serial.printf("Unable to open %s\n", trackId.c_str());
    fail();
  } else Serial.printf("Opened file %s\n", trackId.c_str());

  playing = true;
  digitalWrite(MOTOR_PIN, HIGH);

  while (f) {
    if (player.availableForWrite() > 512) {
      int len = f.read(filebuff, 512);
      player.write(filebuff, len);

      if (len != 512) {
        f.close();
      }
      // Serial.println(len);
      // Serial.print("Sound left: ");
    }
  }

  digitalWrite(MOTOR_PIN, LOW);
  playing = false;
}
