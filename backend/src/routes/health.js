const express = require('express');
const router  = express.Router();

/**
 * GET /health
 * Returns API status + MQTT connection state.
 * Use this to verify everything is wired up correctly.
 */
router.get('/', (req, res) => {
  const mqttConnected = req.mqtt.client.connected;

  res.json({
    status:    'ok',
    timestamp: new Date().toISOString(),
    uptime:    `${Math.floor(process.uptime())}s`,
    services: {
      api:  'running',
      mqtt: mqttConnected ? 'connected' : 'disconnected',
    },
  });
});

/**
 * GET /health/ping
 * Simplest possible liveness check — good for load balancers / monitoring.
 */
router.get('/ping', (req, res) => {
  res.json({ pong: true, timestamp: new Date().toISOString() });
});

module.exports = router;
