#! /bin/bash

set -e

echo "Making binary."
make

echo "Completed binary, moving to /bin directory and giving +x permissions."
chmod +x jumper
mv jumper /bin

echo "Success, generating base config."
cd /etc
touch jumper.conf

echo "Setup complete."