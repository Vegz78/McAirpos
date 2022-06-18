#!/bin/bash

# 1. Check for directory /home/pi/McAirpos and change working directory
if [[ -d /home/pi/McAirpos ]]; then
   echo "McAirpos found!"
else
   echo "McAirpos not found, exiting script..."
   exit 1
fi
cd /home/pi
if [[ $(pwd) = "/home/pi" ]]; then
   echo "Working direcory ok!: $(pwd)"
else
   echo "Something wrong with working directory, exiting script..."
exit 1
fi
mount -o remount,rw /

# 7. HDMI-Audio-Fix
if [[ -d /home/pi/McAirpos/McAirpos/Recalbox8 ]]; then
   cp /etc/modprobe.d/alsa-base.conf /etc/modprobe.d/alsa-base.conf_McAirpos.bak
   cp /usr/share/alsa/cards/vc4-hdmi.conf /usr/share/alsa/cards/vc4-hdmi.conf_McAirpos.bak
   cp /home/pi/McAirpos/McAirpos/Recalbox8/etc/modprobe.d/alsa-base.conf /etc/modprobe.d/alsa-base.conf
   cp /home/pi/McAirpos/McAirpos/Recalbox8/usr/share/alsa/cards/vc4-hdmi.conf /usr/share/alsa/cards/vc4-hdmi.conf
else
   echo "Folder /home/pi/McAirpos/McAirpos/Recalbox8 not found, please update McAirpos with \"rm -rf /home/pi/McAirpos\" and run installation script again..."
   exit 1
fi

# 8. Finish up
mount -o remount,ro /
echo "HDMI-fix finished installing!"
echo "Please reboot the system for the changes to take effect."
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
