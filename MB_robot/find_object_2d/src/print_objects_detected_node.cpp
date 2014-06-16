/*
 * Copyright (C) 2012, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */

#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>

void objectsDetectedCallback(const std_msgs::Float32MultiArray & msg)
{
	printf("---\n");
	for(unsigned int i=0; i<msg.data.size(); i+=12)
	{
		// id, dx, dy
		printf("Object %d detected at (%f,%f)\n", (int)msg.data[i], msg.data[i+9], msg.data[i+10]);
	}

}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "objects_detected");

	ros::NodeHandle nh;
	ros::Subscriber subs;
	subs = nh.subscribe("objects", 1, objectsDetectedCallback);

	ros::spin();

	return 0;
}
