  #include "ros/ros.h"   //Librería de funciones del sistema ROS
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"  
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"     
  #include "std_msgs/Int16.h"
  #include "std_msgs/String.h"  
  #include "geometry_msgs/Point.h"
  #include "math.h"  //Librería de funciones matemáticas 
  
  geometry_msgs::Point punto_0,punto_1;
  
  int cont = 0;
  int cont_1 = 0;
  int cont_2 = 0;
  int inclinacion_pinza = 0;
  int start = 0;
    
  myrabot_arm_base::Servos pg, cg;
  myrabot_arm_base::WriteServos move;
  myrabot_arm_base::Servos pinza;
  myrabot_arm_base::WriteServos pin;  
  myrabot_arm_base::WriteServos error_coordenadas;
  
  myrabot_arm_base::WriteServos acercar(geometry_msgs::Point cerca)
  {
  	double x = cerca.x;
  	double z = cerca.z; 
  	 	
  	cerca.z = cerca.z - 70*cos(atan2(x,z));
  	cerca.x = cerca.x - 70*sin(atan2(x,z));
  	
  	myrabot_arm_base::WriteServos move = inversa(cerca, ::inclinacion_pinza, ::pg, 0);
  	
  	return move;

  }
  
    myrabot_arm_base::WriteServos levantar_acercar(geometry_msgs::Point la)
  {
  	 	
  	la.x = 0;
  	la.y = la.y + 50;
   	la.z = 60;
  	
  	if (la.y < 75) {
		la.y = 75;
	}
  	 	
  	myrabot_arm_base::WriteServos move = inversa(la, ::inclinacion_pinza, ::pg, 0);
  	
  	::punto_0 = la;
  	
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
	

	if (::cont == 0)
	{
		::punto_0 = home(p, c);
		
		::punto_1 = ::punto_0;
	
		::cont = ::cont+1;
	
	}
	

	//std::cout<<::punto_0<<", "<<::punto_1<<std::endl;
	
	if (::punto_0.x != ::punto_1.x || ::punto_0.y != ::punto_1.y || ::punto_0.z != ::punto_1.z ) {


		
		
  	if (::cont_1 == 0) {
  	
		::move = acercar(::punto_0); 	
  	
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
				
				::move = inversa(::punto_0, ::inclinacion_pinza, ::pg, 0);
  	
				move_pub_.publish(::move);
			
				//std::cout<<p<<", "<<::move<<std::endl;
			 
				::cont_2 = ::cont_2+1;
			
			
			}
			else {	 
			
			
			  	if (((p.base-15) < ::move.posicion.base && ::move.posicion.base < (p.base+15)) && ((p.arti1-15) < ::move.posicion.arti1 && ::move.posicion.arti1 < (p.arti1+15)) && ((p.arti2-15) < ::move.posicion.arti2 && ::move.posicion.arti2 < (p.arti2+15)) && ((p.arti3-15) < ::move.posicion.arti3 && ::move.posicion.arti3 < (p.arti3+15))) {
				
				
				::pinza.pinza = p.pinza;
				
					if(::pin.posicion.pinza != p.pinza) {
					
						std::cout<<c.pinza<<std::endl;
				
						::pinza.pinza = ::pinza.pinza + 20;
					
						::pin = control_pinza(::pinza, p, c);
					
						hand_pub_.publish(::pin);
						
					}
					else {
					
						std::cout<<"Agarrado"<<std::endl;
				
						::move = levantar_acercar(::punto_0);				
			
						move_pub_.publish(::move);
					
						::punto_1 = ::punto_0;
					
						::cont_1 = 0;
						::cont_2 = 0;
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
	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm"
	
	::punto_0 = point;
			
	/*myrabot_arm_base::mover move;
		
  	move = acercar(point);
  	
  	move_pub_.publish(move);
  	
	ros::Time ahora;
	
 		ahora = ros::Time::now();   //Asignación del tiempo actual a la variable ahora
 
 
		while (ros::Time::now() < (ahora + ros::Duration(0.5)))
 		{}	
	  	
  	
  	if (::e.base == 0 && ::e.arti1 == 0 && ::e.arti2 == 0 && ::e.arti3 == 0) {
		
		 	ahora = ros::Time::now();   //Asignación del tiempo actual a la variable ahora
 
 
			while (ros::Time::now() < (ahora + ros::Duration(1)))
			{}	
			
			move = inversa(point);
			
			move_pub_.publish(move);
			
			ahora = ros::Time::now();   //Asignación del tiempo actual a la variable ahora
 
 
			while (ros::Time::now() < (ahora + ros::Duration(0.5)))
			{}	
			
			if (::e.base == 0 && ::e.arti1 == 0 && ::e.arti2 == 0 && ::e.arti3 == 0) {
				
				std_msgs::Int16 pinza;
				
				pinza.data = ::p.pinza;
				
				while (::c.pinza < 600) {
				
					pinza.data = pinza.data + 2;
					
					hand_pub_.publish(pinza);
						
				}
			}
			
	}
	else {
			
			std::cout<<"En movimiento"<<std::endl;
	}*/

  } 
  
  void voz(const std_msgs::String& orden)   //Función de llamda con cada dato recibido
  {	
	
	std::string voz = orden.data;
	
	if (voz == "take the cola")
	{
		::punto_1.x = 0;
		::punto_1.y = 80;
		::punto_1.z = 180;		
		
	}
	else if (voz == "take the coca")
	{
		::punto_1.x = 0;
		::punto_1.y = 80;
		::punto_1.z = 180;		
		
	}

  }
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "control_recogida");   //Creación del nodo "control_brazo"
 
	ros::NodeHandle n;
	
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicion_estado_corriente);  //Suscripción del topic "pose_arm" 
  	
  	ros::Subscriber point_sub_= n.subscribe("pick_point", 1, punto);  //Suscripción del topic "point"
  	
  	ros::Subscriber voz_sub_= n.subscribe("recognizer/output", 1, voz); 
  	
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"

	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm"	


	
	ros::spin();   //Mantiene la suscripción al topic hasta que se reciba "Ctrl+C"
 
        return 0;
  }
