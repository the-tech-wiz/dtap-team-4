#!/bin/bash
# ─────────────────────────────────────────────────────────────────────────────
# gcloud-provision.sh
# Run this from YOUR LOCAL MACHINE (not the VM) using the gcloud CLI.
# It creates the VM and opens the right firewall ports for you.
#
# Prerequisites:
#   1. Install the gcloud CLI: https://cloud.google.com/sdk/docs/install
#   2. Run: gcloud auth login
#   3. Run: gcloud config set project YOUR_PROJECT_ID
# ─────────────────────────────────────────────────────────────────────────────

# ── Configuration — edit these ────────────────────────────────────────────────
PROJECT_ID="project-3dfc83ab-516a-48d6-b9c"          # ← replace with your GCP project ID
REGION="europe-north1"                # Closest to Finland — change if needed
ZONE="${REGION}-a"
VM_NAME="mqtt-api-server"
MACHINE_TYPE="e2-small"               # ~$13/month; use e2-micro for free tier
DISK_SIZE="20GB"
IMAGE_FAMILY="ubuntu-2204-lts"
IMAGE_PROJECT="ubuntu-os-cloud"

# ── 1. Enable required APIs ───────────────────────────────────────────────────
echo "Enabling Compute Engine API..."
gcloud services enable compute.googleapis.com --project="$PROJECT_ID"

# ── 2. Create the VM ──────────────────────────────────────────────────────────
echo "Creating VM: $VM_NAME in $ZONE..."
gcloud compute instances create "$VM_NAME" \
  --project="$PROJECT_ID" \
  --zone="$ZONE" \
  --machine-type="$MACHINE_TYPE" \
  --image-family="$IMAGE_FAMILY" \
  --image-project="$IMAGE_PROJECT" \
  --boot-disk-size="$DISK_SIZE" \
  --boot-disk-type="pd-standard" \
  --tags="mqtt-api-server" \
  --metadata="enable-oslogin=TRUE"

# ── 3. Firewall rules ─────────────────────────────────────────────────────────
# The --tags flag on the VM means these rules only apply to VMs with that tag.

echo "Creating firewall rule for REST API (port 3000)..."
gcloud compute firewall-rules create allow-rest-api \
  --project="$PROJECT_ID" \
  --direction=INGRESS \
  --priority=1000 \
  --network=default \
  --action=ALLOW \
  --rules=tcp:3000 \
  --source-ranges=0.0.0.0/0 \
  --target-tags="mqtt-api-server" \
  --description="Allow inbound traffic on port 3000 for Node/Express API"

echo "Creating firewall rule for MQTT broker (port 1883)..."
gcloud compute firewall-rules create allow-mqtt \
  --project="$PROJECT_ID" \
  --direction=INGRESS \
  --priority=1000 \
  --network=default \
  --action=ALLOW \
  --rules=tcp:1883 \
  --source-ranges=0.0.0.0/0 \
  --target-tags="mqtt-api-server" \
  --description="Allow inbound MQTT traffic on port 1883 for Mosquitto"

# ── 4. Print the external IP ──────────────────────────────────────────────────
EXTERNAL_IP=$(gcloud compute instances describe "$VM_NAME" \
  --project="$PROJECT_ID" \
  --zone="$ZONE" \
  --format="get(networkInterfaces[0].accessConfigs[0].natIP)")

echo ""
echo "══════════════════════════════════════"
echo "  VM created successfully!"
echo "══════════════════════════════════════"
echo ""
echo "External IP: $EXTERNAL_IP"
echo ""
echo "SSH into your VM:"
echo "  gcloud compute ssh $VM_NAME --zone=$ZONE --project=$PROJECT_ID"
echo ""
echo "Or copy your project files to the VM first:"
echo "  gcloud compute scp --recurse ./mqtt-api $VM_NAME:~ --zone=$ZONE --project=$PROJECT_ID"
echo ""
echo "Then on the VM, run:"
echo "  cd mqtt-api && bash setup.sh"
