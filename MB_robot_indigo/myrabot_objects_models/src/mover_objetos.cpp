
  #include "ros/ros.h"
  #include "gazebo/ModelState.h"
  #include <signal.h>
  #include <termios.h>
  #include <stdio.h> 
 
  #define PI 3.14159265
  #define L1 104
  #define L2 104
  #define Lp 60
  
  #define KEYCODE_Xmas 0x43 
  #define KEYCODE_Xmenos 0x44
  #define KEYCODE_Ymas 0x41
  #define KEYCODE_Ymenos 0x42
  #define KEYCODE_Zmas 0x77
  #define KEYCODE_Zmenos 0x73
  #define KEYCODE_XgiroMas 0x61
  #define KEYCODE_XgiroMenos 0x64
  #define KEYCODE_YgiroMas 0x65
  #define KEYCODE_YgiroMenos 0x71  
  
struct termios cooked, raw; 

int cont = 0;

double retardo = 0.1;

gazebo::ModelState objeto;  
 
void quit(int sig)
{
  tcsetattr(0, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}

void callback(const ros::TimerEvent&)
{
 
	ros::NodeHandle n;   	
  	
  	ros::Publisher object_pub_=n.advertise<gazebo::ModelState>("gazebo/set_model_state", 1);
  	
	std::string modelo_objeto;

	ros::NodeHandle nh("~");

	nh.param("modelo_objeto", modelo_objeto, std::string("lata_amstel"));

	ros::Time anterior;

	if (::cont == 0)
    {
		anterior = ros::Time::now();
		
		::objeto.model_name = modelo_objeto;
  
		::objeto.reference_frame = "world";
		
		::objeto.pose.position.x = 0;
		
		::objeto.pose.position.y = 0.5;
		
		::objeto.pose.position.z = 0.74;
		
		::objeto.pose.orientation.x = 0;
		
		::objeto.pose.orientation.y = 0;
		
		::objeto.pose.orientation.z = 0;
		
		::objeto.pose.orientation.w = 1;												 
		
		::cont = 1;

		object_pub_.publish(::objeto);		
		
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
  puts("                   CONTROLES OBJETO");
  puts("#####################################################");
  puts("");  
  puts("Flecha arriba:_______ Subir objeto");
  puts("Flecha abajo:________ Bajar objeto");
  puts("Flecha izquierda:____ Desplazar objeto a la izquierda"); 
  puts("Flecha derecha:______ Desplazar objeto a la derecha");
  puts("Tecla W:_____________ Desplazar objeto hacia delante");
  puts("Tecla S:_____________ Desplazar objeto hacia atrás");
  puts("Tecla A:_____________ Inclinar objeto en eje x mas");
  puts("Tecla D:_____________ Inclinar objeto en eje x menos");
  puts("Tecla Q:_____________ Inclinar objeto en eje y mas");
  puts("Tecla E:_____________ Inclinar objeto en eje y menos");                               

        
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
			::objeto.pose.position.x = ::objeto.pose.position.x + 0.005;
			
			anterior = ros::Time::now();
		}		
      }
      if (c == KEYCODE_Xmenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.position.x = ::objeto.pose.position.x - 0.005;

			anterior = ros::Time::now();
		}				
      }
      if (c == KEYCODE_Ymas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.position.z = ::objeto.pose.position.z + 0.005;
			
			anterior = ros::Time::now();
		}					    
      }
      if (c == KEYCODE_Ymenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.position.z = ::objeto.pose.position.z - 0.005;
			
			anterior = ros::Time::now();
		}						
      }
      if (c == KEYCODE_Zmas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.position.y = ::objeto.pose.position.y + 0.005;
			
			anterior = ros::Time::now();
		}					
      }
      if (c == KEYCODE_Zmenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.position.y = ::objeto.pose.position.y - 0.005;
			
			anterior = ros::Time::now();
		}				
      }
      if (c == KEYCODE_XgiroMas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.orientation.x = ::objeto.pose.orientation.x + 0.1;
			
			anterior = ros::Time::now();
		}	
      }
      if (c == KEYCODE_XgiroMenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.orientation.x = ::objeto.pose.orientation.x - 0.1;
			
			anterior = ros::Time::now();
		}	
      }
      if (c == KEYCODE_YgiroMas)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.orientation.y = ::objeto.pose.orientation.y + 0.1;
			
			anterior = ros::Time::now();
		}	
      }
      if (c == KEYCODE_YgiroMenos)
      {
		if (ros::Time::now() > anterior + ros::Duration(::retardo))
		{
			::objeto.pose.orientation.y = ::objeto.pose.orientation.y - 0.1;

			anterior = ros::Time::now();
		}	
      }
      
      object_pub_.publish(::objeto);

	}	
}
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "mover_objetos");   
 
	ros::NodeHandle n;
  	
  	ros::Publisher object_pub_=n.advertise<gazebo::ModelState>("gazebo/set_model_state", 1);    	 	


	signal(SIGINT,quit); 
	
	ros::Timer timer = n.createTimer(ros::Duration(1), callback);
    
   	   
 
	ros::spin();  
 
        return 0;
  }
