// Testing BackgroundAudio library with ESP32, SD card, PCM5102 (DAC) and PAM8302A (amp)

// Play MP3 file from SD card

#include <BackgroundAudioMP3.h>
#include <ESP32I2SAudio.h>
#include <SD.h>

// I/O for DAC
#define DAC_LRCK 32
#define DAC_BCK  25
#define DAC_DIN  33

// file name to play as default/fallback audio
#define DEFAULT_AUDIO "/default_laugh.mp3"

ESP32I2SAudio audio(DAC_BCK, DAC_LRCK, DAC_DIN);
File f;
uint8_t filebuff[512];
BackgroundAudioMP3 player(audio);

// handling failure
// void fail() {
//   while (1) {
//     if (Serial.available()) Serial.println("Restarting");
//     delay(1000);
//     ESP.restart();
//   }
// }

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("Start program");

  // initialize SD card
  if (!SD.begin()) {
    Serial.println("Fail to read SD card");
    // fail();
  } else Serial.println("Opened SD card");

  f = SD.open(DEFAULT_AUDIO);
  if (!f) {
    Serial.printf("Unable to open %s", DEFAULT_AUDIO);
    // fail();
  } else Serial.println("Opened file");

  player.begin();
  player.setGain(0.1);
  Serial.println("Start playing");
}

void loop() {
  int count = 0;
  while (f && player.availableForWrite() > 512) {
    int len = f.read(filebuff, 512);
    player.write(filebuff, len);
    if(len != 512) {
      f.close();
    }
  }
}
