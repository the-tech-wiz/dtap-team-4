
// --- VIBRATOR ---
GroveVibrator vibrator = {"vibrator", VIBRATOR_PIN};

void outputSetup() {
  vibrator.setup();

  // test if parts working
  vibrator.pulseTest(3, 100, 100);
}
void outputLoop() {}
