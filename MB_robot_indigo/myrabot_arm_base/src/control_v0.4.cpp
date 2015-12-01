  #include "ros/ros.h"
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"  
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"   
  #include "geometry_msgs/Point.h"
  #include "math.h"
 
  #define PI 3.14159265
  #define L1 104
  #define L2 104
  #define Lp 60
 
 
  myrabot_arm_base::Servos  p, e, c;
  myrabot_arm_base::WriteServos destino, mover;  
  geometry_msgs::Point punto_0, punto_1, punto_i;
  double lambda = 0;
  int inclinacion_pinza = 0;
  int cont = 0; 
 
  void posicion(const myrabot_arm_base::ReadServos& pose)
  {
	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);  
 
	::p = pose.posicion;
    ::e = pose.estado;
    ::c = pose.corriente;
    
    if (cont == 0)
    { 
		
		::punto_0 = home(::p, ::c);
		
		::mover = inversa(::punto_0, ::inclinacion_pinza, ::p, 0);
		
		cont = 1;
	}
		
	//std::cout<<::punto_0<<" - "<<::punto_1<<std::endl;
	//std::cout<<::p<<" - "<<::mover<<std::endl;
	
    
	if ((::punto_0.x != ::punto_1.x || ::punto_0.y != ::punto_1.y || ::punto_0.z != ::punto_1.z) && ((::p.base-15) < ::mover.posicion.base && ::mover.posicion.base < (::p.base+15)) && ((::p.arti1-15) < ::mover.posicion.arti1 && ::mover.posicion.arti1 < (::p.arti1+15)) && ((::p.arti2-15) < ::mover.posicion.arti2 && ::mover.posicion.arti2 < (::p.arti2+15)) && ((::p.arti3-15) < ::mover.posicion.arti3 && ::mover.posicion.arti3 < (::p.arti3+15)))
	{	
    
		if (((::p.base-15) < ::destino.posicion.base && ::destino.posicion.base < (::p.base+15)) && ((::p.arti1-15) < ::destino.posicion.arti1 && ::destino.posicion.arti1 < (::p.arti1+15)) && ((::p.arti2-15) < ::destino.posicion.arti2 && ::destino.posicion.arti2 < (::p.arti2+15)) && ((::p.arti3-15) < ::destino.posicion.arti3 && ::destino.posicion.arti3 < (::p.arti3+15))) 
		{
			::punto_0 = ::punto_1;
			::lambda = 0;
		}
		else
		{    
			geometry_msgs::Point u;
			
			//ecuación de la recta p1 = p0+lambda.u
			
			u.x = ::punto_1.x - ::punto_0.x;
			u.y = ::punto_1.y - ::punto_0.y;
			u.z = ::punto_1.z - ::punto_0.z;        
			
			double paso = abs(sqrt(pow(u.x,2)+pow(u.y,2)+pow(u.z,2))/10);
			
			//std::cout<<paso<<std::endl;
			   
			::lambda = ::lambda + 1/paso;
			
			::punto_i.x = ::punto_0.x + (::lambda)*u.x;
			::punto_i.y = ::punto_0.y + (::lambda)*u.y;
			::punto_i.z = ::punto_0.z + (::lambda)*u.z;
				
			//std::cout<<punto_0<<std::endl;
			//std::cout<<punto_i<<" : "<<lambda<<std::endl;
			
			::mover = inversa(::punto_i, ::inclinacion_pinza, ::p, 0);
			
			move_pub_.publish(::mover);
			
			//std::cout<<::p<<std::endl;

			//std::cout<<::destino<<std::endl;  	
		}

	}
 
  }
 
  void punto(const geometry_msgs::Point& point)
  {	
	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1); 
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);
  	
	::lambda = 0;    
    
    ::punto_0 = directa(::p, ::inclinacion_pinza);  	
	
	::punto_1 = point;
	
	::destino = inversa(::punto_1, ::inclinacion_pinza, ::p, 0);
 
  } 
 
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "control_brazo");   
 
	ros::NodeHandle n;
 
 
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicion); 
 
  	ros::Subscriber point_sub_= n.subscribe("point", 1, punto); 
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1); 
  	   
 
	ros::spin();  
 
    return 0;
  }
