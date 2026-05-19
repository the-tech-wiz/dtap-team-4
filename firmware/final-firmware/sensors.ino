// accelerometer input with ADXL345
#include <Wire.h>
#include <ADXL345_WE.h>
#define ADXL345_I2CADDR 0x53
#define ACC1_PIN 1
#define ACC2_PIN 2
#define SHAKE_THRESHOLD 1.5

ADXL345_WE accel = ADXL345_WE(ADXL345_I2CADDR);
xyzFloat shift;
void sensorSetup() {
  // set up accelerometer
  Wire.setPins(ACC1_PIN, ACC2_PIN);
  Wire.begin();
  if (!accel.init()) {
    Serial.println("ADXL345 not connected");
  }

  accel.setDataRate(ADXL345_DATA_RATE_12_5);
  delay(100);
  Serial.print("Data rate: ");
  Serial.print(accel.getDataRateAsString());

  accel.setRange(ADXL345_RANGE_4G);
  accel.setLowPower(true);
  Serial.println("Sensor setup complete");
}
/**
 * Sense using all sensors, triggering outputs if needed
*/
void sense() {
  accel.getGValues(&shift);
  float mag = sqrt(shift.x*shift.x+shift.y*shift.y+shift.z*shift.z);
  // Serial.print("Sensor state:");
  // Serial.println(mag);
  if (mag > SHAKE_THRESHOLD) {
    triggerOutput();
  }
}
