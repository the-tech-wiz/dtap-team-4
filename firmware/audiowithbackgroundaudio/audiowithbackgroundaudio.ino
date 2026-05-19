// Testing BackgroundAudio library with ESP32, SD card, ADXL345, MAX98357

// Play MP3 file from SD card

#include <BackgroundAudioMP3.h>
#include <ESP32I2SAudio.h>
#include <SD.h>
#include <Wire.h>
#include <ADXL345_WE.h>
#define ADXL345_I2CADDR 0x53

ADXL345_WE accel = ADXL345_WE(ADXL345_I2CADDR);

// I/O for DAC
#define DAC_LRCK 6
#define DAC_BCK  5
#define DAC_DIN  4

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
xyzFloat g;
bool shaked = false;

void setup() {
  Serial.begin(115200);
  while (!Serial.available()) {
    delay(1);
  }
  
  Serial.println("Start program");

  // set up motor
  pinMode(38, OUTPUT);

  // set up accelerometer
  Wire.setPins(1, 2);
  Wire.begin();
  if(!accel.init()) {
    Serial.println("ADXL345 not connected");
  }

  accel.setDataRate(ADXL345_DATA_RATE_12_5);
  delay(100);
  Serial.print("Data rate: ");
  Serial.print(accel.getDataRateAsString());

  accel.setRange(ADXL345_RANGE_4G);
  accel.setLowPower(true);

  // initialize SD card
  if (!SD.begin()) {
    Serial.println("Fail to read SD card");
    // fail();
  } else Serial.println("Opened SD card");

  f = SD.open(DEFAULT_AUDIO);
  if (!f) {
    Serial.printf("Unable to open %s", DEFAULT_AUDIO);
    // fail();
  } else Serial.printf("Opened file %s \n", DEFAULT_AUDIO);

  player.begin();
  player.setGain(0.2);

  digitalWrite(38, HIGH);

  Serial.println("Done Vibrate, Start playing");
}

void loop() {
  accel.getGValues(&g);
  
  float mag = sqrt(g.x * g.x + g.y * g.y + g.z * g.z);
  Serial.println(mag);
  while (f && player.availableForWrite() > 512) {
    int len = f.read(filebuff, 512);
    player.write(filebuff, len);
    
    if(len != 512) {
      f.close();
      digitalWrite(38, LOW);
    }
  }
}
