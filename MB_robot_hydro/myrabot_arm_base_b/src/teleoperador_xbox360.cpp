  #include "ros/ros.h"  
  #include "myrabot_arm_base_b/Servos.h"
  #include "myrabot_arm_base_b/WriteServos.h"  
  #include "myrabot_arm_base_b/ReadServos.h"
  #include "myrabot_arm_base_b/myrabot_arm_base_b.h"
  #include "geometry_msgs/Twist.h"
  #include "geometry_msgs/Point.h"    
  #include "sensor_msgs/Joy.h"
  #include "math.h"

myrabot_arm_base_b::Servos p, e, c, pinza;
geometry_msgs::Point punto;
int pinza_incli = 0;
int paso = 5;
int cont = 0;
	
myrabot_arm_base_b::WriteServos teleop;
myrabot_arm_base_b::WriteServos teleop_pinza;

float avance = 0.2, giro = 0.5;
int velocidad = 50;
int start = 0;
int flanco;

geometry_msgs::Twist base;

std::string elemento_final = "pinza";

float pad_izquierda_x, pad_izquierda_y, gatillo_izquierda, pad_derecha_x, pad_derecha_y, gatillo_derecha;
int boton_a, boton_b, boton_izquierda, boton_x, boton_y, boton_derecha, boton_back, boton_start, boton_pad_izquierda, boton_pad_derecha, cruz_izquierda, cruz_derecha, cruz_arriba, cruz_abajo;
 
  void posicionEstadoCorriente(const myrabot_arm_base_b::ReadServos& pec)   
  {
	
	::p = pec.posicion;
	::e = pec.estado;
	::c = pec.corriente;
	  
  }	  	 
 
  void xbox(const sensor_msgs::Joy& xbox_joystick)   
  {	
	
	::pad_izquierda_x = xbox_joystick.axes[0];
	::pad_izquierda_y = xbox_joystick.axes[1];
	::gatillo_izquierda = xbox_joystick.axes[2] -1;
	::pad_derecha_x = xbox_joystick.axes[3];
	::pad_derecha_y = xbox_joystick.axes[4];
	::gatillo_derecha = xbox_joystick.axes[5] -1;
	
	::boton_a = xbox_joystick.buttons[0];
	::boton_b = xbox_joystick.buttons[1];
	::boton_x = xbox_joystick.buttons[2];
	::boton_y = xbox_joystick.buttons[3];
	::boton_izquierda = xbox_joystick.buttons[4];
	::boton_derecha = xbox_joystick.buttons[5];
	::boton_back = xbox_joystick.buttons[6];
	::boton_start = xbox_joystick.buttons[7];
	::boton_pad_izquierda = xbox_joystick.buttons[9];
	::boton_pad_derecha = xbox_joystick.buttons[10];	
	::cruz_izquierda = xbox_joystick.buttons[11];
	::cruz_derecha = xbox_joystick.buttons[12];
	::cruz_arriba = xbox_joystick.buttons[13];
	::cruz_abajo = xbox_joystick.buttons[14];	
  }
 
  int main(int argc, char **argv)
  {
 
	ros::init(argc, argv, "teleoperador_xbox360");   
 
	ros::NodeHandle n;
	
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicionEstadoCorriente);   
  	
  	ros::Subscriber joystick_sub_= n.subscribe("joy", 1, xbox); 
  	
  	
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("move_arm", 1);   

	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("hand_arm", 1);
	
	ros::Publisher base_pub_=n.advertise<geometry_msgs::Twist>("cmd_vel", 1); 
	
	ros::param::get("~Elemento_Final",::elemento_final);
		
    ros::Rate loop_rate(5);	
	
	while (ros::ok())
	{
	
		myrabot_arm_base_b::Servos pose = ::p;	
		
		if (::cont == 0 && (pose.base != 0 && pose.arti1 != 0 && pose.arti2 != 0 && pose.arti3 != 0))
		{
			::punto = home(::p, ::c, ::elemento_final);
		
			::cont = 1;
		
			::pinza.pinza = 511;
		
			::teleop = inversa(::punto, ::pinza_incli, ::p, ::velocidad);		
		
			::teleop_pinza = controlPinza(::pinza, ::p, ::c);
		}

		if (::pad_izquierda_y > 0.2 || ::pad_izquierda_y < -0.2 || ::pad_izquierda_x > 0.2 || ::pad_izquierda_x < -0.2 || ::pad_derecha_x > 0.2 || ::pad_derecha_x < -0.2 || pad_derecha_y > 0.2 || pad_derecha_y < -0.2 || (::gatillo_izquierda < -0.2 && ::gatillo_izquierda != -1) || (::gatillo_derecha < -0.2 && ::gatillo_derecha != -1) || ::cruz_izquierda !=  0 || ::cruz_derecha !=  0 || ::cruz_arriba != 0 || ::cruz_abajo != 0 || ::boton_back != 0 || ::boton_start != 0 || ::boton_a != 0 || ::boton_b != 0 || ::boton_x != 0 || ::boton_y != 0 || ::boton_izquierda != 0 || ::boton_derecha != 0)
		{
		
			if (::boton_start != 0 && ::flanco == 0)
			{
				flanco = 1;
			}
			else 
			{
				if (::boton_start != 0)
				{
					::flanco = 2;
				}
				else
				{
					::flanco = 0;
				}
			}
			
		
			if (::boton_start != 0 && ::flanco == 1)
			{
				if (::start == 0)
				{
					::start = 1;
				}
				else
				{
					::start = 0;
				}
			}
			
			if (::start == 1)
			{
			
				if (::boton_back != 0)
				{
					::cont = 0;
				}
				
				if (::cruz_izquierda != 0)
				{
					::velocidad = ::velocidad - 10;
					
					if (::velocidad < 10)
					{
						::velocidad = 10;
					}
				}
				
				if (::cruz_derecha != 0)
				{
					::velocidad = ::velocidad + 10;
					
					if (::velocidad > 100)
					{
						::velocidad = 100;
					}
				}

				if (::pad_derecha_x > 0.2 || ::pad_derecha_x < -0.2)
				{
					float f_pad_derecha_x = ::pad_derecha_x;
					::punto.x = ::punto.x + (f_pad_derecha_x * ::paso);
					::teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3) 
					{::punto.x = ::punto.x - (f_pad_derecha_x * ::paso);}
					else
					{move_pub_.publish(::teleop);}
				}
	
				if (pad_derecha_y > 0.2 || pad_derecha_y < -0.2)
				{
					float f_pad_derecha_y = ::pad_derecha_y;
					::punto.z = ::punto.z + (f_pad_derecha_y * ::paso);
					::teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3) 
					{::punto.z = ::punto.z - (f_pad_derecha_y * ::paso);}
					else
					{move_pub_.publish(::teleop);}
				}
				
				if (::gatillo_izquierda < -0.2 && ::gatillo_izquierda != -1)
				{
					float f_gatillo_izquierda = ::gatillo_izquierda;
					::punto.y = ::punto.y + (f_gatillo_izquierda  * (::paso/2));
					::teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3) 
					{::punto.y = ::punto.y - (f_gatillo_izquierda * (::paso/2));}
					else
					{move_pub_.publish(::teleop);}
				}
				
				if (::gatillo_derecha < -0.2 && ::gatillo_derecha != -1)
				{
					float f_gatillo_derecha = ::gatillo_derecha;
					::punto.y = ::punto.y - (f_gatillo_derecha  * (::paso/2));
					::teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3) 
					{::punto.y = ::punto.y + (f_gatillo_derecha * (::paso/2));}
					else
					{move_pub_.publish(::teleop);}
				}		
				
				if (::cruz_arriba !=  0)
				{
					::pinza_incli = ::pinza_incli - (::paso * ::cruz_arriba);
					teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3)
					{::pinza_incli = ::pinza_incli + (::paso * ::cruz_arriba);}
					else
					{move_pub_.publish(::teleop);}
				}
				
				if (::cruz_abajo !=  0)
				{
					::pinza_incli = ::pinza_incli + (::paso * ::cruz_abajo);
					::teleop = inversa(::punto, ::pinza_incli, pose, ::velocidad, ::elemento_final);
					if (::teleop.posicion.base == pose.base && ::teleop.posicion.arti1 == pose.arti1 && ::teleop.posicion.arti2 == pose.arti2 && ::teleop.posicion.arti3 == pose.arti3)
					{::pinza_incli = ::pinza_incli - (::paso * ::cruz_abajo);}
					else
					{move_pub_.publish(::teleop);}
				}
				
				if (::boton_izquierda != 0)
				{     
					::pinza.pinza = ::pinza.pinza - (::paso * ::boton_izquierda);
					::teleop_pinza = controlPinza(::pinza, pose, ::c);
					if (::teleop_pinza.posicion.pinza == pose.pinza) 
					{::pinza.pinza = ::pinza.pinza + (::paso * ::boton_izquierda);}
					else
					{hand_pub_.publish(::teleop_pinza);}	
				}
				
				if (::boton_derecha != 0)
				{     
					::pinza.pinza = ::pinza.pinza + (::paso * ::boton_derecha);
					::teleop_pinza = controlPinza(::pinza, pose, ::c);
					if (::teleop_pinza.posicion.pinza == pose.pinza) 
					{::pinza.pinza = ::pinza.pinza - (::paso * ::boton_derecha);}
					else
					{hand_pub_.publish(::teleop_pinza);}	
				}
				
				if (::boton_a != 0)
				{
					::avance = ::avance - 0.05;
					
					if (::avance < 0.05)
					{
						::avance = 0.05;
					}
				}
				
				if (::boton_b != 0)
				{
					::avance = ::avance + 0.05;
					
					if (::avance > 0.5)
					{
						::avance = 0.5;
					}
				}
				
				if (::pad_izquierda_y > 0.2 || ::pad_izquierda_y < -0.2)
				{
					::base.linear.x = ::avance * pad_izquierda_y;
					base_pub_.publish(::base);
				}
				else
				{
					::base.linear.x = 0;
				}
				
				if (::boton_x != 0)
				{
					::giro = ::giro - 0.05;
					
					if (::giro < 0.1)
					{
						::giro = 0.1;
					}
				}
				
				if (::boton_y != 0)
				{
					::giro = ::giro + 0.05;
					
					if (::giro > 1.5)
					{
						::giro = 1.5;
					}
				}
				
				if (::pad_izquierda_x > 0.2 || ::pad_izquierda_x < -0.2)
				{
					::base.angular.z = ::giro * pad_izquierda_x;
					base_pub_.publish(::base);
				}
				else
				{
					::base.angular.z = 0;
				}	
			}
		}
		
		ros::spinOnce();
	
		loop_rate.sleep();
	}		  
 
        return 0;
  }
