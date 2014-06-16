/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#ifndef OBJECTSDETECTEDCALLBACK_H_
#define OBJECTSDETECTEDCALLBACK_H_

#include <QMap>
#include <QObject>
#include <QTransform>
#include <QRect>
#include <QPair>
#include <std_msgs/Float32MultiArray.h>
#include <ros/ros.h>

class ObjectsDetectedCallback : public QObject
{
	Q_OBJECT
public:
	ObjectsDetectedCallback(QObject * parent = 0) : QObject(parent)
	{
		ros::NodeHandle nh;
		pub_ = nh.advertise<std_msgs::Float32MultiArray>("objects", 1);
	}
	virtual ~ObjectsDetectedCallback() {}

public slots:
	void publish(const QMap<int, QPair<QRect, QTransform> > & objectsDetected)
	{
		if(objectsDetected.size())
		{
			std_msgs::Float32MultiArray msg;
			msg.data = std::vector<float>(objectsDetected.size()*12);
			int i=0;
			for(QMap<int, QPair<QRect, QTransform> >::const_iterator iter=objectsDetected.constBegin(); iter!=objectsDetected.constEnd(); ++iter)
			{
				msg.data[i++] = iter.key();
				msg.data[i++] = iter->first.width();
				msg.data[i++] = iter->first.height();
				msg.data[i++] = iter->second.m11();
				msg.data[i++] = iter->second.m12();
				msg.data[i++] = iter->second.m13();
				msg.data[i++] = iter->second.m21();
				msg.data[i++] = iter->second.m22();
				msg.data[i++] = iter->second.m23();
				msg.data[i++] = iter->second.m31(); // dx
				msg.data[i++] = iter->second.m32(); // dy
				msg.data[i++] = iter->second.m33();
			}
			pub_.publish(msg);
		}
	}
private:
	ros::Publisher pub_;
};


#endif /* OBJECTSDETECTEDCALLBACK_H_ */
