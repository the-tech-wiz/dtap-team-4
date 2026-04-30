const API_BASE = import.meta.env.VITE_API_BASE ?? 'http://localhost:3000';
const DEVICE_ID = 'device-1';

async function post(path, body) {
  const res = await fetch(`${API_BASE}${path}`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });
  if (!res.ok) throw new Error(`Request failed: ${res.status}`);
  return res.json();
}

async function get(path) {
  const res = await fetch(`${API_BASE}${path}`);
  if (!res.ok) throw new Error(`Request failed: ${res.status}`);
  return res.json();
}

export function playTrack(trackId) {
  return post(`/device/${DEVICE_ID}/command`, {
    command: 'playTrack',
    payload: { trackId },
  });
}

export function stopPlayback() {
  return post(`/device/${DEVICE_ID}/command`, { command: 'stopPlayback' });
}

export function setDeviceVolume(volume) {
  return post(`/device/${DEVICE_ID}/command`, {
    command: 'setVolume',
    payload: { volume },
  });
}

export function getDeviceState() {
  return get(`/device/${DEVICE_ID}/state`);
}
