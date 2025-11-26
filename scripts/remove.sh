#! /bin/bash

echo -n "Are you sure you want to remove jumper.conf? [y/n] "
read response

if [[ "y" == "$response" || "yes" == "$response" ]]; then
    cd /bin
    rm jumper
    echo "Removed binary from bin/"
    
    cd /etc
    rm jumper.conf
    echo "Removed config from etc/"
fi
