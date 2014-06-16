/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#include "CameraROS.h"
#include "Settings.h"
#include "sensor_msgs/image_encodings.h"
#include "opencv2/imgproc/imgproc.hpp"

CameraROS::CameraROS(QObject * parent) :
	Camera(parent)
{
	ros::NodeHandle n;
	image_transport::ImageTransport it(n);
	image_sub_ = it.subscribe("image", 1, &CameraROS::imgReceivedCallback, this);
}

CameraROS::~CameraROS()
{
}

bool CameraROS::start()
{
	this->startTimer();
	return true;
}

void CameraROS::stop()
{
	this->stopTimer();
}

void CameraROS::takeImage()
{
	ros::spinOnce();
}

void CameraROS::imgReceivedCallback(const sensor_msgs::ImageConstPtr & msg)
{
	if(msg->data.size())
	{
		cv_bridge::CvImageConstPtr ptr = cv_bridge::toCvShare(msg);
		if(msg->encoding.compare(sensor_msgs::image_encodings::BGR8) == 0)
		{
			cv::Mat cpy = ptr->image.clone();
			emit imageReceived(cpy);
		}
		else if(msg->encoding.compare(sensor_msgs::image_encodings::RGB8) == 0)
		{
			cv::Mat bgr;
			cv::cvtColor(ptr->image, bgr, cv::COLOR_RGB2BGR);
			emit imageReceived(bgr);
		}
		else
		{
			ROS_WARN("Encoding \"%s\" detected. Supported image encodings are bgr8 and rgb8...", msg->encoding.c_str());
		}
	}
}

