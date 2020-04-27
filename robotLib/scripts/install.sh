#!/usr/bin/bash

#
# Script for installing RobotLib.
# Written By: Brach Knutson
#

#methods here
InstallRLTools() {
	echo "Installing RL Tools"
	mkdir bin
	g++ -o bin/QuietGPIO QuietGPIO.cpp -lwiringPi
	g++ -o bin/SetupRLHostapd SetupRLHostapd.cpp
	
	sudo ln bin/QuietGPIO /usr/bin/quietgpio
	sudo ln bin/SetupRLHostapd /usr/bin/setuprlhostapd
	sudo ln command.sh /usr/bin/rlcommand
}

UninstallRLTools() {
	echo "Uninstalling RL Tools"
	
	rm -r bin
	sudo rm /usr/bin/quietgpio
	sudo rm /usr/bin/setuprlhostapd
	sudo rm /usr/bin/rlcommand
}

ConfigureDHCP() {
	echo "Configuring DHCP"
	sudo mv /etc/dhcpcd.conf /etc/dhcpcd.conf.orig
	sudo cp conffiles/dhcpcd.conf /etc/dhcpcd.conf
}

UnconfigureDHCP() {
	echo "Unconfiguring DHCP"
	sudo rm /etc/dhcpcd.conf
	sudo mv /etc/dhcpcd.conf.orig /etc/dhcpcd.conf
}

InstallOpenCV() {
	echo "Installing OpenCV"
	
	sudo apt-get --assume-yes install v4l-utils
	
	cd ..
	mkdir opencv
	wget https://github.com/opencv/opencv/archive/3.4.5.zip
	unzip 3.4.5.zip
	mkdir build
	cmake -S opencv-3.4.5 -B build \
	-DCMAKE_BUILD_TYPE=RELEASE \
	-DCMAKE_INSTALL_PREFIX=/usr/local \
	-DBUILD_TESTS=FALSE \
	-DBUILD_JAVA=FALSE \
	-DWITH_GTK=FALSE
	
	cd build
	sudo make -j3 install
	sudo ldconfig
	cd ../..
	cd scripts
}

UninstallOpenCV() {
	echo "Uninstalling OpenCV"
	
	cd ../opencv/build
	sudo make -j4 uninstall
	sudo ldconfig
	cd ../..
	rm -r opencv
	
	sudo apt-get -assume-yes remove v4l-utils
	
	cd scripts
}

InstallDnsmasq() {
	echo "Installing dnsmasq"
	sudo apt-get --assume-yes install dnsmasq
	sudo systemctl stop dnsmasq
	sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
	sudo cp conffiles/dnsmasq.conf /etc/dnsmasq.conf
}

UninstallDnsmasq() {
	echo "Uninstalling dnsmasq"
	sudo rm /etc/dnsmasq.conf
	sudo rm /etc/dnsmasq.conf.orig
	sudo apt-get --assume-yes remove dnsmasq
}

InstallHostapd() {
	echo "Installing hostapd"
	sudo apt-get --assume-yes install hostapd
	sudo systemctl stop hostapd
	echo
	sudo setuprlhostapd
}

UninstallHostapd() {
	echo "Uninstalling hostapd"
	sudo apt-get --assume-yes remove hostapd
	sudo rm -r /etc/hostapd
}

PromptReboot() {
	echo "A reboot is required for some settings to take effect."
	read -p "Reboot now? [y/n]: " reb
	if [ $reb = "y" ];
	then
	reboot
	fi
}

#script enters here
echo "Welcome to the RL installer!"
echo "What would you like to do?"
echo "[1]: Install/Update"
echo "[2]: Uninstall"
read -p "Enter an Action: " action

if [ $action = "1" ];
then
	#install
	echo "The following dependencies will be installed:"
	echo " - RobotLib Tools"
	echo " - Hostapd"
	echo " - Dnsmasq"
	echo " - Cmake"
	echo " - VNC"
	echo " - (Optional) Video4Linux"
	echo " - (Optional) OpenCV"
	echo "Some dependencies may take a very long time to install."
	read -p "Are you sure? [y/n]: " sure
	if [ $sure = "y" ];
	then
		read -p "Install OpenCV as well?" cv
		echo "Installing"
		InstallRLTools
		sudo apt-get --assume-yes install cmake
		sudo apt-get --assume-yes install realvnc-vnc-server
		ConfigureDHCP
		if [ $cv = "y" ];
		then
			InstallOpenCV
		fi
		InstallDnsmasq
		InstallHostapd
		PromptReboot
	else
		echo "Aborting."
		return 0
	fi

	echo "Install Complete!"
fi

if [ $action = "2" ];
then
	#uninstall
	echo "The following dependencies will be uninstalled:"
	echo " - RobotLib Tools"
	echo " - Hostapd"
	echo " - Dnsmasq"
	echo " - (Optional) Video4Linux"
	echo " - (Optional) OpenCV"
	echo
	echo "The following dependencies will still remain:"
	echo " - Cmake (Can be uninstalled with: sudo apt-get remove cmake)"
	echo " - VNC (Can e uninstalled with: sudo apt-get remove realvnc-vnc-server)"
	read -p "Are you sure? [y/n]: " sure
	if [ $sure = "y" ];
	then
		read -p "Uninstall OpenCV as well?" cv
		echo "Uninstalling"
		UninstallRLTools
		UnconfigureDHCP
		if [ $cv = "y" ];
		then
			UninstallOpenCV
		fi
		UninstallHostapd
		UninstallDnsmasq
		PromptReboot
	else
		echo "Aborting."
		return 0
	fi
	
	echo "Uninstall Complete!"
fi
