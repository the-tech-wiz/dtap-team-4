/*
Input:
- Piezo: pin 34

Output:
- ULN2003 driving a motor
- MAX98357A

I/O:
- MicroSD card
*/

#include <Stepper.h>
#include <BackgroundAudioMP3.h>
#include <ESP32I2SAudio.h>
#include <SD.h>


// piezo
const int piezo = 34;

// motor
const int stepsPerRevolution = 2048;

#define IN1 4
#define IN2 2
#define IN3 16
#define IN4 17

Stepper stepVibrator(stepsPerRevolution, IN1, IN3, IN2, IN4);

// DAC + amp
#define DAC_LRCK 32
#define DAC_BCK  25
#define DAC_DIN  33

// file name to play
#define DEFAULT_AUDIO "/default_laugh.mp3"

ESP32I2SAudio audio(DAC_BCK, DAC_LRCK, DAC_DIN);
File f;
uint8_t filebuff[512];
BackgroundAudioMP3 player(audio);

void setup() {
  Serial.begin(115200);

  // piezo
  pinMode(piezo, INPUT);

  // motor
  stepVibrator.setSpeed(90);

  // SD card
  if (!SD.begin()) {
    Serial.println("Fail to read SD card");
  } else Serial.println("Opened SD card");


  player.setGain(0.02);
  player.begin();
        // open file
    f = SD.open(DEFAULT_AUDIO);

  Serial.println("Done init");
}

void loop() {
  int vibrateState = analogRead(piezo);
  int stepperState = 1;
  Serial.printf("Vibration level: %u\n", vibrateState);
  if (vibrateState > 70) {
    Serial.println(f.size());
    if (!f) {
      Serial.printf("Unable to open %s", DEFAULT_AUDIO);
    }

    while (f && player.availableForWrite() > 512) {
      int len = f.read(filebuff, 512);
      Serial.println(len);
      player.write(filebuff, len);
      stepVibrator.step(stepperState);
      stepperState = (stepperState==1)?0:1;
      if(len != 512) {
        Serial.println("Done laughing");
      }
    }
  }
  delay(150);
}
