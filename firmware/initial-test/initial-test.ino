/*
 * HARDWARE SETUP:
 * - Piezo Vibration sensor (pin 25)
 * - Grove Vibration motor (pin 32)
 * - MicroSD card adapter
 * - Amplifier AdaFruit PAM8302A
 * - Speaker
 *
 * TODO:
 * Write code to test MicroSD card adapter and amplifier
 *
 * Debug Levels (set PROTOFLOW_DEBUG_LEVEL before including library):
 * 0 = No debug output (production)
 * 3 = Errors + Warnings + Info (recommended for development)
 * 5 = Verbose (detailed debugging)
 */

#define PROTOFLOW_DEBUG_LEVEL 3
#include <AaltoProtoFlow.h>


// ============================================================================
// YOUR PROJECT NAME - This appears in the PWA device list
// ============================================================================
#define DEVICE_NAME "Laugh box"  // <-- CHANGE THIS!

// ============================================================================
// STEP 1: Include the component headers you need (uncomment as needed)
// ============================================================================
#include <components/GroveVibrator.h>
#include <WT2605C_Player.h>

// ============================================================================
// STEP 2: Create your component instances
//
// Configure each component with:
//   - A unique name (appears in PWA)
//   - The GPIO pin number
//   - Component-specific settings
//
// IMPORTANT: Each sensor/actuator name must be unique!
// ============================================================================

// --- PIEZO SENSOR ---
const int piezoPin = 25;

// --- VIBRATOR ---
GroveVibrator vibrator = {"vibrator", 32};


// ============================================================================
// STEP 3: Setup - runs once at startup
// ============================================================================
void setup() {
    // --- Device configuration ---
    ProtoFlow.configureDevice(DEVICE_NAME, "ESP32");

    // --- Register all your components ---
    // Each .setup() call registers the component with ProtoFlow
    vibrator.setup();
    pinMode(piezoPin, INPUT);
    pinMode(19, OUTPUT);
    digitalWrite(19, HIGH);
    delay(500);
    digitalWrite(19, LOW);

    // Optional: LED startup blink test
    vibrator.pulseTest(3, 100, 100);

}

// ============================================================================
// STEP 4: Loop - runs continuously
// ============================================================================
void loop() {
    // --- Update all your sensor components ---
    // Each .loop() call reads the sensor and sends data if needed
    int vibrateState = analogRead(piezoPin);
    Serial.println(vibrateState);
    delay(10);
    if (vibrateState > 60) {
        vibrator.pulse(50);
    }


}
