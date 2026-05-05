#!/bin/bash
set -e

cd ~/dtap-team-4/frontend
git pull
npm install
npm run build
sudo cp -r dist/* /var/www/dtap-frontend/
echo "Frontend deployed."
