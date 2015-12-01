  #include "ros/ros.h"
  #include "myrabot_arm_base/Servos.h"
  #include "myrabot_arm_base/WriteServos.h"
  #include "myrabot_arm_base/ReadServos.h"
  #include "myrabot_arm_base/SwitchesSecuence.h"
  #include "myrabot_arm_base/WriteServosTrajectory.h"
  #include "myrabot_arm_base/myrabot_arm_base.h"
  #include "std_msgs/UInt8MultiArray.h"
  #include "std_msgs/Bool.h"
  #include "std_msgs/String.h"
  #include "geometry_msgs/Point.h"
  #include "math.h"
  #include "std_msgs/Float32MultiArray.h"
  #include "tf/transform_listener.h"
  #include "find_object_2d/ObjectsStamped.h"
  #include "sensor_msgs/Imu.h"
  #include "nav_msgs/Odometry.h"
  #include "geometry_msgs/Twist.h"
  #include "geometry_msgs/Pose.h"
  
  #define PI 3.14159265

	static double angulo_1_ = 0;
	static double angulo_2_ = 0.6;

  int inclinacion_pinza = 0;
  myrabot_arm_base::Servos p;
  myrabot_arm_base::Servos c;
  myrabot_arm_base::WriteServos move, move_acercar;
  int start=0, cont=0, cont_1=0, cont_2=0, cont_3=0, cont_4=0;
  
  bool start_task = false;

  geometry_msgs::Point punto, punto_1;
  
  geometry_msgs::Pose base_pose;
  geometry_msgs::Twist move_base;
  
  myrabot_arm_base::SwitchesSecuence secuencia;
  
  bool secuencia_recibida = false;
  
  std::vector<int> id;
  ros::Time tiempo;
  
  std_msgs::UInt8MultiArray histograma;
  
  struct iluminacion
  {bool sin_cambio;
   bool aumento;
   bool disminucion;};
  
  void objetosDetectados(const find_object_2d::ObjectsStamped& msg)
  {
    for(unsigned int i = 0; i < msg.objects.data.size(); i+=12)
    {
		::tiempo = msg.header.stamp;
        ::id.push_back( (int)msg.objects.data[i] );
	}
  }
  
  void posicionEstadoCorriente(const myrabot_arm_base::ReadServos& pec)   
  {
	ros::NodeHandle n;

	ros::Publisher effector_pub_=n.advertise<geometry_msgs::Pose>("effector_position", 1);

	::p = pec.posicion;
	::c = pec.corriente;
	::start = 1;
	
	geometry_msgs::Pose effector;

	effector.position = directa(::p, ::inclinacion_pinza, "dedo");

	effector.orientation.x = 0;
	effector.orientation.y = 0;
	effector.orientation.z = 0;
	effector.orientation.w = 0;

	effector_pub_.publish(effector);
  }
  
  void startTask(const std_msgs::Bool& task)
  {
	::start_task = task.data;
  }

  void posicionBase(const nav_msgs::Odometry& odom)
  {
	::base_pose.position.x = odom.pose.pose.position.x;
	::base_pose.position.y = odom.pose.pose.position.y;
  }
 
  void orientacionBase(const sensor_msgs::Imu& imu)
  {
	tf::Quaternion quat;
	
    tf::quaternionMsgToTF(imu.orientation, quat);
	  
	tf::Matrix3x3 m(quat);
					
	double R, P, Y;
					
	m.getRPY(R, P, Y);
	
	::base_pose.orientation.z = Y;
  }
  
  void secuenciaInterruptores(const myrabot_arm_base::SwitchesSecuence& secuence)
  {   
	  ::secuencia.initial_state = std::vector<uint8_t>(secuence.initial_state.size());

	  ::secuencia.switches = std::vector<int>(secuence.switches.size());
	  
	  for (int i = 0; i < secuence.initial_state.size(); i++)
	  {
		  ::secuencia.initial_state[i] = secuence.initial_state[i];
	  }
	  
	  for (int i = 0; i < secuence.switches.size(); i++)
	  {
		  ::secuencia.switches[i] = secuence.switches[i];
	  }
	  
	  ::secuencia_recibida = true;
	  
	  std::cout<<::secuencia<<std::endl;
  }
  
  void histogramaImagen(const std_msgs::UInt8MultiArray& histogram)   
  {    
	::histograma = histogram;
  }
  
  bool girarAnguloBase(double izquierda_derecha, double giro, double angulo_0, double angulo, double error)
  {
	bool alcanzado = false;
	
	double angulo_uno = angulo - std::abs(giro);
	
	double angulo_cero = angulo_0, angulo_cero_ = angulo_0;
	
	if (angulo_0 != 4)
	{
		std::cout<<"giro:"<<angulo_uno<<" Y_0:"<<angulo_cero<<std::endl;
		
		if (izquierda_derecha > 0 && (angulo_0 + angulo_uno) > PI)
		{angulo_cero = angulo_0 - 2*PI;}

		if (izquierda_derecha < 0 && (angulo_0 - angulo_uno) < -PI)
		{angulo_cero = angulo_0 + 2*PI;}
			
		std::cout<<izquierda_derecha<<" Y:"<<::base_pose.orientation.z<<" Y_0:"<<angulo_cero<<std::endl;
		
		if (izquierda_derecha > 0 && (::base_pose.orientation.z > (angulo_cero + angulo_uno + error) || ::base_pose.orientation.z < (angulo_cero + angulo_uno - error))) //izquierda
		{
			if ((angulo_0 + angulo_uno) > PI && ::base_pose.orientation.z < 0)
			{angulo_cero_ = angulo_cero;}
			::move_base.angular.z = (((angulo_uno - std::abs(angulo_cero_ - ::base_pose.orientation.z)) * 0.1)/angulo_uno) + 0.05;
		}
		else if (izquierda_derecha < 0 && (::base_pose.orientation.z > (angulo_cero - angulo_uno + error) || ::base_pose.orientation.z < (angulo_cero - angulo_uno - error))) //derecha
		{
			if ((angulo_0 - angulo_uno) < -PI && ::base_pose.orientation.z > 0)
			{angulo_cero_ = angulo_cero;}
			::move_base.angular.z = (((angulo_uno - std::abs(angulo_cero_ - ::base_pose.orientation.z)) * -0.1)/angulo_uno) - 0.05;
		}
		else
		{
			::move_base.angular.z = 0;
			
			alcanzado = true;
		}
	}
	
	return alcanzado;
  }
  
  bool moverDistanciaBase(geometry_msgs::Pose base_pose_0, double distancia, double error)
  {
	bool alcanzado = false;
	
	if (distancia != 0)
	{  
		double avanzado = sqrt(pow(::base_pose.position.x - base_pose_0.position.x,2) + pow(::base_pose.position.y - base_pose_0.position.y,2))*1000;
		
		if (distancia > 0 && avanzado < (std::abs(distancia) - error))
		{
			::move_base.linear.x = (((distancia - avanzado) * 0.05)/distancia) + 0.02;
		}
		else if (distancia < 0 && avanzado < (std::abs(distancia) -error))
		{
			::move_base.linear.x = (((distancia + avanzado) * -0.05)/distancia) - 0.02;
		}
		else
		{
			::move_base.linear.x = 0.0;
			
			alcanzado = true;
		}
		
		std::cout<<distancia<<"... vete a cagar"<<std::endl;
	}
	
	return alcanzado;
  }
  
  bool acercarInterruptor(desplazamiento avance, geometry_msgs::Pose base_pose_0)
  {
	ros::NodeHandle n;  
	  
	ros::Publisher base_pub_=n.advertise<geometry_msgs::Twist>("cmd_vel", 1);  
	
	static bool regresar = false;
	
	static double angulo_2 = 4;
	
	double angulo_1 =  base_pose_0.orientation.z;
	
	bool mantener_objeto = true;
	
	if (angulo_2 != 4)
	{angulo_1 = 4;}

	if (avance.izquierda_derecha != 0)
	{	
		if (girarAnguloBase(avance.izquierda_derecha, avance.giro, angulo_1, 1.57, 0.1) && !regresar)
		{
			::move_base.angular.z = 0.0;
			
			if (moverDistanciaBase(base_pose_0, std::abs(avance.izquierda_derecha), 1))
			{
				::move_base.linear.x = 0.0;
				
				angulo_2 = ::base_pose.orientation.z;
				
				regresar = true;
			}
		}
		else if (girarAnguloBase(-avance.izquierda_derecha, 0, angulo_2, 1.57, 0.1) && regresar)
		{
			::move_base.linear.x = 0.0;
							
			mantener_objeto = false;
			
			angulo_2 = 4;
			
			regresar = false;
		}
	}
	else if (avance.adelante_atras != 0)
	{
		if (moverDistanciaBase(base_pose_0, avance.adelante_atras, 3))
		{
			::move_base.linear.x = 0.0;
			
			mantener_objeto = false;
		}
	}
	else if (avance.giro != 0)
	{
		if (girarAnguloBase(avance.giro, 0, angulo_1, std::abs(avance.giro), 0.1))
		{
			mantener_objeto = false;
		}
	}
		  
	base_pub_.publish(::move_base);
	
	return mantener_objeto;
  }
  
  bool buscarInterruptor(geometry_msgs::Pose base_pose_0)
  {
	ros::NodeHandle n;  
	  
	ros::Publisher base_pub_=n.advertise<geometry_msgs::Twist>("cmd_vel", 1); 
	
	static bool distancia = false;
	
	double avance = -20;
	
	int sentido = -1;
	
	if (distancia)
	{avance = 0;}
	
	if (angulo_1_ > 0.8)
	{sentido = 1;}
	
	std::cout<<angulo_1_<<" - "<<angulo_2_<<std::endl;
	  
	bool buscar_objeto = true;

	if (moverDistanciaBase(base_pose_0, avance, 3) || distancia)
	{
		::move_base.linear.x = 0.0;
		
		distancia = true;
		
		if (girarAnguloBase(sentido, 0, base_pose_0.orientation.z, 0.2, 0.08))
		{
			::move_base.angular.z = 0.0;
			
			if (sentido == -1)
			{angulo_1_ = angulo_1_ + 0.2;}
			else
			{angulo_2_ = angulo_2_ - 0.2;}
			
			buscar_objeto = false;
		}
	}
	
	if (angulo_2_ < -0.8)
	{
		angulo_1_ = -1;
		angulo_2_ = 1;
		distancia = false;
	}
	
	base_pub_.publish(::move_base);
	
	return buscar_objeto;
  }
  
  iluminacion cambioIluminacion(std_msgs::UInt8MultiArray anterior, std_msgs::UInt8MultiArray posterior)
  {
	iluminacion cambio;
	
	cambio.sin_cambio = false;
	cambio.aumento = false;
	cambio.disminucion = false;
	
	std::vector<int> aumento;
	
	std::vector<int> disminucion;
	
	int aumento_suma = 0;
	
	int disminucion_suma = 0;
	
	for (int i = 170; i < 256; i++)
	{
		int diferencia = posterior.data[i] - anterior.data[i];
		
		if (diferencia > 0)
		{aumento.push_back(diferencia);}
		else if (diferencia < 0)
		{disminucion.push_back(-diferencia);}
	}
	
	for (int i = 0; i < aumento.size(); i++)
	{
		aumento_suma = aumento_suma + aumento[i];
	}
	
	for (int i = 0; i < disminucion.size(); i++)
	{
		disminucion_suma = disminucion_suma + disminucion[i];
	}
	
	if (aumento.size() < (256-170)*0.8 && disminucion.size() < (256-170)*0.8)
	{cambio.sin_cambio = true;}
	else if (aumento_suma > disminucion_suma)
	{cambio.aumento = true;}
	else
	{cambio.disminucion = true;}

	return cambio;
  }
 
  int main(int argc, char **argv)
  {
 
	int n_interruptores = argc-3;
 
	ros::init(argc, argv, "control_interruptores");  
 
	ros::NodeHandle n;
	
  	
  	ros::Subscriber pose_sub_= n.subscribe("pose_arm", 1, posicionEstadoCorriente);

    ros::Subscriber objects_sub_= n.subscribe("objectsStamped", 1, objetosDetectados);
    
    ros::Subscriber odom_sub_= n.subscribe("odom", 1, posicionBase);
    
    ros::Subscriber imu_sub_= n.subscribe("imu/data", 1, orientacionBase);
    
    ros::Subscriber secuence_sub_= n.subscribe("switchesSecuence", 1, secuenciaInterruptores);
    
    ros::Subscriber histo_sub_= n.subscribe("histogram", 1, histogramaImagen);
    
    ros::Subscriber task_sub_= n.subscribe("manipulation_start", 1, startTask);

  	
	ros::Publisher move_pub_=n.advertise<myrabot_arm_base::WriteServos>("move_arm", 1);  

	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base::WriteServos>("hand_arm", 1);
	
	ros::Publisher base_pub_=n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	
	//publicaciones requeridas
	ros::Publisher notif_pub_=n.advertise<std_msgs::String>("notification", 1);

	ros::Publisher target_pub_=n.advertise<geometry_msgs::Pose>("target_position", 1);

	ros::Publisher effector_pub_=n.advertise<geometry_msgs::Pose>("effector_position", 1);

	
	tf::TransformListener tf_listener;
		
	tf::StampedTransform transform;	
	
	struct posicionInterruptor
	{int                  id;
	 int                  type;
	 geometry_msgs::Point on;
	 geometry_msgs::Point off;
	};
	
	posicionInterruptor *interruptores = new posicionInterruptor[n_interruptores];
	
	posicionInterruptor interruptor;
	
	for (int i = 0; i < n_interruptores; i++)
	{
		ros::NodeHandle nh(argv[i+1]);
		
		std::cout<<argv[i+1]<<std::endl;
		
		int switch_id;
		nh.getParam("switch_id", switch_id);
		interruptor.id = switch_id;

		int switch_type;
		nh.getParam("switch_type", switch_type);
		interruptor.type = switch_type;
		
		int posicion_on_x;
		nh.getParam("posicion_on_x", posicion_on_x);
		interruptor.on.x = posicion_on_x;
		
		int posicion_on_y;
		nh.getParam("posicion_on_y", posicion_on_y);
		interruptor.on.y = posicion_on_y;
		
		int posicion_on_z;
		nh.getParam("posicion_on_z", posicion_on_z);
		interruptor.on.z = posicion_on_z;
		
		int posicion_off_x;
		nh.getParam("posicion_off_x", posicion_off_x);
		interruptor.off.x = posicion_off_x;
		
		int posicion_off_y;
		nh.getParam("posicion_off_y", posicion_off_y);
		interruptor.off.y = posicion_off_y;
		
		int posicion_off_z;
		nh.getParam("posicion_off_z", posicion_off_z);
		interruptor.off.z = posicion_off_z;
		
		interruptores[i] = interruptor;
		
		std::cout<<interruptor.id<<interruptor.type<<" - "<<interruptor.on<<interruptor.off<<std::endl;
	}
	
	double R, P, Y;
	
	bool light_status = false;
	
	std::string object_link;
	
	bool existe_objetivo = false;
	
	bool alcance_objetivo = false;
	
	int j = 0;
	
	desplazamiento alcance;
	
	geometry_msgs::Pose base_pose_0;
	
	int intentos = 0;
	
	bool buscar_objetivo = false;
	
	int retroceso = 0;
	
	bool existe_trayectoria = false;
	
	myrabot_arm_base::WriteServosTrajectory trayecto;
	
	bool objetivo_ok = false;
	
	int objetivo = 0;
	
	int objetivo_id = 0;
	
	int id_switch = 0;

	int colocar = 0;

	bool orientando_panel = false;

	ros::Rate loop_rate(20);
	

	while (ros::ok())
	{
		if (::cont == 0 && ::start == 1)
		{	
			geometry_msgs::Point punto_0;
			
			punto_0 = home(::p, ::c, "dedo");
			
			::move = inversa(punto_0, ::inclinacion_pinza, ::p, 0, "dedo");
			
			::cont = 1;
		}
		
	if(::secuencia_recibida && ::start_task && objetivo < ::secuencia.switches.size())
	{
		if (objetivo_ok)
		{
			objetivo = objetivo + 1;
			
			objetivo_ok = false;
		}
		
		objetivo_id = ::secuencia.switches[objetivo];
		
		for (int i = 0; i < n_interruptores; i ++)
		{
			if (interruptores[i].id == objetivo_id)
			{
				id_switch = i;
			}
		}

		if (!existe_objetivo)
		{
			if (!buscar_objetivo)
			{
				std::cout<<"array_position:"<<objetivo<<"objetc_"<<objetivo_id<<intentos<<std::endl;
				
				//for(unsigned int i=0; i < ::id.size(); i++)
				//{
					//if (::id[i] == objetivo_id)
					//{
						char object_link[11];
						
						sprintf(object_link, "/object_%d", objetivo_id);
						
						try{
						  tf_listener.waitForTransform("/fixed_link", object_link, ros::Time(0), ros::Duration(1.0));
						  tf_listener.lookupTransform("/fixed_link", object_link, ros::Time(0), transform);
						  
						  if (::secuencia.initial_state[objetivo_id-1] == 1)
						  {
							  ::punto.y = (-transform.getOrigin().getX()*1000) + interruptores[id_switch].off.y;
							  ::punto.x = (-transform.getOrigin().getY()*1000) + interruptores[id_switch].off.x;
							  ::punto.z = (transform.getOrigin().getZ()*1000) + interruptores[id_switch].off.z;

							  ::punto_1.y = (-transform.getOrigin().getX()*1000) + interruptores[id_switch].on.y;
							  ::punto_1.x = (-transform.getOrigin().getY()*1000) + interruptores[id_switch].on.x;
							  ::punto_1.z = (transform.getOrigin().getZ()*1000) + interruptores[id_switch].on.z;
						  }
						  else
						  {
							  ::punto.y = (-transform.getOrigin().getX()*1000) + interruptores[id_switch].on.y;
							  ::punto.x = (-transform.getOrigin().getY()*1000) + interruptores[id_switch].on.x;
							  ::punto.z = (transform.getOrigin().getZ()*1000) + interruptores[id_switch].on.z;

							  ::punto_1.y = (-transform.getOrigin().getX()*1000) + interruptores[id_switch].off.y;
							  ::punto_1.x = (-transform.getOrigin().getY()*1000) + interruptores[id_switch].off.x;
							  ::punto_1.z = (transform.getOrigin().getZ()*1000) + interruptores[id_switch].off.z;
						  }
						
						  //Y<0.02 -izquierda +derecha paralelo al panel de interruptores
						  tf::Matrix3x3 m(transform.getRotation());
						
						  m.getRPY(R, P, Y);
						
						  std::cout<<"punto:"<<::punto<<" orientacion:"<<Y<<std::endl;
						
						  alcance = dentroAlcance(::punto, Y, ::inclinacion_pinza, ::p, 0, "dedo");
						  
						  geometry_msgs::Pose target;

						  target.position = ::punto;

						  target.orientation.x = transform.getRotation().getX();
						  target.orientation.y = transform.getRotation().getY();
						  target.orientation.z = transform.getRotation().getZ();
						  target.orientation.w = transform.getRotation().getW();

						  target_pub_.publish(target);


						  /*base_pose_0.position.x = ::base_pose.position.x;
						  base_pose_0.position.y = ::base_pose.position.y;
						  base_pose_0.orientation.z = ::base_pose.orientation.z;*/
						  
						  existe_objetivo = true;
						}
						catch (tf::TransformException &ex) {
						  ROS_WARN("%s",ex.what());
						  //continue;
						}
						
					  if (colocar == 12)
					  {
						  double orient = 0;

						  int n_detect = 0;

						  for (int i = 0; i < n_interruptores; i++)
						  {
							  int id_ = interruptores[i].id;

								sprintf(object_link, "/object_%d", id_);

								try{
								  tf_listener.waitForTransform("/fixed_link", object_link, ros::Time(0), ros::Duration(1.0));
								  tf_listener.lookupTransform("/fixed_link", object_link, ros::Time(0), transform);

								  //Y<0.02 -izquierda +derecha paralelo al panel de interruptores
								  tf::Matrix3x3 m(transform.getRotation());

								  m.getRPY(R, P, Y);

								  orient = orient + Y;

								  n_detect = n_detect + 1;
								}
								catch (tf::TransformException &ex) {
								  ROS_WARN("%s",ex.what());
								  //continue;
								}
						  }

						  orient = orient/n_detect;

						  alcance.en_posicion =

						  alcance.izquierda_derecha = 0;

						  alcance.giro = orient;

						  orientando_panel = true;

						  existe_objetivo = true;
					  }

						//break;
					//}
					
					base_pose_0.position.x = ::base_pose.position.x;
				    base_pose_0.position.y = ::base_pose.position.y;
				    base_pose_0.orientation.z = ::base_pose.orientation.z;
				//}
				
				/*if (!existe_objetivo)
				{
					base_pose_0.position.x = ::base_pose.position.x;
					base_pose_0.position.y = ::base_pose.position.y;
					base_pose_0.orientation.z = ::base_pose.orientation.z;
				}*/
				
			}
			
			if (existe_objetivo || orientando_panel)
			{
				intentos = 0;

				angulo_1_ = 0;
				angulo_2_ = 0.6;
			}
			else if (intentos > 2)
			{
				buscar_objetivo = true;
				
				if (!buscarInterruptor(base_pose_0))
				{
					buscar_objetivo = false;
					
					intentos = 0;
					
					colocar = colocar + 1;

					//sleep(4);
				}
			}
			else
			{	
				intentos = intentos + 1;
			}
		}
		else
		{
			std::cout<<alcance.en_posicion<<std::endl;
			std::cout<<alcance.izquierda_derecha<<std::endl;
			std::cout<<alcance.adelante_atras<<std::endl;
			std::cout<<alcance.giro<<std::endl;
			
			if (alcance.en_posicion)
			{
				if (interruptores[objetivo_id].type == 0)
				{
					::punto_1.y = ::punto.y - 50;
					::punto_1.x = ::punto.x;
					::punto_1.z = ::punto.z;
				
					alcance = dentroAlcance(::punto_1, Y, ::inclinacion_pinza, ::p, 0, "dedo");
					
					if (!alcance.en_posicion)
					{
						retroceso = retroceso + 1;

						alcance.adelante_atras = -20 * retroceso;

						alcance_objetivo = false;

						colocar = colocar + 2;
					}
					else
					{
						retroceso = 0;

						::move_acercar = inversa(::punto_1, ::inclinacion_pinza, ::p, 0, "dedo");

						alcance_objetivo = true;

						colocar = 0;
					}
				}
				else
				{
					::move_acercar = inversa(::punto_1, ::inclinacion_pinza, ::p, 0, "dedo");

					alcance_objetivo = true;
				}
			}
			else
			{
				if (!acercarInterruptor(alcance, base_pose_0))
				{
					existe_objetivo = false;
					
					orientando_panel = false;

					colocar = colocar + 2;

					//sleep(4);
				}
			}
		}
			
		if (puntoAlcanzado(::p, ::move, 20) && alcance_objetivo && std::abs(::c.pinza) < 70 && alcance.en_posicion) {
		
			if (!puntoAlcanzado(::p, ::move_acercar, 20) && !existe_trayectoria)
			{	
				::move = ::move_acercar;
				
				if (!igualPosicion(::p, ::move))
				{
					move_pub_.publish(::move);
				}
			}
			else
			{
				if (!existe_trayectoria)
				{
					trayecto = trayectoria(::punto_1, ::punto, ::inclinacion_pinza, ::p, 0, "dedo");

					existe_trayectoria = trayecto.correcta;
				}
				else if (existe_trayectoria && j < trayecto.trayectoria.size())
				{
					::move = trayecto.trayectoria[j];
					
					move_pub_.publish(::move);
					
					j = j + 1;
				}
				else
				{
					j = 0;
					::cont = 0;
					::cont_1 = 0;
					existe_objetivo = false;
					existe_trayectoria = false;
					alcance_objetivo = false;
					buscar_objetivo = false;
					objetivo_ok = true;

					char object_notif[19];

					sprintf(object_notif, "Switch %d operated", objetivo_id);

					std_msgs::String notif;

					notif.data = object_notif;

					notif_pub_.publish(notif);
				}
			}
		}
		else if (std::abs(::c.pinza) >= 70)
		{
			j = 0;
			::cont = 0;
			::cont_1 = 0;
			existe_objetivo = false;
			existe_trayectoria = false;
			alcance_objetivo = false;
			buscar_objetivo = false;
			objetivo_ok = true;

			char object_notif[19];

			sprintf(object_notif, "Switch %d operated", objetivo_id);

			std_msgs::String notif;

			notif.data = object_notif;

			notif_pub_.publish(notif);
		}
		else if (colocar > 20 && !orientando_panel)
		{
			j = 0;
			::cont = 0;
			::cont_1 = 0;
			existe_objetivo = false;
			existe_trayectoria = false;
			alcance_objetivo = false;
			buscar_objetivo = false;
			objetivo_ok = true;

			colocar = 0;

			char object_notif[18];

			sprintf(object_notif, "Switch %d skipped", objetivo_id);

			std_msgs::String notif;

			notif.data = object_notif;

			notif_pub_.publish(notif);
		}
	}	
		ros::spinOnce();
	
		loop_rate.sleep();	
	
	}
	
	ros::spin();  
 
        return 0;
  }
