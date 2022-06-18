#!/bin/bash

# 1. Make sure to start in directory /home/pi
if [[ ! -d /home/pi ]]; then
   C_USER=$USER
   sudo mkdir -p /home/pi
   sudo chmod 755 /home/pi
   sudo chown $C_USER:$C_USER /home/pi
   C_USER=
fi
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
        else
           rm clone_McAirpos
        fi
      fi
   fi
   if [[ ! $SUCCESS = 1 ]]; then
      echo "clone_McAirpos failed, trying git..."
      if [[ -f /usr/bin/git ]]; then
         echo "Git already installed..."
      else
         sudo apt update
         sudo apt install -y git-core
      fi
      if git clone https://github.com/Vegz78/McAirpos.git; then
         SUCCESS=1
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
sudo cp -r ~/McAirpos/McAirpos/MakeCode/sd /
sudo chown -R pi /sd&&sudo chgrp -R pi /sd&&sudo chmod -R 755 /sd

# 4. Set up EmulationStation
# Delete old es_systems.cfg backup file
if [[ -f /etc/emulationstation/es_systems.cfg_McAirpos.bak ]]; then
   sudo rm -f /etc/emulationstation/es_systems.cfg_McAirpos.bak
fi
# Installation of EmulationStation system need for McAirpos
if [[ -f /etc/emulationstation/es_systems.cfg ]]; then
   #Backup original es_systems.cfg file
   sudo cp /etc/emulationstation/es_systems.cfg /etc/emulationstation/es_systems.cfg_McAirpos.bak
   # Removing MakeCode Arcade system, if present
   sudo sed -i '/<system>/{:a;/<\/system>/!{N;ba;}};/<name>MakeCode<\/name>/d;' /etc/emulationstation/es_systems.cfg
   # Appending updated MakeCode Arcade system from repository
   sudo sed -i '$e cat /home/pi/McAirpos/McAirpos/EmulationStation/es_systems.cfg_MakeCode' /etc/emulationstation/es_systems.cfg
   # Add MakeCode Arcade carbon theme
   if [[ -d /etc/emulationstation/themes/carbon-2021 ]]; then
      sudo cp ~/McAirpos/McAirpos/EmulationStation/makecode/art/controller.svg /etc/emulationstation/themes/carbon-2021/art/controllers/makecode.svg
      sudo cp ~/McAirpos/McAirpos/EmulationStation/makecode/art/system.svg /etc/emulationstation/themes/carbon-2021/art/systems/makecode.svg
   else
      sudo cp -r ~/McAirpos/McAirpos/EmulationStation/makecode /etc/emulationstation/themes/carbon/
   fi
else
   echo "Couldn't find the file /etc/emulationstation/es_systems.cfg, continuing script without..."
fi

# 5. Create MakeCode Arcade games folder for RetroPie, if not present
if [[ -d ./RetroPie/roms/makecode ]]; then
   echo "MakeCode Arcade games folder already present, continuing..."
else
   sudo mkdir -p ~/RetroPie/roms/makecode
   sudo chown -R pi ~/RetroPie/roms/makecode&&sudo chgrp -R pi ~/RetroPie/roms/makecode&&sudo chmod -R 755 ~/RetroPie/roms/makecode
fi

# 6. Initialize uinput-mapper
cd ~/McAirpos/McAirpos/uinput-mapper
make
cd ~

# 7. Finish up
sudo ln -s ~/McAirpos/McAirpos/launCharc/launCharc /usr/bin/launCharc
echo "McAirpos finished installing!"
echo "Please add a .elf game to the MakeCode roms folder and reboot RetroPie for the changes to take effect."
echo ""
echo "Download MakeCode Arcade .elf game files from https://vegz78.github.io/McAirpos"
echo "Run MakeCode Arcade games from the RetroPie menu or from the Linux console/CLI:"
launCharc
echo ""
echo "For more details, please visit https://github.com/Vegz78/McAirpos"
exit 0
