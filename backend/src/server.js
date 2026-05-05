require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mqttClient = require('./mqtt/client');
const healthRoutes = require('./routes/health');
const deviceRoutes = require('./routes/device');

const app = express();
const PORT = process.env.PORT || 3000;

// ── Middleware ────────────────────────────────────────────────────────────────
app.use(cors());
app.use(express.json());

// Log every incoming request
app.use((req, res, next) => {
  console.log(`[API] ${req.method} ${req.path}`, Object.keys(req.body).length ? req.body : '');
  next();
});

// Attach MQTT helpers to every request so routes can publish/read state
app.use((req, res, next) => {
  req.mqtt = mqttClient;
  next();
});

// ── Routes ────────────────────────────────────────────────────────────────────
app.use('/health', healthRoutes);
app.use('/device', deviceRoutes);

// 404 catch-all
app.use((req, res) => {
  res.status(404).json({ error: 'Route not found' });
});

// Global error handler
app.use((err, req, res, next) => {
  console.error('Unhandled error:', err);
  res.status(500).json({ error: 'Internal server error' });
});

// ── Start ─────────────────────────────────────────────────────────────────────
app.listen(PORT, () => {
  console.log(`[API] REST server running on port ${PORT}`);
  console.log(`[API] Health check → http://localhost:${PORT}/health`);
});

module.exports = app;
