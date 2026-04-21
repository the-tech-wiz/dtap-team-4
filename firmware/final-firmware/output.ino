// Code concerning all output modules, e.g. audio players and vibration motors
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

// --- VIBRATOR ---
const int VIBRATOR_PIN = 32;
GroveVibrator vibrator = {"vibrator", VIBRATOR_PIN};

void outputSetup() {
  vibrator.setup();

  // test if parts working
  vibrator.pulseTest(3, 100, 100);
}

/**
 * Trigger outputs, such as vibration motors and speakers, etc.
 */
void triggerOutput() {
  vibrator.pulse(50);
  //speaker.play() TODO: speaker play
}