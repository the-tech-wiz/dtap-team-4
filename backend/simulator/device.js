/**
 * Device Simulator
 *
 * Pretends to be the physical hardware for local development.
 * Connects to the same local Mosquitto broker as the API, subscribes to
 * device commands, and publishes realistic status responses back.
 *
 * Run alongside the API:
 *   terminal 1 → npm run dev          (starts the Express API)
 *   terminal 2 → node simulator/device.js
 *
 * Then send a command with curl or Postman:
 *   POST http://localhost:3000/device/pelican-1/command
 *   { "command": "playTrack", "payload": { "trackId": 3 } }
 *
 * Then read the status back:
 *   GET  http://localhost:3000/device/pelican-1/state
 */

require('dotenv').config({ path: require('path').resolve(__dirname, '../.env') });
const mqtt = require('mqtt');

// ── Config ────────────────────────────────────────────────────────────────────

const DEVICE_ID  = process.env.SIM_DEVICE_ID || 'pelican-1';
const MQTT_URL   = process.env.MQTT_URL       || 'mqtt://localhost:1883';
const REPLY_DELAY_MS = 500; // simulate hardware processing time

// ── State ─────────────────────────────────────────────────────────────────────

let deviceStatus = {
  online:   true,
  playing:  false,
  trackId:  null,
  volume:   50,
};

// ── Connect ───────────────────────────────────────────────────────────────────

const client = mqtt.connect(MQTT_URL, {
  clientId: `simulator-${DEVICE_ID}-${Math.random().toString(16).slice(3)}`,
  clean: true,
});

client.on('connect', () => {
  console.log(`[SIM] Connected to broker at ${MQTT_URL}`);
  console.log(`[SIM] Simulating device: ${DEVICE_ID}`);

  client.subscribe(`device/${DEVICE_ID}/command`, { qos: 1 }, (err) => {
    if (!err) console.log(`[SIM] Subscribed to device/${DEVICE_ID}/command`);
  });

  // Announce that the device is online
  publishStatus();
  console.log('[SIM] Ready — waiting for commands...\n');
});

// ── Command handler ───────────────────────────────────────────────────────────

client.on('message', (topic, messageBuffer) => {
  let msg;
  try {
    msg = JSON.parse(messageBuffer.toString());
  } catch {
    console.warn('[SIM] Received non-JSON message, ignoring');
    return;
  }

  console.log(`[SIM] Received command: ${msg.command}`, msg.payload || '');

  // Route to the right handler
  const handlers = {
    playTrack,
    stopPlayback,
    setVolume,
  };

  const handler = handlers[msg.command];
  if (handler) {
    setTimeout(() => handler(msg.payload || {}), REPLY_DELAY_MS);
  } else {
    console.warn(`[SIM] Unknown command: "${msg.command}"`);
    setTimeout(() => publishStatus({ error: `Unknown command: ${msg.command}` }), REPLY_DELAY_MS);
  }
});

// ── Command implementations ───────────────────────────────────────────────────

function playTrack({ trackId }) {
  if (trackId === undefined) {
    console.warn('[SIM] playTrack called without trackId');
    return publishStatus({ error: 'Missing trackId' });
  }

  deviceStatus = { ...deviceStatus, playing: true, trackId };
  console.log(`[SIM] Playing track ${trackId}`);
  publishStatus();
}

function stopPlayback() {
  deviceStatus = { ...deviceStatus, playing: false, trackId: null };
  console.log('[SIM] Playback stopped');
  publishStatus();
}

function setVolume({ volume }) {
  if (volume === undefined || volume < 0 || volume > 100) {
    return publishStatus({ error: 'Volume must be 0–100' });
  }
  deviceStatus = { ...deviceStatus, volume };
  console.log(`[SIM] Volume set to ${volume}`);
  publishStatus();
}

// ── Helpers ───────────────────────────────────────────────────────────────────

function publishStatus(extra = {}) {
  const topic   = `device/${DEVICE_ID}/status`;
  const payload = JSON.stringify({ ...deviceStatus, ...extra });
  client.publish(topic, payload, { qos: 1, retain: false });
  console.log(`[SIM] Published status → ${payload}`);
}

client.on('error',     (err) => console.error('[SIM] Error:', err));
client.on('reconnect', ()    => console.log('[SIM] Reconnecting...'));
client.on('offline',   ()    => console.log('[SIM] Offline'));