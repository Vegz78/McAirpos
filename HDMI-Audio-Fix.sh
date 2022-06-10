#!/bin/bash

# 1. Check for directory /home/pi
cd /home/pi
if [[ $(pwd) = "/home/pi" ]]; then
   echo "Working direcory ok!: $(pwd)"
else
   echo "Something wrong with working directory, exiting script..."
exit 1
fi
mount -o remount,rw /

# 7. HDMI-Audio-Fix
cp /etc/modprobe.d/alsa-base.conf /etc/modprobe.d/alsa-base.conf_McAirpos.bak
cp /usr/share/alsa/cards/vc4-hdmi.conf /usr/share/alsa/cards/vc4-hdmi.conf_McAirpos.bak
cp /home/pi/McAirpos/McAirpos/Recalbox8.0/etc/modprobe.d/alsa-base.conf /etc/modprobe.d/alsa-base.conf
cp /home/pi/McAirpos/McAirpos/Recalbox8.0/usr/share/alsa/cards/vc4-hdmi.conf /usr/share/alsa/cards/vc4-hdmi.conf


# 8. Finish up
mount -o remount,ro /
echo "HDMI-fix finished installing!"
echo "Please reboot the system for the changes to take effect."
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
