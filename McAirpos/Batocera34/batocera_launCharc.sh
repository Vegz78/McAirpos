#!/bin/bash

export LD_LIBRARY_PATH=/lib32/arm-linux-gnueabihf
export SPA_PLUGIN_DIR="/lib32/arm-linux-gnueabihf/spa-0.2"
export PIPEWIRE_MODULE_DIR="/lib32/arm-linux-gnueabihf/pipewire-0.3"

chvt 1
/home/pi/McAirpos/McAirpos/launCharc/launCharc_arm64 $1
sleep 1
chvt 2

export LD_LIBRARY_PATH=
export SPA_PLUGIN_DIR=
export PIPEWIRE_MODULE_DIR=
