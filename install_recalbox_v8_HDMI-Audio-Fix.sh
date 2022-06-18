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
SUCCESS=0
if [[ -d ./McAirpos ]]; then
   echo "McAirpos repository already present, countinuing without downloading..."
else
   echo "Fetching McAirpos..."
   if [[ -f /usr/bin/wget ]]; then
      echo "Trying clone_McAirpos..."
      if wget https://github.com/Vegz78/McAirpos/raw/master/McAirpos/clone_McAirpos/clone_McAirpos; then
        chmod +x clone_McAirpos
        if ./clone_McAirpos; then
           SUCCESS=1
           rm -rf /home/pi/McAirpos/.git
        fi
        rm clone_McAirpos
      fi
   fi
   if [[ ! $SUCCESS = 1 ]] && [[ -f /bin/tar ]]; then
      echo "git failed, trying wget..."
      if wget https://github.com/Vegz78/McAirpos/archive/master.tar.gz; then
         SUCCESS=1
      elif wget https://github.com/Vegz78/McAirpos/archive/master.tar.gz --no-check-certificate; then
         SUCCESS=1
      fi
      if [[ $SUCCESS = 1 ]]; then
         gzip -d ./master.tar.gz
         tar -xf ./master.tar -C /home/pi
         mv /home/pi/McAirpos-master /home/pi/McAirpos
         rm -f ./master.tar
      fi
   fi
   if [[ ! $SUCCESS = 1 ]]; then
      echo "git, wget or tar missing or failed, exiting script..."
      exit 1
   fi
fi
SUCCESS=

# 3. Set up MakeCode Arcade files
cp -r /home/pi/McAirpos/McAirpos/MakeCode/sd /
chmod -R 755 /sd
chmod -R 755 /home/pi/McAirpos

# 4. Set up EmulationStation
# Delete old es_systems.cfg backup file
if [[ -f /recalbox/share_init/system/.emulationstation/systemlist.xml_McAirpos.bak ]]; then
   rm -f /recalbox/share_init/system/.emulationstation/systemlist.xml_McAirpos.bak
fi
# Installation of EmulationStation system needed for McAirpos
if [[ -f /recalbox/share_init/system/.emulationstation/systemlist.xml ]]; then
   #Backup original es_systems.cfg file
   cp /recalbox/share_init/system/.emulationstation/systemlist.xml /recalbox/share_init/system/.emulationstation/systemlist.xml_McAirpos.bak
   # Removing MakeCode Arcade system, if present
   sed -i '/<system uuid="7f9da90c-5a4b-475d-a9c2-efdbfc0d9d78" name="MakeCode" fullname="MakeCode Arcade" platforms="MakeCode">/{:a;/<\/system>/!{N;ba;}};/<descriptor command="\/usr\/bin\/launCharc %ROM%" path="%ROOT%\/makecode" theme="makecode" extensions=".elf .ELF" icon="$F251"\/>/d;' /recalbox/share_init/system/.emulationstation/systemlist.xml
   # Appending updated MakeCode Arcade system from repository
   sed -i '$d' /recalbox/share_init/system/.emulationstation/systemlist.xml
   cat /home/pi/McAirpos/McAirpos/Recalbox8/systemlist.xml_MakeCode_RB >> /recalbox/share_init/system/.emulationstation/systemlist.xml
   echo "</systemList>" >> /recalbox/share_init/system/.emulationstation/systemlist.xml
   # Add MakeCode Arcade carbon theme
   cp -r /home/pi/McAirpos/McAirpos/Recalbox8/themes/recalbox-next/makecode /recalbox/share_init/system/.emulationstation/themes/recalbox-next/
else
   echo "Couldn't find the file /recalbox/share_init/system/.emulationstation/systemlist.xml, continuing script without..."
fi

# 5. Create MakeCode Arcade games folder for Recalbox, if not present
if [[ -d /recalbox/share/roms/makecode ]]; then
   echo "MakeCode Arcade games folder already present, continuing..."
else
   mkdir -p /recalbox/share/roms/makecode
   chmod -R 755 /recalbox/share/roms/makecode
fi

# 6. Initialize uinput-mapper
#cd /home/pi/McAirpos/McAirpos/uinput-mapper
#make
#cd /home/pi

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
ln -s /home/pi/McAirpos/McAirpos/launCharc/launCharc /usr/bin/launCharc
chmod -R 755 /usr/bin/launCharc
mount -o remount,ro /
echo "McAirpos finished installing!"
echo "Please add a .elf game to the MakeCode roms folder and reboot Recalbox for the changes to take effect."
echo ""
echo "Download MakeCode Arcade .elf game files from https://vegz78.github.io/McAirpos"
echo "Run MakeCode Arcade games from the Recalbox menu or from the Linux console/CLI:"
launCharc
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
