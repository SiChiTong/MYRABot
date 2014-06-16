#!/bin/bash

#MYRABot Script
FOLDERS="myrabot_arm_base myrabot_arm_model myrabot_gazebo_plugins myrabot_robot_model myrabot_roomba find_object_2d"

CORTINILLA_DE_ESTRELLAS="========================================================"
CORTINILLA_DOS=" ################################################################### " 

HELP_MESSAGE="This is the first test-environment script:\\n
        -c: lanza el cmake de los packages y compila todo\n
        -d: borra toda la información de compilaciones previas\n
        -i: instala las dependencias necesarias de PR2 y turtlebot\n
        -l: lanza el MYRABot en gazebo\n
        -t: arranca la teleoperación (es necesario instalar las app's de turtlebot) \n
	-x: about us
	"
        
START_MESSAGE="Welcome to MYRABot robot environment"

INSTALLING_MESSAGE=" >> Now we are going to install the PR2 and turtlebot needed packages <<"

ENDING_MESSAGE="It's done"

PARAM_MESSAGE="Este programa necesita parámetros para ser lanzado. Por favor ejecute:\n  \$ ./MB.sh -h"

ROS_FUERTE_PR2_DEPENDENCIES="ros-fuerte-pr2-controllers ros-fuerte-pr2-mechanism ros-fuerte-pr2-simulator"

ROS_FUERTE_TURTLEBOT_DEPENDENCIES="ros-fuerte-turtlebot ros-fuerte-turtlebot-apps ros-fuerte-turtlebot-simulator ros-fuerte-turtlebot-viz ros-fuerte-camera-umd"

message()
{

echo "
       				 ##
                               ##  ##
                              #     ##
                             ######## ##
                            ##::::::# # ##
                          ##::::::::::#  #
                         #::::::::::::##  #
                        #::::::::::::#::# #
                       #:::::::::::::#:::#
                       #:::::::::::::::::#
                      #:::::::::::::::::#:#
                      #:::::::::::::::::#:#
                     #::::::::::::::::::::#
                    #:::::::::::::::::::::#
                   #::::::::::::::::::::::#
                   #####:::#####::::::::::#
                  #     #:#    ##:::::::::#
                  #     ##       #::::::::#
                 #   ## #        #::::::::#
                 #   # #         #:::::::#
                  #    #         #::::#::#
                   X####     ##  #:::##:##
                   #::::#    ##  #::#:#::#
                  #:::::#        #::#:## #
                  #:::::##      #:::::#  #
                  #:::::::##  X#::::::#  #
                  #:::::::::##::::::#:#
                   ##########::::::::##
                  #:::::::::::##:::::::#
                 #::::::::::::::#::::#:#
                ##::::::::::::####::###
                #:::::::::::##  ##:::#
                ##::::::::## #  ##:::#
                 #### #####   ####:::#
                     #  #  #  #:#::::##
                     #  #  # #::#::::# #
                     #  #####:::#::::# ##
                    #####::::::##::::## #
                    ##:::::::::#:::::#  #
                     ###::::::#::::##    #
                       ##::###::::#      #
                      # ####X::::#       #
                     #  #::::::##        #
                     # ##:::::#          #
                     # ##::::#          ##
                   ##  ##::::#         #  #
             ##  ## ### #::## #       ##  ###
        # ###::###   #  #:#   #      ## ###::######
       #:#::#::#::#  #   #    #    ### #::#::#::#::#
      #::#::#::#::#                    #::#::#::##::#     #
 #########::#::#::######################::#::#::#:#########
         #::#::###                        ###::#::#  dD
          ## ##                            ## ##"


}


end_message()
{
  echo $CORTINILLA_DOS
  echo $ENDING_MESSAGE        
  echo $CORTINILLA_DOS
}

teleop()
{
    roslaunch turtlebot_teleop keyboard_teleop.launch
}

install_dependencies()
{
  echo $CORTINILLA_DE_ESTRELLAS
  echo "Installing PR2 packages"
  echo $CORTINILLA_DE_ESTRELLAS
  sudo apt-get install $ROS_FUERTE_PR2_DEPENDENCIES
  
  echo
  echo $CORTINILLA_DE_ESTRELLAS
  echo "Installing Turtlebot packages"
  echo $CORTINILLA_DE_ESTRELLAS
  sudo apt-get install $ROS_FUERTE_TURTLEBOT_DEPENDENCIES

  end_message
}

premaking()
{

for f in $FOLDERS; do  
echo $CORTINILLA_DE_ESTRELLAS
echo "              cmaking  $f"
echo $CORTINILLA_DE_ESTRELLAS
cd $f 
cmake .
cd ..

done


}

delete()
{
  echo $CORTINILLA_DE_ESTRELLAS
  echo "  Removing CMakeCache.txt, Makefile, CMakeFiles, cmake_install.cmake"
  echo $CORTINILLA_DE_ESTRELLAS
	
  rm `find . -name "CMakeCache.txt"`
  rm `find . -name "Makefile"`
  rm -rf `find . -name "CMakeFiles"`
  rm -rf `find . -name "cmake_install.cmake"`

  end_message
}


##############################################################################
# Script Start
##############################################################################
echo  $CORTINILLA_DOS
echo -e  $START_MESSAGE
echo -e $CORTINILLA_DOS


##############################################################################
# Check 3 arguments are given #
if [ $# -lt 1 ]
then
        echo -e $PARAM_MESSAGE
        exit
fi



while getopts ":dcilhtx" optname
  do
    case "$optname" in
      "d")
        echo "Option $optname is specified"
	delete
        ;;
      "c")
	echo "Option $optname is specified"
        premaking
        rosmake
        end_message
        ;;
      "h")
        echo -e $HELP_MESSAGE
        ;;
      "i")
	echo "Option $optname is specified"
        echo -e $INSTALLING_MESSAGE
        install_dependencies
        ;;
      "l")
        echo "Option $optname is specified"
        roslaunch myrabot_robot_model myrabot_gazebo.launch
        ;;
      "t")
        teleop
        ;;
      "x")
        message
        ;;
      "?")
        echo "Unknown option $OPTARG"
        ;;
      ":")
        echo "No argument value for option $OPTARG"
        ;;
      *)
      # Should not occur
        echo "Unknown error while processing options"
        ;;
    esac
   # echo "OPTIND is now $OPTIND"
  done

