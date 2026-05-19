# API Reference

Base URL: `http://<VM_EXTERNAL_IP>:3000`  
Local development: `http://localhost:3000`

> **Note:** The VM does not currently have a static IP. Retrieve the current external IP with:
> ```bash
> gcloud compute instances describe mqtt-api-server --zone=europe-north1-a \
>   --format="get(networkInterfaces[0].accessConfigs[0].natIP)"
> ```

---

## Health

### GET /health

Returns the status of the API server and its MQTT broker connection.

**Request**
```
GET /health
```

**Response `200 OK`**
```json
{
  "status": "ok",
  "timestamp": "2025-01-15T10:30:00.000Z",
  "uptime": "42s",
  "services": {
    "api": "running",
    "mqtt": "connected"
  }
}
```

| Field | Type | Description |
|-------|------|-------------|
| `status` | string | Always `"ok"` if the server is reachable. |
| `timestamp` | string | ISO 8601 timestamp of the response. |
| `uptime` | string | Time since the server process started. |
| `services.api` | string | Always `"running"`. |
| `services.mqtt` | string | MQTT broker connection state: `"connected"` or `"disconnected"`. |

---

### GET /health/ping

Simple liveness check.

**Request**
```
GET /health/ping
```

**Response `200 OK`**
```json
{
  "pong": true,
  "timestamp": "2025-01-15T10:30:00.000Z"
}
```

---

## Device

### GET /device

Returns the cached state for all known devices. State is populated from incoming MQTT `status` and `telemetry` messages and held in memory.

**Request**
```
GET /device
```

**Response `200 OK`**
```json
{
  "devices": {
    "device-1": {
      "status": {
        "online": true,
        "playing": true,
        "trackId": 3,
        "volume": 50,
        "receivedAt": "2025-01-15T10:30:00.000Z"
      },
      "telemetry": {
        "receivedAt": "2025-01-15T10:29:55.000Z"
      }
    }
  }
}
```

> **Note:** Response shape depends on what MQTT messages have been received since the server started. No database is used — state resets on server restart. Each sub-object (`status`, `telemetry`) is the raw JSON payload from the device with a `receivedAt` field appended.

---

### GET /device/:id/state

Returns the cached state for a single device.

**Request**
```
GET /device/:id/state
```

**Path Parameters**

| Parameter | Type | Description |
|-----------|------|-------------|
| `id` | string | The device ID (e.g. `device-1`) |

**Response `200 OK`**
```json
{
  "deviceId": "device-1",
  "state": {
    "status": {
      "online": true,
      "playing": true,
      "trackId": 3,
      "volume": 50,
      "receivedAt": "2025-01-15T10:30:00.000Z"
    },
    "telemetry": {
      "receivedAt": "2025-01-15T10:29:55.000Z"
    }
  }
}
```

**Response `404 Not Found`**
```json
{
  "error": "Device not found or has not reported yet",
  "deviceId": "device-1"
}
```

Returned if no MQTT messages have been received for the given device ID since the server started.

---

### POST /device/:id/command

Publishes a command to a device via the MQTT broker on the topic `device/{id}/command`.

**Request**
```
POST /device/:id/command
Content-Type: application/json
```

**Path Parameters**

| Parameter | Type | Description |
|-----------|------|-------------|
| `id` | string | The target device ID (e.g. `device-1`) |

**Request Body**

```json
{
  "command": "playTrack",
  "payload": {
    "trackId": 3
  }
}
```

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `command` | string | Yes | The command name to send to the device |
| `payload` | object | No | Additional parameters for the command. Defaults to `{}` if omitted. |

**Supported commands**

| `command` | `payload` | Effect |
|-----------|-----------|--------|
| `playTrack` | `{ "trackId": <number> }` | Play audio track by ID from SD card |
| `stopPlayback` | — | Stop current playback |
| `setVolume` | `{ "volume": <0–100> }` | Set output volume |

**Response `200 OK`**
```json
{
  "success": true,
  "deviceId": "device-1",
  "command": "playTrack",
  "publishedTo": "device/device-1/command"
}
```

**Response `400 Bad Request`**
```json
{
  "error": "Missing required field: command"
}
```

**Response `500 Internal Server Error`**
```json
{
  "error": "Failed to send command to device"
}
```

Returned if the MQTT publish fails (e.g. broker is unreachable).

---

## MQTT Topic Reference

The API interacts with the following MQTT topics internally. These are not HTTP endpoints but are documented here for cross-team reference.

| Topic | Direction | Description |
|-------|-----------|-------------|
| `device/{id}/status` | Device → API | Device playback state (online, playing, trackId, volume) |
| `device/{id}/telemetry` | Device → API | Sensor readings and diagnostics |
| `device/{id}/command` | API → Device | Commands published by the POST endpoint |

The API subscribes to `device/+/status` (QoS 1) and `device/+/telemetry` (QoS 0) on startup.

Commands are published as JSON:
```json
{
  "command": "playTrack",
  "payload": { "trackId": 3 },
  "sentAt": "2025-01-15T10:30:00.000Z"
}
```

---

## Testing Locally

With the dev server running (`npm run dev`), you can test endpoints with curl:

```bash
# Health checks
curl http://localhost:3000/health
curl http://localhost:3000/health/ping

# Device state
curl http://localhost:3000/device
curl http://localhost:3000/device/device-1/state

# Send commands
curl -X POST http://localhost:3000/device/device-1/command \
  -H "Content-Type: application/json" \
  -d '{"command": "playTrack", "payload": {"trackId": 3}}'

curl -X POST http://localhost:3000/device/device-1/command \
  -H "Content-Type: application/json" \
  -d '{"command": "stopPlayback"}'

curl -X POST http://localhost:3000/device/device-1/command \
  -H "Content-Type: application/json" \
  -d '{"command": "setVolume", "payload": {"volume": 75}}'
```
