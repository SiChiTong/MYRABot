  #include "ros/ros.h"
  #include "ros/time.h"  
  #include "sensor_msgs/JointState.h"
  
  sensor_msgs::JointState joint_states_todo;
  int cont_brazo = 0;
  int cont_roomba = 0;
  std::string name[9];
  float position[9];
  float velocity[9];
  float effort[9];

  void brazo(const sensor_msgs::JointState& brazo_states)
  {
	
	int tam = brazo_states.name.size();
	
	if (::cont_brazo == 0)
	{		
		
		for (int i = 0; i < tam; i++)
		{
			::name[i] = brazo_states.name[i];

			::position[i] = brazo_states.position[i];
			::velocity[i] = brazo_states.velocity[i];
			::effort[i] = brazo_states.effort[i];
		}
		
		::cont_brazo = 1;
	}		  	  	  
  }
  
  void roomba(const sensor_msgs::JointState& roomba_states)
  {
	int tam = roomba_states.name.size();
	
	if (::cont_roomba == 0)
	{
		for (int i = 0; i < tam; i++)
		{
			::name[i+5] = roomba_states.name[i];
			::position[i+5] = roomba_states.position[i];
			::velocity[i+5] = roomba_states.velocity[i];
			::effort[i+5] = roomba_states.effort[i];
		}
		
		::cont_roomba = 1;
	}	  	  	  
  }    
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "remap_joint_states");   
 
	ros::NodeHandle n;
  	
  	ros::Subscriber joint_states_brazo_sub_= n.subscribe("brazo/joint_states", 1, brazo);
  	ros::Subscriber joint_states_roomba_sub_= n.subscribe("joint_states_roomba", 1, roomba);   	 
  	
  	ros::Publisher joint_states_pub_=n.advertise<sensor_msgs::JointState>("joint_states", 1);  	 
  	
    ros::Rate loop_rate(100);
    
	ros::Time ahora;
    
	::joint_states_todo.name = std::vector<std::string>(9);
	::joint_states_todo.position = std::vector<double>(9);
	::joint_states_todo.velocity = std::vector<double>(9);
	::joint_states_todo.effort = std::vector<double>(9);		  	      	

    while (ros::ok())
    {	
		
		if (::cont_brazo == 1 && ::cont_roomba == 1)
		{
			
			for (int i = 0; i < 9; i++)
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
			::cont_roomba = 0;
		}
		
			ros::spinOnce();
	
			loop_rate.sleep();			
	}   
   	   
 
	ros::spin();  
 
        return 0;
  }
