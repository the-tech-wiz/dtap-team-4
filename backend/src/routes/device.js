const express = require('express');
const router  = express.Router();

/**
 * GET /device
 * Returns the latest known state for all devices.
 * State is populated when a device publishes to device/{id}/status or /telemetry.
 */
router.get('/', (req, res) => {
  const devices = req.mqtt.getAllDeviceStates();
  res.json({ devices });
});

/**
 * GET /device/:id/state
 * Returns the latest cached state for a specific device.
 *
 * Example response:
 * {
 *   "deviceId": "sensor-01",
 *   "state": {
 *     "status":    { "online": true, "receivedAt": "..." },
 *     "telemetry": { "temp": 22.5, "humidity": 60, "receivedAt": "..." }
 *   }
 * }
 */
router.get('/:id/state', (req, res) => {
  const { id } = req.params;
  const state  = req.mqtt.getDeviceState(id);

  if (!state) {
    return res.status(404).json({
      error:    'Device not found or has not reported yet',
      deviceId: id,
    });
  }

  res.json({ deviceId: id, state });
});

/**
 * POST /device/:id/command
 * Send a command to a specific device via MQTT.
 * The device must subscribe to: device/{id}/command
 *
 * Request body:
 * {
 *   "command": "set_led",       ← required: what to do
 *   "payload": { "on": true }   ← optional: command parameters
 * }
 */
router.post('/:id/command', async (req, res) => {
  const { id }              = req.params;
  const { command, payload } = req.body;

  if (!command) {
    return res.status(400).json({ error: 'Missing required field: command' });
  }

  try {
    const result = await req.mqtt.publishCommand(id, command, payload || {});
    res.json({
      success:     true,
      deviceId:    id,
      command,
      publishedTo: result.topic,
    });
  } catch (err) {
    console.error('[API] Failed to publish command:', err);
    res.status(500).json({ error: 'Failed to send command to device' });
  }
});

module.exports = router;
