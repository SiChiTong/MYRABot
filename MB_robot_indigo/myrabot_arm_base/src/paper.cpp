  #include "ros/ros.h"  
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"  
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"     
  #include "std_msgs/Int16.h"
  #include "std_msgs/String.h"  
  #include "geometry_msgs/Point.h"
  #include "math.h"  
  
  
  int inclinacion_pinza = 0;
  myrabot_arm_base::Servos p;
  myrabot_arm_base::Servos c;
  myrabot_arm_base::WriteServos move;
  int start=0, cont=0, cont_1=0, cont_2=0, cont_3=0, cont_4=0;
  
  geometry_msgs::Point punto;
  
  void posicionEstadoCorriente(const myrabot_arm_base::ReadServos& pec)   
  { 

	ros::NodeHandle n;
	  
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1); 
	   
	p = pec.posicion;
	
	c = pec.corriente;
	
	if (start == 0 || (((p.base-15) < ::move.posicion.base && ::move.posicion.base < (p.base+15)) 
  	    && ((p.arti1-15) < ::move.posicion.arti1 && ::move.posicion.arti1 < (p.arti1+15)) 
  	    && ((p.arti2-15) < ::move.posicion.arti2 && ::move.posicion.arti2 < (p.arti2+15)) 
  	    && ((p.arti3-15) < ::move.posicion.arti3 && ::move.posicion.arti3 < (p.arti3+15)))) {
		
		if (::cont == 0)
		{
			::punto = home(p, c);
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			::cont = 1;
			::start = 1;
			
			sleep(2);
		}
		
		else if (::cont_1 == 0)
		{
			::punto.x = 0;
			::punto.z = 0;
			::punto.y = 265;
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			move_pub_.publish(::move);
			
			::cont_1 = 1;
		}
		else if (::cont_2 == 0)
		{
			::punto.x = 260;
			::punto.z = 0;
			::punto.y = 50;
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			move_pub_.publish(::move);
			
			::cont_2 = 1;
			
			sleep(2);
		}
		else if (::cont_3 == 0)
		{
			::punto.x = 0;
			::punto.z = 205;
			::punto.y = 60;
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			move_pub_.publish(::move);
			
			::cont_3 = 1;
			
			sleep(2);
		}
		else if (::cont_4 == 0)
		{
			::punto.x = -260;
			::punto.z = 0;
			::punto.y = 50;
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			move_pub_.publish(::move);
			
			::cont_4 = 1;
			
			sleep(2);
		}
		else
		{
			::punto.x = 0;
			::punto.z = 0;
			::punto.y = 265;
			
			::move = inversa(::punto, ::inclinacion_pinza, p, 0);
			
			move_pub_.publish(::move);

			::cont = 0;
			::cont_1 = 0;
			::cont_2 = 0;
			::cont_3 = 0;
			::cont_4 = 0;
			::start = 0;
			
			sleep(2);
		}
	}
	
  }
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "control_recogida");  
 
	ros::NodeHandle n;
	
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicionEstadoCorriente);  

  	
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);  

	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);   	

	
	ros::spin();  
 
        return 0;
  }
