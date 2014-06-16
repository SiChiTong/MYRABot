  #include "ros/ros.h"   //Librería de funciones del sistema ROS
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"  
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"     
  #include "std_msgs/Int16.h"
  #include "std_msgs/String.h"   
  #include "geometry_msgs/Point.h"
  #include "math.h"  //Librería de funciones matemáticas
  
  geometry_msgs::Point punto_0, punto_1;
  
  int cont = 0;
  int cont_1 = 0;
  int cont_2 = 0;
  int cont_3 = 0;
  int inclinacion_pinza = 0;
    
  myrabot_arm_base::Servos pg, cg, pinza;
  myrabot_arm_base::WriteServos move;
  myrabot_arm_base::WriteServos pin;
  myrabot_arm_base::WriteServos error_coordenadas;  
  
  myrabot_arm_base::WriteServos separar(geometry_msgs::Point lejos)
  {
  	double x = lejos.x;
  	double z = lejos.z; 
  	 	
  	lejos.z = lejos.z - 70*cos(atan2(x,z));
  	lejos.x = lejos.x - 70*sin(atan2(x,z));
  	 	
  	myrabot_arm_base::WriteServos move = inversa(lejos, ::inclinacion_pinza, ::pg, 0);
  	
  	return move;

  }
  
  void posicion_estado_corriente(const myrabot_arm_base::ReadServos& pec)   //Función de llamda con cada dato recibido
  { 
	  
	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm"  
  	   
	myrabot_arm_base::Servos p = pec.posicion;   //Posición del servomotor

	::pg = pec.posicion;
	::cg = pec.corriente; 	
	
	myrabot_arm_base::Servos e = pec.estado;   //Estado del servomotor
	
	myrabot_arm_base::Servos c = pec.corriente;   //Posición del servomotor
	
	//std::cout<<::punto_0<<", "<<::punto_1<<std::endl;
	
	if (::cont == 0)
	{
		::punto_0 = directa(p, inclinacion_pinza);
		
		::punto_1 = ::punto_0;
	
		::cont = ::cont+1;
	}
	
	if (::punto_0.x != ::punto_1.x || ::punto_0.y != ::punto_1.y || ::punto_0.z != ::punto_1.z ) {



		
		
  	if (::cont_1 == 0) {
  	
		::move = inversa(::punto_0, ::inclinacion_pinza, ::pg, 0); 	
  	
		move_pub_.publish(::move);
  	
		::cont_1 = ::cont_1+1;
  	
	}
  	
  	
	//ros::Time ahora;
 
	//std::cout<<p<<", "<<::move<<std::endl;  	
  	
  	if (((p.base-15) < ::move.posicion.base && ::move.posicion.base < (p.base+15)) && ((p.arti1-15) < ::move.posicion.arti1 && ::move.posicion.arti1 < (p.arti1+15)) && ((p.arti2-15) < ::move.posicion.arti2 && ::move.posicion.arti2 < (p.arti2+15)) && ((p.arti3-15) < ::move.posicion.arti3 && ::move.posicion.arti3 < (p.arti3+15))) {
		
		 	//ahora = ros::Time::now();   //Asignación del tiempo actual a la variable ahora
 
 
			//while (ros::Time::now() < (ahora + ros::Duration(1)))
			//{}	
			
			if (::cont_2 == 0) {
				
				::pinza.pinza = 470;
				
				::pin = control_pinza(::pinza, p, c);
					
				hand_pub_.publish(::pin);
				
				::cont_2 = ::cont_2+1;
			
			}
			else {	 
			
			
			  	if (p.pinza <= 495) {
				
					if (::cont_3 == 0) {
					
						::move = separar(::punto_0);
		
						move_pub_.publish(::move);
						
						::cont_3 = ::cont_3+1;		
										
					}	
					
					else if (((p.base-15) < ::move.posicion.base && ::move.posicion.base < (p.base+15)) && ((p.arti1-15) < ::move.posicion.arti1 && ::move.posicion.arti1 < (p.arti1+15)) && ((p.arti2-15) < ::move.posicion.arti2 && ::move.posicion.arti2 < (p.arti2+15)) && ((p.arti3-15) < ::move.posicion.arti3 && ::move.posicion.arti3 < (p.arti3+15))) {
					
						std::cout<<"Suelto"<<std::endl;
						
						::cont = 0;
						
						::cont_1 = 0;
						::cont_2 = 0;
						::cont_3 = 0;
					}
				}
			}
			
	}
	else {
			
			std::cout<<"En movimiento"<<std::endl;
	}
	}
	
		
	  
  }
 
  void punto(const geometry_msgs::Point& point)   //Función de llamda con cada dato recibido
  {	
	
	::punto_0 = point;		

  } 
  
  void voz(const std_msgs::String& orden)   //Función de llamda con cada dato recibido
  {	
	
	std::string voz = orden.data;
	
	if (voz == "leave the cola")
	{
		::punto_1.x = 0;
		::punto_1.y = 80;
		::punto_1.z = 180;		
		
	}
	else if (voz == "leave the coca")
	{
		::punto_1.x = 0;
		::punto_1.y = 80;
		::punto_1.z = 180;		
		
	}

  }  
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "control_entrega");   //Creación del nodo "control_brazo"
 
	ros::NodeHandle n;
	
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicion_estado_corriente);  //Suscripción del topic "pose_arm" 
  	
  	ros::Subscriber point_sub_= n.subscribe("place_point", 1, punto);  //Suscripción del topic "point"
  
  	ros::Subscriber voz_sub_= n.subscribe("recognizer/output", 1, voz);   	
  	
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"

	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm"	


	
	ros::spin();   //Mantiene la suscripción al topic hasta que se reciba "Ctrl+C"
 
        return 0;
  }
