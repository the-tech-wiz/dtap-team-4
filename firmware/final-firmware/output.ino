// Code concerning all output modules, e.g. audio players and vibration motors
// ============================================================================
// OUTPUT
// - VIBRATION: ULN2003 driving a stepper motor
// - AUDIO: MAX98357A Audio-SD card module
// I/O:
// - MicroSD card
// ============================================================================
// =============== SETUP ===================
// --- VIBRATING STEPPER MOTOR ---
#include <Stepper.h>
const int stepsPerRevolution = 2048;

#define IN1 4
#define IN2 2
#define IN3 16
#define IN4 17

Stepper stepVibrator(stepsPerRevolution, IN1, IN3, IN2, IN4);

// --- AUDIO-SD MODULE ---
#include <BackgroundAudioMP3.h>
#include <ESP32I2SAudio.h>
#include <SD.h>

// I/O for DAC
#define DAC_LRCK 32
#define DAC_BCK  25
#define DAC_DIN  33

// default audio file to play (testing/fallback)
#define DEFAULT_AUDIO "/default_laugh.mp3"

ESP32I2SAudio audio(DAC_BCK, DAC_LRCK, DAC_DIN);
File f;
uint8_t filebuff[512];
int stepperState = 1;
BackgroundAudioMP3 player(audio);

void outputSetup() {
  Serial.println("Init outputs");
  //VIBRATOR
  stepVibrator.setSpeed(90);

  // SD CARD AUDIO
  if (!SD.begin()) {
    Serial.println("Fail to read SD card");
    // fail();
  } else Serial.println("Opened SD card");

  // TODO: move this logic to when called by net call
  f = SD.open(DEFAULT_AUDIO);
  if (!f) {
    Serial.printf("Unable to open %s", DEFAULT_AUDIO);
    // fail();
  } else Serial.println("Opened file");

  player.begin();
  player.setGain(0.05);
  Serial.println("Init outputs complete");
}

/**
 * Trigger outputs, such as vibration motors and speakers, etc.
 * Lasts for as long as the sound is playing.
 */
void triggerOutput() {
    //Serial.println(f.size());
    if (!f) {
      Serial.printf("Unable to open %s", DEFAULT_AUDIO);
    }

    while (f && player.availableForWrite() > 512) {
      int len = f.read(filebuff, 512);
      //Serial.println(len);
      player.write(filebuff, len);

      //VIBRATE
      stepVibrator.step(stepperState);
      stepperState = (stepperState==1)?0:1;

      if(len != 512) {
        Serial.println("Done laughing");
        f = SD.open(DEFAULT_AUDIO);
      }
    }
}

