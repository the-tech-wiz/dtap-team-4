//NOTE! SET ARDUINO CORE DEBUG LEVEL TO INFO+ TO GET LOGGING
#include "global.h" // hack for .ino weird concatenations

// ============================================================================
// Setup
// ============================================================================

void setup() {
  // --- Device configuration ---
  Serial.begin(115200);
  delay(300);
  setupOutput();
  setupSensors();
  setupCloud();
  // vibrator only
  // pinMode(38, OUTPUT);
  // digitalWrite(38, HIGH);
  
}

// ============================================================================
// Loop
// ============================================================================

constexpr long STATUS_SEND_INTERVAL = 20000; //20s
unsigned long prevTime = 0;
unsigned long currTime = 0;
void loop() {
  if (detectShake()) triggerOutput();

  // nonblocking status send timer
  currTime = millis();
  if (currTime - prevTime >= STATUS_SEND_INTERVAL) {
    prevTime = currTime;
    sendStatus();
  }

  // digitalWrite(38,HIGH);
  // delay(1000);
  // digitalWrite(38,LOW);
  // delay(1000);

}

