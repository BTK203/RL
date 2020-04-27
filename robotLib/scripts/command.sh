# Script for executing robotLib related commands.
# One can start a project, compile a project, and do related things from here.
# Written By: Brach Knutson
#

invokedDir=$PWD
cd /home/pi/robotLib/scripts

echo "Welcome to RobotLib!"
echo "Please enter an Action"
echo "[1] Install RobotLib"
echo "[2] Create a new project"
#other options here

read -p "Action: " action

echo
echo

if [ $action = "1" ]
then
bash install.sh
fi

if [ $action = "2" ]
then
bash createProject.sh
fi

cd $invokedDir
