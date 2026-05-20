// accelerometer input with ADXL345
#include <Wire.h>
#include <ADXL345_WE.h>
#define ADXL345_I2CADDR 0x53
#define ACC1_PIN 1
#define ACC2_PIN 2
#define SHAKE_THRESHOLD 1.5

ADXL345_WE accel = ADXL345_WE(ADXL345_I2CADDR);
xyzFloat shift;
void setupSensors() {
  // set up accelerometer
  Wire.setPins(ACC1_PIN, ACC2_PIN);
  Wire.begin();
  if (!accel.init()) {
    Serial.println("ADXL345 not connected");
  }

  accel.setDataRate(ADXL345_DATA_RATE_12_5);
  delay(100);
  log_i("Data rate: %i\n",accel.getDataRateAsString());

  accel.setRange(ADXL345_RANGE_4G);
  accel.setLowPower(true);
  Serial.println("Sensor setup complete");
}
/**
 * Detects whether device is being shook (large magnitude of shifting)
*/
bool detectShake() {
  accel.getGValues(&shift);
  float mag = sqrt(shift.x*shift.x+shift.y*shift.y+shift.z*shift.z);
  log_i("Sensor state: %g\n",mag);
  return (mag > SHAKE_THRESHOLD);
}
