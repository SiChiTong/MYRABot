/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#ifndef CAMERAROS_H_
#define CAMERAROS_H_

#include "Camera.h"

#include <ros/ros.h>
#include <ros/spinner.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

class CameraROS : public Camera {
	Q_OBJECT
public:
	CameraROS(QObject * parent = 0);
	virtual ~CameraROS();

	virtual bool start();
	virtual void stop();

	std::string subscribedTopic() const {return image_sub_.getTopic();}

private slots:
	virtual void takeImage();

private:
	void imgReceivedCallback(const sensor_msgs::ImageConstPtr & msg);

private:
	image_transport::Subscriber image_sub_;
};

#endif /* CAMERAROS_H_ */
