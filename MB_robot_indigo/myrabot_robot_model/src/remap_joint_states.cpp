  #include "ros/ros.h"
  #include "ros/time.h"  
  #include "sensor_msgs/JointState.h"
  
  int cont_brazo = 0;
  int cont_roomba = 0;
  int tam_js, tam_brazo;
  std::vector<std::string> name;
  std::vector<float> position;
  std::vector<float> velocity;
  std::vector<float> effort;

  void brazo(const sensor_msgs::JointState& brazo_states)
  {
	::tam_brazo = brazo_states.name.size();
	
	if (::cont_brazo == 0)
	{		
		
		for (int i = 0; i < ::tam_brazo; i++)
		{
			::name.push_back(brazo_states.name[i]);
			::position.push_back(brazo_states.position[i]);
			::velocity.push_back(brazo_states.velocity[i]);
			::effort.push_back(brazo_states.effort[i]);
		}
		
		::cont_brazo = 1;
	}		  	  	  
  }
  
  void roomba(const sensor_msgs::JointState& roomba_states)
  {
	::tam_js = roomba_states.name.size();
	
	if (::cont_roomba == 0)
	{
		for (int i = 0; i < ::tam_js; i++)
		{
			::name.push_back(roomba_states.name[i]);
			::position.push_back(roomba_states.position[i]);
			if (roomba_states.velocity.size() == ::tam_js)
			::velocity.push_back(roomba_states.velocity[i]);
			if (roomba_states.effort.size() == ::tam_js)
			::effort.push_back(roomba_states.effort[i]);
		}
		
		::cont_roomba = 1;
	}	  	  	  
  }    
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "remap_joint_states");   
 
	ros::NodeHandle n;
  	
  	ros::Subscriber joint_states_brazo_sub_= n.subscribe("brazo/joint_states", 1, brazo);
  	ros::Subscriber joint_states_roomba_sub_= n.subscribe("joint_states", 1, roomba);   	 
  	
  	ros::Publisher joint_states_pub_=n.advertise<sensor_msgs::JointState>("joint_states_myrabot", 1);  	 
  	
    ros::Rate loop_rate(100);
    
	ros::Time ahora;

	sensor_msgs::JointState joint_states_todo;		  	      	

    while (ros::ok())
    {	
		
		if (::cont_brazo == 1 && ::cont_roomba == 1)
		{
			int tam = ::tam_brazo+::tam_js;

			joint_states_todo.name = std::vector<std::string>(tam);
			joint_states_todo.position = std::vector<double>(tam);
			joint_states_todo.velocity = std::vector<double>(tam);
			joint_states_todo.effort = std::vector<double>(tam);
			
			for (int i = 0; i < tam; i++)
			{
				joint_states_todo.name[i] = ::name[i];
				joint_states_todo.position[i] = ::position[i];
				joint_states_todo.velocity[i] = ::velocity[i];
				joint_states_todo.effort[i] = ::effort[i];
			}
			
			ahora = ros::Time::now();
			
			joint_states_todo.header.stamp = ahora; 			
			
			joint_states_pub_.publish(joint_states_todo);
			
			::cont_brazo = 0;
			::cont_roomba = 0;
		}
		
		ros::spinOnce();
	
		loop_rate.sleep();			
	}   
   	   
 
	ros::spin();  
 
        return 0;
  }
