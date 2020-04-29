#!/usr/bin/bash

#
# Script for creating a RobotLib project.
# Written By: Brach Knutson
#

currentDir=$PWD

echo "Welcome to the RobotLib New Project creator!"
echo "Currently working in $currentDir"

read -p "Please enter the name of your project: " name
read -p "Please enter the directory for your new project: " directory
echo "A new RobotLib project called $name will be placed here: $directory"

read -p "Are you sure? [y/n] " sure

if [ $sure = "y" ]
then

echo "Creating Project..."
#create project
cd $directory
mkdir $name
cd $name
mkdir project

#copy RobotLib and project base into the new project
echo "Moving RobotLib and Project Base..."
cp -r $currentDir/../robotController $PWD
cp -r $currentDir/../tools $PWD
cp -r $currentDir/../projectBase/* project
cp $currentDir/../project.sh $PWD

#set up tools
echo "Setting up tools..."
cd tools
make -s setup
make -s -j4 all
cd ..

#compile RobotLib
echo "Compiling RobotLib sources..."
cd robotController
make setup
make -s -j4 lib/RobotController.a
cd ..

echo "Performing first-time compile..."
./tools/GenerateProjectMakefile `find project -name "*.cpp"`
make -s setup
sh project.sh compile

cd $currentDir

echo "Done!"

else

echo "Aborting."

fi

cd $currentDir
