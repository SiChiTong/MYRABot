cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
find_package(Boost REQUIRED COMPONENTS thread signals system)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
rosbuild_add_boost_directories()
#rosbuild_add_executable(example examples/example.cpp)
#rosbuild_link_boost(${PROJECT_NAME} thread)
#target_link_libraries(example ${PROJECT_NAME})

rosbuild_add_executable(control_v0.4 src/control_v0.4.cpp)
rosbuild_add_executable(teleoperador_teclado src/teleoperador_teclado.cpp)
rosbuild_add_executable(joint_states_brazo src/joint_states_brazo.cpp)
rosbuild_add_executable(pick src/pick.cpp)
rosbuild_add_executable(place src/place.cpp)
rosbuild_add_executable(teleoperador_xbox src/teleoperador_xbox.cpp)
rosbuild_add_executable(teleoperador_xbox360 src/teleoperador_xbox360.cpp)

#  We need this for electric2fuerte migration
rosbuild_link_boost(control_v0.4 thread signals system)
rosbuild_link_boost(teleoperador_teclado thread signals system)
rosbuild_link_boost(joint_states_brazo thread signals system)
rosbuild_link_boost(pick thread signals system)
rosbuild_link_boost(place thread signals system)
rosbuild_link_boost(teleoperador_xbox360 thread signals system)
