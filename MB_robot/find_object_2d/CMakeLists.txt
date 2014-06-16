cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

ADD_DEFINITIONS(-DPROJECT_PREFIX='"find_object_2d"')
ADD_DEFINITIONS(-DPROJECT_VERSION='"0.2.2"')

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

find_package(OpenCV 2.4 REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtGui)

### Qt Gui stuff ###
SET(headers_ui 
	./src_x/MainWindow.h
	./src_x/AddObjectDialog.h
	./src_x/ObjWidget.h
	./src_x/Camera.h
	./src_x/ParametersToolBox.h
	./src/CameraROS.h
	./src/ObjectsDetectedCallback.h
	./src_x/AboutDialog.h
	./src_x/utilite/UPlot.h
	./src_x/rtabmap/PdfPlot.h
)

SET(uis
    ./src_x/ui/mainWindow.ui
    ./src_x/ui/addObjectDialog.ui
    ./src_x/ui/aboutDialog.ui
)

SET(qrc 
    ./src_x/resources.qrc
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(srcs_qrc ${qrc})

#Generate .h files from the .ui files
QT4_WRAP_UI(moc_uis ${uis})

#This will generate moc_* for Qt
QT4_WRAP_CPP(moc_srcs ${headers_ui})
### Qt Gui stuff  end###


SET(SRC_FILES 
	./src_x/MainWindow.cpp
	./src_x/AddObjectDialog.cpp
	./src_x/KeypointItem.cpp
	./src_x/QtOpenCV.cpp
	./src_x/Camera.cpp
	./src/CameraROS.cpp
	./src_x/ParametersToolBox.cpp
	./src_x/Settings.cpp
	./src_x/ObjWidget.cpp
	./src_x/AboutDialog.cpp
	./src_x/utilite/UPlot.cpp
	./src_x/utilite/UFile.cpp
	./src_x/utilite/UDirectory.cpp
	./src_x/rtabmap/PdfPlot.cpp
	${moc_srcs} 
	${moc_uis} 
	${srcs_qrc}
)

SET(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src_x
	${OpenCV_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR} # for qt ui generated in binary dir
)

INCLUDE(${QT_USE_FILE})

SET(LIBRARIES
    ${QT_LIBRARIES} 
    ${OpenCV_LIBS} 
)

#include files
INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

rosbuild_add_executable(find_object_2d src/find_object_2d_node.cpp ${SRC_FILES})
target_link_libraries(find_object_2d ${LIBRARIES})

rosbuild_add_executable(print_objects_detected src/print_objects_detected_node.cpp)

rosbuild_add_executable(objects_detected src/objects_detected.cpp)
target_link_libraries(objects_detected ${LIBRARIES})

