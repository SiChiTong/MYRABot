cmake_minimum_required(VERSION 2.8.3)
project(myrabot_arm_base_b)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  control_msgs
  trajectory_msgs
  actionlib
  actionlib_msgs
  roscpp
  std_msgs
  message_generation
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
   DIRECTORY msg
   FILES
   Servos.msg
   ReadServos.msg
   WriteServos.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

generate_messages()

## Generate added messages and services with any dependencies listed here
#generate_messages(
#   DEPENDENCIES geometry_msgs std_msgs control_msgs trajectory_msgs actionlib_msgs
#)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES myrabot_arm_base_b
#  CATKIN_DEPENDS geometry_msgs actionlib control_msgs trajectory_msgs roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a cpp library
# add_library(myrabot_arm_base_b
#   src/${PROJECT_NAME}/myrabot_arm_base_b.cpp
# )

## Declare a cpp executable
# add_executable(myrabot_arm_base_b_node src/myrabot_arm_base_b_node.cpp)
add_executable(joint_states_brazo src/joint_states_brazo.cpp)
add_executable(control_v04 src/control_v0.4.cpp)
add_executable(pick src/pick.cpp)
add_executable(place src/place.cpp)
add_executable(teleoperador_teclado src/teleoperador_teclado.cpp)
add_executable(teleoperador_xbox src/teleoperador_xbox.cpp)
add_executable(teleoperador_xbox360 src/teleoperador_xbox360.cpp)
add_executable(controlador_moveit src/controlador_moveit.cpp)
add_executable(paper src/paper.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
#add_dependencies(myrabot_arm_base_b_node myrabot_arm_base_b_generate_messages_cpp)

add_dependencies(joint_states_brazo ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(control_v04 ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(pick ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(place ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(teleoperador_teclado ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(teleoperador_xbox ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(teleoperador_xbox360 ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(controlador_moveit ${${PROJECT_NAME}_EXPORTED_TARGETS})
add_dependencies(paper ${${PROJECT_NAME}_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(myrabot_arm_base_b_node
#   ${catkin_LIBRARIES}
# )

target_link_libraries(joint_states_brazo
	${catkin_LIBRARIES}
)
target_link_libraries(control_v04
	${catkin_LIBRARIES}
)
target_link_libraries(pick
	${catkin_LIBRARIES}
)
target_link_libraries(place
	${catkin_LIBRARIES}
)
target_link_libraries(teleoperador_teclado
    ${catkin_LIBRARIES}
)
target_link_libraries(teleoperador_xbox
    ${catkin_LIBRARIES}
)
target_link_libraries(teleoperador_xbox360
    ${catkin_LIBRARIES}
)
target_link_libraries(controlador_moveit
    ${catkin_LIBRARIES}
)
target_link_libraries(paper
    ${catkin_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS myrabot_arm_base_b myrabot_arm_base_b_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_myrabot_arm_base_b.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
