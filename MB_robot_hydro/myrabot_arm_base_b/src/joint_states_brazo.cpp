  #include "ros/ros.h"
  #include "ros/time.h"  
  #include "sensor_msgs/JointState.h"
  #include "myrabot_arm_base_b/ReadServos.h"
  #include "std_msgs/String.h"   
  
  #define PI 3.14159265  
  
  sensor_msgs::JointState joint_states_todo;
  int cont = 0;
  int cont_brazo = 0;
  std::string name[5];
  float position[5];
  float position_pre[5] = {0,0,0,0,0};
  float velocity[5];
  float effort[5];
  double antes = 0;

  void brazo(const myrabot_arm_base_b::ReadServos& brazo)
  {
	::cont = 1;  
	  
	double ahora = ros::Time::now().toSec();
	
	if (::cont_brazo == 0)
	{
		::name[0] = "base_brazo";
		::name[1] = "arti1";
		::name[2] = "arti2";
		::name[3] = "arti3";
		::name[4] = "pinza";								
		
		::position[0] = ((((brazo.posicion.base * 300)/1023)*PI)/180)-2.62;
		::position[1] = ((((brazo.posicion.arti1 * 300)/1023)*PI)/180)-2.62;
		::position[2] = ((((brazo.posicion.arti2 * 300)/1023)*PI)/180)-2.62;
		::position[3] = ((((brazo.posicion.arti3 * 300)/1023)*PI)/180)-2.62;
		::position[4] = ((((brazo.posicion.pinza * 300)/1023)*PI)/180)-2.62;
		
		::velocity[0] = abs(::position[0] - ::position_pre[0])/(ahora - ::antes);
		::velocity[1] = abs(::position[1] - ::position_pre[1])/(ahora - ::antes);
		::velocity[2] = abs(::position[2] - ::position_pre[2])/(ahora - ::antes);
		::velocity[3] = abs(::position[3] - ::position_pre[3])/(ahora - ::antes);
		::velocity[4] = abs(::position[4] - ::position_pre[4])/(ahora - ::antes);		
								
		::position_pre[0] = ::position[0];
		::position_pre[1] = ::position[1];
		::position_pre[2] = ::position[2];
		::position_pre[3] = ::position[3];
		::position_pre[4] = ::position[4];
		
		::effort[0] = brazo.corriente.base/1000;
		::effort[1] = brazo.corriente.arti1/1000;
		::effort[2] = brazo.corriente.arti2/1000;
		::effort[3] = brazo.corriente.arti3/1000;
		::effort[4] = brazo.corriente.pinza/1000;																		
		
		::antes = ahora;
						
		::cont_brazo = 1;
	}		  	  	  
  }   
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "joint_states_brazo");   
 
	ros::NodeHandle n;
  	
  	ros::Subscriber pose_arm_sub_= n.subscribe("pose_arm", 1, brazo);  	 
  	
  	ros::Publisher joint_states_pub_=n.advertise<sensor_msgs::JointState>("joint_states", 1);  
  	
	ros::Publisher arduino_pub_=n.advertise<std_msgs::String>("arduino_control", 1);   		 
  	
    ros::Rate loop_rate(100);
    
	ros::Time ahora;
    
    int cont = 0;
    
	::joint_states_todo.name = std::vector<std::string>(5);
	::joint_states_todo.position = std::vector<double>(5);
	::joint_states_todo.velocity = std::vector<double>(5);
	::joint_states_todo.effort = std::vector<double>(5);		  	      	

    while (ros::ok())
    {	

		if (::cont == 0)
		{
			std_msgs::String brazo;
		
			brazo.data = "brazo";			
		
			arduino_pub_.publish(brazo);				
			
		}
		
		if (::cont_brazo == 1)
		{
			//std::cout<<::joint_states_todo<<std::endl;
			
			for (int i = 0; i < 5; i++)
			{
				::joint_states_todo.name[i] = ::name[i];
				::joint_states_todo.position[i] = ::position[i];
				::joint_states_todo.velocity[i] = ::velocity[i];
				::joint_states_todo.effort[i] = ::effort[i];
			}
			
			ahora = ros::Time::now();
			
			::joint_states_todo.header.stamp = ahora; 			
			
			joint_states_pub_.publish(::joint_states_todo);
			
			::cont_brazo = 0;
		}
		
			ros::spinOnce();
	
			loop_rate.sleep();			
	}   
   	   
 
	ros::spin();  
 
        return 0;
  }
