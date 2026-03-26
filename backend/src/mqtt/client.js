const mqtt = require('mqtt');

const MQTT_URL      = process.env.MQTT_URL      || 'mqtt://localhost:1883';
const MQTT_USERNAME = process.env.MQTT_USERNAME || '';
const MQTT_PASSWORD = process.env.MQTT_PASSWORD || '';

// ── Connection options ────────────────────────────────────────────────────────
const options = {
  clientId:         `api-server-${Math.random().toString(16).slice(3)}`,
  clean:            true,
  connectTimeout:   4000,
  reconnectPeriod:  2000,
};

if (MQTT_USERNAME) {
  options.username = MQTT_USERNAME;
  options.password = MQTT_PASSWORD;
}

// ── Connect ───────────────────────────────────────────────────────────────────
const client = mqtt.connect(MQTT_URL, options);

// In-memory state store — keyed by deviceId.
// In a real app you'd replace this with a database (e.g. Firestore, PostgreSQL).
const deviceState = {};

// ── Event handlers ────────────────────────────────────────────────────────────
client.on('connect', () => {
  console.log(`[MQTT] Connected to broker at ${MQTT_URL}`);

  // Subscribe to status updates from any device: "device/{id}/status"
  client.subscribe('device/+/status', { qos: 1 }, (err) => {
    if (!err) console.log('[MQTT] Subscribed to device/+/status');
  });

  // Subscribe to telemetry (sensor readings, etc.): "device/{id}/telemetry"
  client.subscribe('device/+/telemetry', { qos: 0 }, (err) => {
    if (!err) console.log('[MQTT] Subscribed to device/+/telemetry');
  });
});

client.on('message', (topic, message) => {
  // topic format: "device/{deviceId}/{type}"
  const parts = topic.split('/');
  if (parts.length !== 3 || parts[0] !== 'device') return;

  const [, deviceId, type] = parts;
  console.log(`[MQTT] ${topic} → ${message.toString()}`);

  try {
    const payload = JSON.parse(message.toString());
    deviceState[deviceId] = {
      ...deviceState[deviceId],
      [type]: { ...payload, receivedAt: new Date().toISOString() },
    };
  } catch {
    // Not JSON — store raw value
    deviceState[deviceId] = {
      ...deviceState[deviceId],
      [type]: { raw: message.toString(), receivedAt: new Date().toISOString() },
    };
  }
});

client.on('error',     (err) => console.error('[MQTT] Error:', err));
client.on('reconnect', ()    => console.log('[MQTT] Reconnecting...'));
client.on('offline',   ()    => console.log('[MQTT] Client offline'));

// ── Helpers exposed to routes ─────────────────────────────────────────────────

/**
 * Publish a command to a specific device.
 * The physical device should subscribe to "device/{deviceId}/command".
 */
function publishCommand(deviceId, command, payload = {}) {
  return new Promise((resolve, reject) => {
    const topic   = `device/${deviceId}/command`;
    const message = JSON.stringify({
      command,
      payload,
      sentAt: new Date().toISOString(),
    });

    client.publish(topic, message, { qos: 1, retain: false }, (err) => {
      if (err) return reject(err);
      resolve({ topic, message });
    });
  });
}

/** Return the cached state for a single device, or null if unknown. */
function getDeviceState(deviceId) {
  return deviceState[deviceId] || null;
}

/** Return cached state for every device seen so far. */
function getAllDeviceStates() {
  return deviceState;
}

module.exports = { client, publishCommand, getDeviceState, getAllDeviceStates };
