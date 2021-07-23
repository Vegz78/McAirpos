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
      tar -zxf ./master.tar.gz -C /home/pi
      mv /home/pi/McAirpos-master /home/pi/McAirpos
      rm -f ./master.tar.gz
   else
      echo "wget or tar missing, exiting script..."
      exit 1
   fi
fi

# 3. Set up MakeCode Arcade files
cp -r /home/pi/McAirpos/McAirpos/MakeCode/sd /
chmod -R 755 /sd

# 4. Set up EmulationStation
# Delete old es_systems.cfg backup file
if [[ -f /etc/emulationstation/es_systems.cfg_McAirpos.bak ]]; then
   rm -f /etc/emulationstation/es_systems.cfg_McAirpos.bak
fi
# Installation of EmulationStation system needed for McAirpos
if [[ -f /etc/emulationstation/es_systems.cfg ]]; then
   #Backup original es_systems.cfg file
   cp /etc/emulationstation/es_systems.cfg /etc/emulationstation/es_systems.cfg_McAirpos.bak
   # Removing MakeCode Arcade system, if present
   sed -i '/<system>/{:a;/<\/system>/!{N;ba;}};/<name>MakeCode<\/name>/d;' /etc/emulationstation/es_systems.cfg
   # Appending updated MakeCode Arcade system from repository
   sed -i '$d' /etc/emulationstation/es_systems.cfg
   cat /home/pi/McAirpos/McAirpos/Recalbox/es_systems.cfg_MakeCode_RB >> /etc/emulationstation/es_systems.cfg
   cp /etc/emulationstation/es_systems.cfg /recalbox/share_init/system/.emulationstation/es_systems.cfg
   # Add MakeCode Arcade carbon theme
   cp -r /home/pi/McAirpos/McAirpos/Recalbox/makecode /etc/emulationstation/themes/recalbox-next/
else
   echo "Couldn't find the file /etc/emulationstation/es_systems.cfg, continuing script without..."
fi

# 5. Create MakeCode Arcade games folder for RetroPie, if not present
if [[ -d ./RetroPie/roms/makecode ]]; then
   echo "MakeCode Arcade games folder already present, continuing..."
else
   mkdir -p /recalbox/share/roms/makecode
fi
chmod -R 777 /recalbox/share/roms/makecode
# 6. Initialize uinput-mapper
#cd /home/pi/McAirpos/McAirpos/uinput-mapper
#make
#cd /home/pi

# 7. Finish up
echo "McAirpos finished installing!"
echo "Download MakeCode Arcade .elf game files from https://vegz78.github.io/McAirpos"
echo "Run MakeCode Arcade games from RetroPie or from the Linux console/CLI:"
ln -s /home/pi/McAirpos/McAirpos/launCharc/launCharc /usr/bin/launCharc
launCharc
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
