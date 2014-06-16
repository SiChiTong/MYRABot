
  #include "ros/ros.h"
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"  
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"  
  #include "geometry_msgs/Point.h"
  #include "std_msgs/Int16.h"
  #include "std_msgs/String.h"  
  #include <signal.h>
  #include <termios.h>
  #include <stdio.h> 
  
  #define KEYCODE_Xmas 0x43 
  #define KEYCODE_Xmenos 0x44
  #define KEYCODE_Ymas 0x41
  #define KEYCODE_Ymenos 0x42
  #define KEYCODE_Zmas 0x77
  #define KEYCODE_Zmenos 0x73
  #define KEYCODE_Pabrir 0x61
  #define KEYCODE_Pcerrar 0x64
  #define KEYCODE_PinclinarMas 0x65
  #define KEYCODE_PinclinarMenos 0x71  
  
struct termios cooked, raw; 

int cont = 0;

double retardo = 0.11;

  geometry_msgs::Point punto;
  myrabot_arm_base::Servos vel;
  myrabot_arm_base::Servos pinza, p, c;
  
  int pinza_incli = 0;
  
	
void posicion_estado_corriente(const myrabot_arm_base::ReadServos& pec) 
  { 
	     
	::p = pec.posicion;  
	::c = pec.corriente;

  }	  
 
void quit(int sig)
{
  tcsetattr(0, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
} 

void callback(const ros::TimerEvent&)
{
 
	ros::NodeHandle n;   	
  	
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1); 
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);  	  	

	ros::Time anterior;
	
	myrabot_arm_base::WriteServos teleop;
	myrabot_arm_base::WriteServos teleop_pinza;	

	if (::cont == 0)
    {
		anterior = ros::Time::now();
		
		::punto = home(::p, ::c);
		
		::cont = 1;
		
		::pinza.pinza = 511;
		
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		teleop_pinza = control_pinza(::pinza, ::p, ::c);
		
		//std::cout<<teleop<<"-"<<teleop_pinza<<std::endl;
	}
	
  char c;

  // get the console in raw mode                                                              
  tcgetattr(0, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &=~ (ICANON | ECHO);
  // Setting a new line, then end of file                         
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(0, TCSANOW, &raw);

  puts(""); 
  puts("#####################################################");
  puts("                   CONTROLES BRAZO");
  puts("#####################################################");
  puts("");  
  puts("Flecha arriba:________ Subir pinza");
  puts("Flecha abajo:_________ Bajar pinza");
  puts("Flecha izquierda:_____ Desplazar pinza a la izquierda"); 
  puts("Flecha derecha:_______ Desplazar pinza a la derecha");
  puts("Tecla W:______________ Desplazar pinza hacia delante");
  puts("Tecla S:______________ Desplazar pinza hacia atrás");
  puts("Tecla A:______________ Abrir pinza");
  puts("Tecla D:______________ Cerrar pinza");
  puts("Tecla Q:______________ Inclinar pinza hacia arriba");
  puts("Tecla E:______________ Inclinar pinza hacia abajo");                 

        
    while (ros::ok())
    {
				
    // get the next event from the keyboard  
    if(read(0, &c, 1) < 0)
    {
      perror("read():");
      exit(-1);
    }
	
      if (c == KEYCODE_Xmas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
		::punto.x = ::punto.x - 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::punto.x = ::punto.x + 5;}
		anterior = ros::Time::now();
		}			
      }
      if (c == KEYCODE_Xmenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::punto.x = ::punto.x + 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::punto.x = ::punto.x - 5;}
		anterior = ros::Time::now();
		}				
      }
      if (c == KEYCODE_Ymas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::punto.y = ::punto.y + 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::punto.y = ::punto.y - 5;}
		anterior = ros::Time::now();
		}					    
      }
      if (c == KEYCODE_Ymenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::punto.y = ::punto.y - 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::punto.y = ::punto.y + 5;}		
		anterior = ros::Time::now();
		}						
      }
      if (c == KEYCODE_Zmas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::punto.z = ::punto.z + 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3)  
		{::punto.z = ::punto.z - 5;}		
		anterior = ros::Time::now();
		}					
      }
      if (c == KEYCODE_Zmenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::punto.z = ::punto.z - 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::punto.z = ::punto.z + 5;}	
		anterior = ros::Time::now();
		}				
      }
      if (c == KEYCODE_Pabrir)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::pinza.pinza = ::pinza.pinza - 5;
		teleop_pinza = control_pinza(::pinza, ::p, ::c);
		if (teleop_pinza.posicion.pinza == ::p.pinza) {::pinza.pinza = ::pinza.pinza + 5;}		
		anterior = ros::Time::now();
		}
      }
      if (c == KEYCODE_Pcerrar)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::pinza.pinza = ::pinza.pinza + 5;
		teleop_pinza = control_pinza(::pinza, ::p, ::c);
		if (teleop_pinza.posicion.pinza == ::p.pinza) {::pinza.pinza = ::pinza.pinza - 5;}
		anterior = ros::Time::now();
		}
      }
      if (c == KEYCODE_PinclinarMas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::pinza_incli = ::pinza_incli + 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::pinza_incli = ::pinza_incli - 5;}
		anterior = ros::Time::now();
		}
      }
      if (c == KEYCODE_PinclinarMenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{      
		::pinza_incli = ::pinza_incli - 5;
		teleop = inversa(::punto, ::pinza_incli, ::p, 0);
		if (teleop.posicion.base == ::p.base && teleop.posicion.arti1 == ::p.arti1 && teleop.posicion.arti2 == ::p.arti2 && teleop.posicion.arti3 == ::p.arti3) 
		{::pinza_incli = ::pinza_incli + 5;}
		anterior = ros::Time::now();
		}
      }                                     
		
		if (teleop.posicion.base != ::p.base || teleop.posicion.arti1 != ::p.arti1 || teleop.posicion.arti2 != ::p.arti2 || teleop.posicion.arti3 != ::p.arti3)
		{
			move_pub_.publish(teleop);			
		}
		if (teleop_pinza.posicion.pinza != ::p.pinza)
		{				
			hand_pub_.publish(teleop_pinza);
		}						    	

	}	
}
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "teleoperador_teclado");   
 
	ros::NodeHandle n;
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicion_estado_corriente);  	
  	
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);   
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);    
		 	 

	signal(SIGINT,quit); 
	
	ros::Timer timer = n.createTimer(ros::Duration(1), callback);  
   	   
 
	ros::spin();  
 
        return 0;
  }
