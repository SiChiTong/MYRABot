#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <opencv2/opencv.hpp>
#include <QTransform>
#include <geometry_msgs/Point.h>
#include <std_msgs/Int16.h>
#include <find_object_2d/PointObjects.h>
#include <find_object_2d/Point_id.h>

#define dZ0 590
#define alfa 40
#define h 310
#define d 50
#define PI 3.14159265


void objectsDetectedCallback(const std_msgs::Float32MultiArray& msg)
{
    ros::NodeHandle nh;
    ros::Publisher position_pub_=nh.advertise<find_object_2d::PointObjects>("point", 1);
   
    find_object_2d::PointObjects p_objects;
    find_object_2d::Point_id objeto;
           
    p_objects.objeto = std::vector<find_object_2d::Point_id>(msg.data.size()/12);
           
    for(unsigned int i=0; i<msg.data.size(); i+=12)
    {
        // get data
        int id = (int)msg.data[i];
        float objectWidth = msg.data[i+1];
        float objectHeight = msg.data[i+2];

        // Find corners Qt
        QTransform qtHomography(msg.data[i+3], msg.data[i+4], msg.data[i+5],
                                msg.data[i+6], msg.data[i+7], msg.data[i+8],
                                msg.data[i+9], msg.data[i+10], msg.data[i+11]);

        QPointF qtTopLeft = qtHomography.map(QPointF(0,0));
        QPointF qtTopRight = qtHomography.map(QPointF(objectWidth,0));
        QPointF qtBottomLeft = qtHomography.map(QPointF(0,objectHeight));
        QPointF qtBottomRight = qtHomography.map(QPointF(objectWidth,objectHeight));
       
        geometry_msgs::Point punto;
		
		float widthTop = sqrt(pow(qtTopRight.x() - qtTopLeft.x(),2) + pow(qtTopRight.y() - qtTopLeft.y(),2));
		float widthBottom = sqrt(pow(qtBottomRight.x() - qtBottomLeft.x(),2) + pow(qtBottomRight.y() - qtBottomLeft.y(),2));
		float heightLeft = sqrt(pow(qtBottomLeft.x() - qtTopLeft.x(),2) + pow(qtBottomLeft.y() - qtTopLeft.y(),2));
		float heightRight = sqrt(pow(qtBottomRight.x() - qtTopRight.x(),2) + pow(qtBottomRight.y() - qtTopRight.y(),2));
        
        float dArea_0 = (objectHeight*objectWidth) - (((widthTop + widthBottom)/2) * ((heightLeft + heightRight)/2));

        float dZ_0 = dZ0 + (dArea_0/10);
        
        float dY_0 = (((480/2) - (((qtTopLeft.y() + qtTopRight.y())/2) + ((heightLeft + heightRight)/4)))*dZ_0)/585;                
        
        float beta_0 = atan2(dY_0,dZ_0);        
        
        objectHeight = objectHeight/cos((alfa*PI)/180);
        
        float height = ((heightLeft + heightRight)/2)/cos(((alfa*PI)/180)-beta_0);                    
        
        float dArea = (objectHeight*objectWidth) - (((widthTop + widthBottom)/2) * height);

        float dZ = dZ0 + (dArea/38);
       
        float dX = (((640/2) - (((qtTopLeft.x() + qtBottomLeft.x())/2) + ((widthTop + widthBottom)/4)))*dZ)/585;
       
        float dY = (((480/2) - (((qtTopLeft.y() + qtTopRight.y())/2) + ((heightLeft + heightRight)/4)))*dZ)/585;
       
        float beta = atan2(dY,dZ);
       
        punto.x = dX;
        punto.y = h-((dZ/cos(beta))*sin(((alfa*PI)/180)-beta));
        punto.z = ((dZ/cos(beta))*cos(((alfa*PI)/180)-beta))-d;
        
        //Validate detection
        int paralelepipedo;
        
        if (abs(widthTop - widthBottom) < 15 && abs(heightLeft - heightRight) < 10)
        {
			paralelepipedo = 1;
		}
		else
		{
			paralelepipedo = 0;
		}
        
        if (paralelepipedo == 1)
        {       
			objeto.punto = punto;
			objeto.id = id;
       
			p_objects.objeto[i/12] = objeto;
		}

    }
   
    position_pub_.publish(p_objects);

}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "objects_detected");

    ros::NodeHandle nh;
    ros::Subscriber subs = nh.subscribe("objects", 1, objectsDetectedCallback);

    ros::Publisher position_pub_=nh.advertise<find_object_2d::PointObjects>("point", 1);  

    ros::spin();

    return 0;
}
