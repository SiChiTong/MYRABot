/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "ParametersToolBox.h"
#include "Settings.h"
#include "CameraROS.h"
#include "ObjectsDetectedCallback.h"
#include <signal.h>


bool gui = true;
std::string objectsPath;

void my_handler(int s){
	QApplication::quit();
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "find_object_2d");
	QApplication app(argc, argv);

	ros::NodeHandle nh("~");
	nh.param("gui", gui, gui);
	nh.param("objects_path", objectsPath, objectsPath);
	ROS_INFO("gui=%d", (int)gui);
	ROS_INFO("objects_path=%s", objectsPath.c_str());

	CameraROS * cam = new CameraROS();
	MainWindow mainWindow(cam); // take ownership
	ObjectsDetectedCallback objDetectedCallback;
	QObject::connect(&mainWindow, SIGNAL(objectsFound(const QMap<int, QPair<QRect, QTransform> > &)), &objDetectedCallback, SLOT(publish(const QMap<int, QPair<QRect, QTransform> > &)));

	if(gui)
	{
		// Disable camera settings...
		mainWindow.parametersToolBox()->getParameterWidget(Settings::kCamera_1deviceId())->setEnabled(false);
		mainWindow.parametersToolBox()->getParameterWidget(Settings::kCamera_3imageHeight())->setEnabled(false);
		mainWindow.parametersToolBox()->getParameterWidget(Settings::kCamera_2imageWidth())->setEnabled(false);
		mainWindow.setSourceImageText(mainWindow.tr(
				"<qt>Camera subscribed to <b>%1</b> topic.<br/>"
				"You can remap the topic when starting the node: <br/>\"rosrun find_object_2d find_object_2d image:=your/image/topic\".<br/>"
				"</qt>").arg(QString::fromStdString(cam->subscribedTopic())));
		mainWindow.show();
		app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	}

	if(!objectsPath.empty())
	{
		QString path = objectsPath.c_str();
		if(path.contains('~'))
		{
			path.replace('~', QDir::homePath());
		}
		if(!mainWindow.loadObjects(path))
		{
			ROS_ERROR("No objects loaded from path \"%s\"", path.toStdString().c_str());
		}
	}

	mainWindow.startProcessing();

	// Catch ctrl-c to close the gui
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	return app.exec();
}
