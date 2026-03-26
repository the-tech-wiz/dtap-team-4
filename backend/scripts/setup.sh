#!/bin/bash
# ─────────────────────────────────────────────────────────────────────────────
# setup.sh — Run this once on a fresh Ubuntu 22.04 Google Cloud VM
# It installs Node.js, Mosquitto, PM2, configures everything, and starts it.
# Usage: bash setup.sh
# ─────────────────────────────────────────────────────────────────────────────
set -e  # Exit immediately on any error

BOLD="\033[1m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
RESET="\033[0m"

banner() { echo -e "\n${BOLD}${GREEN}── $1 ──${RESET}"; }
warn()   { echo -e "${YELLOW}⚠  $1${RESET}"; }

banner "System update"
sudo apt-get update -y
sudo apt-get upgrade -y

# ── Node.js 20 LTS ────────────────────────────────────────────────────────────
banner "Installing Node.js 20 LTS"
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -
sudo apt-get install -y nodejs

echo "Node.js: $(node --version)"
echo "npm:     $(npm --version)"

# ── PM2 (process manager) ─────────────────────────────────────────────────────
banner "Installing PM2"
sudo npm install -g pm2
echo "PM2: $(pm2 --version)"

# ── Mosquitto ─────────────────────────────────────────────────────────────────
banner "Installing Mosquitto MQTT broker"
sudo apt-get install -y mosquitto mosquitto-clients

# Stop Mosquitto before editing config
sudo systemctl stop mosquitto

# Copy our config file (overwrites the default)
sudo cp mosquitto/mosquitto.conf /etc/mosquitto/conf.d/custom.conf

# Disable the default config that ships with Mosquitto (it conflicts)
if [ -f /etc/mosquitto/mosquitto.conf ]; then
  sudo mv /etc/mosquitto/mosquitto.conf /etc/mosquitto/mosquitto.conf.bak
fi

# Write a minimal base config that pulls in our custom.conf
cat <<EOF | sudo tee /etc/mosquitto/mosquitto.conf
pid_file /run/mosquitto/mosquitto.pid
include_dir /etc/mosquitto/conf.d
EOF

sudo systemctl enable mosquitto
sudo systemctl start mosquitto
sleep 1  # Give it a moment to start

MOSQUITTO_STATUS=$(sudo systemctl is-active mosquitto)
echo "Mosquitto status: ${MOSQUITTO_STATUS}"

if [ "$MOSQUITTO_STATUS" != "active" ]; then
  warn "Mosquitto failed to start. Check logs: sudo journalctl -u mosquitto"
  exit 1
fi

# Quick local pub/sub test
banner "Testing Mosquitto broker"
mosquitto_sub -h localhost -t "test/setup" -C 1 &
SUB_PID=$!
sleep 0.5
mosquitto_pub -h localhost -t "test/setup" -m "broker-ok"
wait $SUB_PID
echo "Mosquitto pub/sub test passed ✓"

# ── Node.js app dependencies ──────────────────────────────────────────────────
banner "Installing Node.js dependencies"
npm install

mkdir -p logs

# ── Environment file ──────────────────────────────────────────────────────────
if [ ! -f .env ]; then
  cp .env.example .env
  warn "Created .env from .env.example — edit it if you need custom values."
fi

# ── PM2 startup (survive VM reboots) ─────────────────────────────────────────
banner "Configuring PM2 startup on boot"
# This command prints the exact sudo command you need to run — we execute it
pm2 startup systemd -u "$USER" --hp "$HOME" | grep "sudo env" | bash || true

# ── Start the app ─────────────────────────────────────────────────────────────
banner "Starting API server via PM2"
pm2 start ecosystem.config.js
pm2 save

echo ""
echo -e "${BOLD}${GREEN}══════════════════════════════════════${RESET}"
echo -e "${BOLD}${GREEN}  Setup complete!${RESET}"
echo -e "${BOLD}${GREEN}══════════════════════════════════════${RESET}"
echo ""
echo "Test your endpoints (replace VM_IP with your external IP):"
echo "  curl http://VM_IP:3000/health"
echo "  curl http://VM_IP:3000/health/ping"
echo ""
echo "Useful commands:"
echo "  pm2 status              → check process status"
echo "  pm2 logs mqtt-api       → live logs"
echo "  pm2 restart mqtt-api    → restart after code changes"
echo "  sudo systemctl status mosquitto  → broker status"
