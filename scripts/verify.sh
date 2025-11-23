#!/bin/bash

if [ -e "/bin/jumper" ]; then
    echo " [PASSED] jumper executable in bin/"
else
    echo " [ERR] jumper executable could not be found in bin/"
fi


if [ -e "/etc/jumper.conf" ]; then
    echo " [PASSED] jumper config in etc/"
else
    echo " [ERR] jumper config not found in etc/"
fi