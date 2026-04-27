/*
 * HARDWARE SETUP:
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
// Setup
// ============================================================================
void setup() {
    // --- Device configuration ---
    ProtoFlow.configureDevice(DEVICE_NAME, "ESP32");

    Serial.begin(115200);
    delay(300);
    outputSetup();
    sensorSetup();   
}

// ============================================================================
// Loop
// ============================================================================
void loop() {
    // Each .loop() call reads the sensor and sends data if needed
    sense();
    delay(150);
}



