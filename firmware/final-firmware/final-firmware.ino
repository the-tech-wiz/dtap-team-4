//NOTE! SET ARDUINO CORE DEBUG LEVEL TO INFO+ TO GET LOGGING
#include "global.h" // hack for .ino weird concatenations

// ============================================================================
// Setup
// ============================================================================
constexpr long STATUS_SEND_INTERVAL = 3000;
unsigned long prevTime = 0;
unsigned long currTime = 0;
void setup() {
  // --- Device configuration ---
  Serial.begin(115200);
  delay(300);
  setupOutput();
  setupSensors();
  setupCloud();
}

// ============================================================================
// Loop
// ============================================================================


void loop() {
  if (detectShake()) triggerOutput();

  //nonblocking status send timer
  // currTime = millis();
  // if (currTime - prevTime >= STATUS_SEND_INTERVAL) {
  //   prevTime = currTime;
  //   sendStatus();
  // }
  // delay(150);
}

