  #include "ros/ros.h"
  #include "myrabot_arm_base_b/Servos.h"
  #include "myrabot_arm_base_b/WriteServos.h"
  #include "myrabot_arm_base_b/ReadServos.h"
  #include "myrabot_arm_base_b/myrabot_arm_base_b.h"
  #include "std_msgs/Int16.h"
  #include "std_msgs/String.h"
  #include "geometry_msgs/Point.h"
  #include "math.h"
  
  
  int inclinacion_pinza = 0;
  myrabot_arm_base_b::Servos p;
  myrabot_arm_base_b::Servos c;

  geometry_msgs::Point punto;
  
  std::string elemento_final = "pinza";
  
  
  void posicionEstadoCorriente(const myrabot_arm_base_b::ReadServos& pec)   
  { 

	ros::NodeHandle n;
	  
	ros::Publisher point_arm_=n.advertise<geometry_msgs::Point>("point_arm", 1); 
	   
	p = pec.posicion;
	
	geometry_msgs::Point punto = directa(p, 0, ::elemento_final);
	
	point_arm_.publish(punto);
	
  }
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "control_recogida");  
 
	ros::NodeHandle n;
	
	ros::param::get("~Elemento_Final",::elemento_final);
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicionEstadoCorriente);   

	ros::Publisher point_arm_=n.advertise<geometry_msgs::Point>("point_arm", 1);   	

	
	ros::spin();  
 
        return 0;
  }
