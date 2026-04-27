
// TODO: SWITCH TO ACCELEROMETER
// --- PIEZO SENSOR ---
const int PIEZO_PIN = 34;

void sensorSetup(){
    pinMode(PIEZO_PIN, INPUT);
    Serial.println("Sensor setup complete");
}
/**
 * Sense using all sensors, triggering outputs if needed
*/
void sense() {
  int piezoState = analogRead(PIEZO_PIN);
  Serial.print("Sensor state:");
  Serial.println(piezoState);
  if (piezoState > 60) {
    triggerOutput();
  }
}
