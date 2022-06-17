#!/bin/bash

# 1. Make sure to start in directory /home/pi
mount -o remount,rw /
mkdir -p /home/pi
cd /home/pi

if [[ $(pwd) = "/home/pi" ]]; then
   echo "Working direcory ok!: $(pwd)"
else
   echo "Something wrong with working directory, exiting script..."
exit 1
fi

# 2. 1nstall prerequesites and clone McAirpos repository
if [[ -d ./McAirpos ]]; then
   echo "McAirpos repository present, countinuing without downloading..."
else
   if [[ -f /usr/bin/wget ]] && [[ -f /bin/tar ]]; then
      echo "Fetching McAirpos..."
      wget https://github.com/Vegz78/McAirpos/archive/master.tar.gz
      gzip -d ./master.tar.gz
      tar -xf ./master.tar -C /home/pi
      mv /home/pi/McAirpos-master /home/pi/McAirpos
      rm -f ./master.tar
   else
      echo "wget or tar missing, exiting script..."
      exit 1
   fi
fi

# 3. Set up MakeCode Arcade files
cp -r /home/pi/McAirpos/McAirpos/MakeCode/sd /
chmod -R 755 /sd
chmod -R 755 /home/pi/McAirpos

# 4. Set up EmulationStation
# Delete old es_systems.cfg backup file
if [[ -f /etc/emulationstation/es_systems.cfg_McAirpos.bak ]]; then
   rm -f /etc/emulationstation/es_systems.cfg_McAirpos.bak
fi
# Installation of EmulationStation system needed for McAirpos
if [[ -f /etc/emulationstation/es_systems.cfg ]]; then
   #Backup original es_systems.cfg file
   cp /etc/emulationstation/es_systems.cfg /etc/emulationstation/es_systems.cfg_McAirpos.bak
   # Appending updated MakeCode Arcade system from repository
   if grep -q "Pi 4" /proc/device-tree/model; then
      cp /home/pi/McAirpos/McAirpos/Batocera34/configs/emulationstation/es_systems_makecode.cfg /userdata/system/configs/emulationstation
   else
      cp /home/pi/McAirpos/McAirpos/Batocera34/configs/emulationstation/es_systems_makecode.cfg_rpi3 /userdata/system/configs/emulationstation/es_system_makecode.cfg
   fi
   # Add MakeCode Arcade carbon theme
   cp -r /home/pi/McAirpos/McAirpos/Batocera34/art /etc/emulationstation/themes/es-theme-carbon/
   cp -r /home/pi/McAirpos/McAirpos/Batocera34/makecode /etc/emulationstation/themes/es-theme-carbon/
   cp /home/pi/McAirpos/McAirpos/MakeCode/MakeCode_Arcade.png /etc/emulationstation/themes/es-theme-carbon/art/consoles/makecode.png
else
   echo "Couldn't find the file /etc/emulationstation/es_systems.cfg, continuing script without..."
fi

# 5. Create MakeCode Arcade games folder for Batocera, if not present
if [[ -d /userdata/roms/makecode ]]; then
   echo "MakeCode Arcade games folder already present, continuing..."
else
   mkdir -p /userdata/roms/makecode
   chmod -R 755 /userdata/roms/makecode
fi

# 6. Provide 32-bit runtime environment
gzip -d /home/pi/McAirpos/McAirpos/Batocera34/batocera_re.tar.gz
tar xf /home/pi/McAirpos/McAirpos/Batocera34/batocera_re.tar -C /
ln -s /lib32/arm-linux-gnueabihf/ld-2.31.so /lib/ld-linux-armhf.so.3
cp -r /home/pi/McAirpos/McAirpos/Batocera34/usr /
if [[ -d /lib32 ]]; then
   echo "Runtime environment provided ok..."
else
   echo "Runtime environment is missing or faulty, exiting..."
   exit 1
fi

# 7. Initialize uinput-mapper
#cd /home/pi/McAirpos/McAirpos/uinput-mapper
#make
#cd /home/pi

# 8. Finish up
ln -s /home/pi/McAirpos/McAirpos/launCharc/launCharc /usr/bin/launCharc
chmod -R 755 /usr/bin/launCharc
/usr/bin/batocera-save-overlay 100
mount -o remount,ro /
echo "McAirpos finished installing!"
echo "Please add a .elf game to the MakeCode roms folder and reboot Batocera for the changes to take effect."
echo ""
echo "Download MakeCode Arcade .elf game files from https://vegz78.github.io/McAirpos"
echo "Run MakeCode Arcade games from the Batocera menu or from the Linux console/CLI:"
launCharc
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
