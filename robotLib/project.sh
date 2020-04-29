#
# Bash script written to handle project tasks such as compilation and deployment.
# Written By: Brach Knutson
#

compile() {
	./tools/GenerateProjectMakefile `find project/*.cpp`
	make -j4 Main
}

compileController() {
	cd robotController
	make -s -j4 lib/RobotController.a
	cd ..
}

compileTools() {
	cd tools
	make -s -j4 all
	cd ..
}

run() {
	echo
	echo "Running Program..."
	echo 
	./Main
}

action=$1
echo "Action: $1"

if [ $1 = "compile" ];
then
#compile the project
compile
return 0
fi

if [ $1 = "run" ];
then
#run executable
run
return 0
fi

if [ $1 = "compileAndRun" ];
then
#compile and run
compile
run
#run
return 0
fi

if [ $1 = "compileRobotController" ];
then
compileController
return 0
fi

if [ $1 = "clean" ];
then
make cleanall
mkdir bin
compileController
compile
return 0
fi

if [ $1 = "localUpdate" ];
then
rm -r robotController
cp -r $RLDir/tools $PWD
cp -r $RLDir/robotController $PWD
compileController
compileTools

cp /home/pi/robotLib/project.sh $PWD
return 0
fi

if [ $1 = "deploy" ];
then
compile
#deploy things
echo "deploy"
run
return 0
fi

if [ $1 = "help" ];
then
echo "HELP!"
else
echo "hmmmm... The action you entered doesn't match what I can do. Try one of these:"
fi

echo
echo "RobotLib Project Helper"
echo "Usage: sh project.sh [action]"
echo
echo "Possible Actions:"
echo "compile                - Compile the RobotLib project. Executable will be called 'Main'"
echo "compileAndRun          - Compile and run a RobotLib project."
echo "compileRobotController - Compile RobotLib sources."
echo "clean                  - Cleans the project (deletes compiled files), and recompiles sources."
echo "localUpdate            - Copies the system's RobotController sources to the project's RobotController sources."
echo "deploy                 - Compile a RobotLib project and configure it to run on start."
echo "help                   - Show this message."

return 1
