
// ============================================================================
// STEP 1: Include the component headers you need (uncomment as needed)
// ============================================================================
// --- PIEZO SENSOR ---
const int PIEZO_PIN = 25;
void sensorSetup(){
    pinMode(PIEZO_PIN, INPUT);
}
/**
 * Sense using all sensors, triggering outputs if needed
*/
void sense() {
  int piezoState = analogRead(piezoPin);
  Serial.println(piezoState);
  if (piezoState > 60) {
    triggerOutput();
  }
}
