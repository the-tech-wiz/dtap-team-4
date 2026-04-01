/*
 * HARDWARE SETUP:
 * - Piezo Vibration sensor (pin 25)
 * - Grove Vibration motor (pin 32)
 * - MicroSD card adapter
 * - Amplifier AdaFruit PAM8302A
 * - Speaker
 *
 * TODO: Write code to test MicroSD card adapter and amplifier
 * TODO: Network interface code
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
#define DEVICE_NAME "Laugh-With-Me Buddy"  // <-- CHANGE THIS!

// ============================================================================
// Component headers
// ============================================================================
#include <components/GroveVibrator.h>
#include <WT2605C_Player.h>

// ============================================================================
// Create component instances
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
// Setup
// ============================================================================
void setup() {
    // --- Device configuration ---
    ProtoFlow.configureDevice(DEVICE_NAME, "ESP32");

    // --- Register all your components ---
    // Each .setup() call registers the component with ProtoFlow

    //outputs
    vibrator.setup();

    //sensors
    pinMode(piezoPin, INPUT);

    // Optional: output test
    vibrator.pulseTest(3, 100, 100);
}

// ============================================================================
// Loop
// ============================================================================
void loop() {
    // --- Update all your sensor components ---
    // Each .loop() call reads the sensor and sends data if needed
    sense();
    delay(10);
}

void sense() {
  //Update all sensor components

  int piezoState = analogRead(piezoPin);
  Serial.println(piezoState);
  if (vibrateState > 60) {
    triggerOutput();
  }
}

/**
 * Trigger outputs, such as vibration motors and speakers, etc.
 */
void triggerOutput() {
  vibrator.pulse(50);
  //speaker.play() TODO: speaker play
}
