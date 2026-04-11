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
  "mqtt": "connected"
}
```

| Field | Type | Description |
|-------|------|-------------|
| `status` | string | API server status. Always `"ok"` if the server is reachable. |
| `mqtt` | string | MQTT broker connection state. Either `"connected"` or `"disconnected"`. |

---

### GET /health/ping

Simple liveness check. Use this to confirm the server is reachable.

**Request**
```
GET /health/ping
```

**Response `200 OK`**
```json
{
  "message": "pong"
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
    "device-001": {
      "status": "online",
      "telemetry": {
        "temperature": 22.4,
        "humidity": 58
      },
      "lastSeen": "2025-01-15T10:30:00.000Z"
    }
  }
}
```

> **Note:** Response shape depends on what MQTT messages have been received since the server started. No database is used — state resets on server restart.

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
| `id` | string | The device ID (e.g. `device-001`) |

**Response `200 OK`**
```json
{
  "id": "device-001",
  "status": "online",
  "telemetry": {
    "temperature": 22.4,
    "humidity": 58
  },
  "lastSeen": "2025-01-15T10:30:00.000Z"
}
```

**Response `404 Not Found`**
```json
{
  "error": "Device not found"
}
```

Returned if no MQTT messages have been received for the given device ID.

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
| `id` | string | The target device ID (e.g. `device-001`) |

**Request Body**

```json
{
  "command": "set_led",
  "payload": {
    "on": true
  }
}
```

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `command` | string | Yes | The command name to send to the device |
| `payload` | object | No | Additional parameters for the command |

**Response `200 OK`**
```json
{
  "success": true,
  "topic": "device/device-001/command"
}
```

**Response `400 Bad Request`**
```json
{
  "error": "Missing required field: command"
}
```

---

## MQTT Topic Reference

The API interacts with the following MQTT topics internally. These are not HTTP endpoints but are documented here for cross-team reference.

| Topic | Direction | Description |
|-------|-----------|-------------|
| `device/{id}/status` | Device → API | Device online/offline status updates |
| `device/{id}/telemetry` | Device → API | Sensor readings and measurements |
| `device/{id}/command` | API → Device | Commands sent from the mobile app via POST endpoint |

The API subscribes to `device/+/status` and `device/+/telemetry` on startup.

---

## Testing Locally

With the dev server running (`npm run dev`), you can test endpoints with curl:

```bash
# Health checks
curl http://localhost:3000/health
curl http://localhost:3000/health/ping

# Device state
curl http://localhost:3000/device
curl http://localhost:3000/device/device-001/state

# Send a command
curl -X POST http://localhost:3000/device/device-001/command \
  -H "Content-Type: application/json" \
  -d '{"command": "set_led", "payload": {"on": true}}'
```
