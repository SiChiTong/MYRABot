#ifndef myrabot_arm_base_b_H
#define myrabot_arm_base_b_H

  #include "ros/ros.h"
  #include "myrabot_arm_base_b/Servos.h"
  #include "myrabot_arm_base_b/WriteServos.h"  
  #include "myrabot_arm_base_b/ReadServos.h"
  #include "myrabot_arm_base_b/WriteServosTrajectory.h"
  #include "std_msgs/Int16.h"
  #include "geometry_msgs/Point.h"
  #include "math.h" 
  
  #define PI  3.14159265
  #define z0  80
  #define L1  104
  #define L2  104
  #define Lp  60
  #define Dxp 0
  #define Dyp 0
  #define Ld  158
  #define Dxd 47
  #define Dyd 0
  #define max_cerca   250
  #define max_lejos   300
  #define max_lateral 100

geometry_msgs::Point directa(myrabot_arm_base_b::Servos posicion_servos_0, int inclinacion_pinza)
  {
	
	double alfa, beta, beta_a, beta_p, beta_pp, gamma, delta, delta_a, delta_p1, delta_p2, epsilon;
	double z_p;
	double L_a, L;
	double a, b;
	double x, y, z;
	
	alfa = (((posicion_servos_0.base*300)/1023)-150)*PI/180;
	beta = (((posicion_servos_0.arti1*300)/1023)-60)*PI/180;
	gamma = (((posicion_servos_0.arti2*300)/1023)+30)*PI/180;
	delta = (((posicion_servos_0.arti3*300)/1023)+30)*PI/180;
	
	epsilon = (inclinacion_pinza*PI)/180;	
	
	L_a = sqrt(pow(L1,2)+pow(L2,2)-2*L1*L2*cos(gamma));
	
	beta_a = acos((pow(L1,2)+pow(L_a,2)-pow(L2,2))/(2*L1*L_a));
	
	beta_p = beta - beta_a;
	
	delta_a = PI - (beta_a + gamma);
	
	delta_p1 = delta - delta_a;
	
	delta_p2 = 2*PI - (delta - delta_a);
	
	if (delta_p1 < delta_p2) 
	{
		L = sqrt(pow(L_a,2)+pow(Lp,2)-2*L_a*Lp*cos(delta_p1));
	}
	else
	{
		L = sqrt(pow(L_a,2)+pow(Lp,2)-2*L_a*Lp*cos(delta_p2));
	}
	
	z_p = L_a*cos(beta_p) + Lp*cos(epsilon);
	
	beta_pp = acos(z_p/L);
	
	a = L1*sin(beta);
	
	b = L2*sin(beta+gamma)+Lp*sin(epsilon);
	
	if (a >= b) 
	{
		y = L*sin(beta_pp);
	}
	else
	{
		y = -L*sin(beta_pp);
	}	
	
	z = z_p*cos(alfa);
	
	x = z_p*sin(alfa);
	
	geometry_msgs::Point punto;
	
	punto.x = x;
	punto.z = y + z0;
	punto.y = z;
	
	return punto; 
  } 

geometry_msgs::Point directa(myrabot_arm_base_b::Servos posicion_servos_0, int inclinacion_pinza, std::string elemento_final)
  {
	
	double alfa, beta, beta_a, beta_p, beta_pp, gamma, delta, delta_a, delta_p1, delta_p2, epsilon;
	double z_p;
	double L_a, L;
	double a, b;
	double x, y, z;
	
	double Lf;
	double Dx, Dy;
	
	if (elemento_final == "dedo")
	{
		Lf = Ld;
		Dx = Dxd;
		Dy = Dyd;
	}
	else
	{
		Lf = Lp;
		Dx = Dxp;
		Dy = Dyp;
	}
	
	alfa = (((posicion_servos_0.base*300)/1023)-150)*PI/180;
	beta = (((posicion_servos_0.arti1*300)/1023)-60)*PI/180;
	gamma = (((posicion_servos_0.arti2*300)/1023)+30)*PI/180;
	delta = (((posicion_servos_0.arti3*300)/1023)+30)*PI/180;
	
	epsilon = (inclinacion_pinza*PI)/180;	
	
	L_a = sqrt(pow(L1,2)+pow(L2,2)-2*L1*L2*cos(gamma));
	
	beta_a = acos((pow(L1,2)+pow(L_a,2)-pow(L2,2))/(2*L1*L_a));
	
	beta_p = beta - beta_a;
	
	delta_a = PI - (beta_a + gamma);
	
	delta_p1 = delta - delta_a;
	
	delta_p2 = 2*PI - (delta - delta_a);
	
	if (delta_p1 < delta_p2) 
	{
		L = sqrt(pow(L_a,2)+pow(Lf,2)-2*L_a*Lf*cos(delta_p1));
	}
	else
	{
		L = sqrt(pow(L_a,2)+pow(Lf,2)-2*L_a*Lf*cos(delta_p2));
	}
	
	z_p = L_a*cos(beta_p) + Lf*cos(epsilon);
	
	beta_pp = acos(z_p/L);
	
	a = L1*sin(beta);
	
	b = L2*sin(beta+gamma)+Lf*sin(epsilon);
	
	if (a >= b) 
	{
		y = L*sin(beta_pp);
	}
	else
	{
		y = -L*sin(beta_pp);
	}	
	
	z = z_p*cos(alfa);
	
	x = z_p*sin(alfa);
	
	x = x + Dx*cos(alfa);
 	
 	z = z + Dx*sin(alfa);
 	
 	y = y + Dy;
	
	geometry_msgs::Point punto;
	
	punto.x = x;
	punto.z = y + z0;
	punto.y = z;
	
	return punto; 
  } 
  
  myrabot_arm_base_b::WriteServos inversa(geometry_msgs::Point destino, int inclinacion_pinza, myrabot_arm_base_b::Servos posicion_servos_0, int velocidad) 
  {
	  
	double x = destino.x;
  	double z = destino.y;
	double y = destino.z - z0;

	int coordenadas_correctas = 1;

	double alfa, beta, beta_a, beta_p, beta_pp, gamma, delta, delta_a, delta_p, epsilon;
	double z_p;
	double L_a, L;
 
	//correccion peso
	y = y + ((y/250) * 25);

	epsilon = (inclinacion_pinza*PI)/180;
 
	alfa = (atan2(x,z)*180)/PI;
 	
	z_p = sqrt(pow(z,2)+pow(x,2));
 
	L = sqrt(pow(z_p,2)+pow(y,2));
 
	L_a = sqrt(pow(y+(Lp*sin(epsilon)),2)+pow(z_p-(Lp*cos(epsilon)),2));
 
	beta_p = atan2(y+(Lp*sin(epsilon)),z_p-(Lp*cos(epsilon)));
	
	beta_pp = atan2(y,z_p);
 
	beta_a = acos((pow(L1,2)+pow(L_a,2)-pow(L2,2))/(2*L1*L_a));
 
	beta = ((beta_p+beta_a)*180)/PI;
 
	gamma = acos((pow(L1,2)+pow(L2,2)-pow(L_a,2))/(2*L1*L2));
 
	delta_a = PI-(beta_a+gamma);
 
	gamma = (gamma*180)/PI;
 
        delta_p = acos((pow(L_a,2)+pow(Lp,2)-pow(L,2))/(2*L_a*Lp));       
 
	if (beta_pp > beta_p) {
		delta = ((2*PI-(delta_p-delta_a))*180)/PI;
	}
	else {
		delta = ((delta_p+delta_a)*180)/PI;
 
		if (isnan(delta)) {
			delta = ((PI+delta_a)*180)/PI;
		}
	}
 
 
	if (isnan(gamma))
	{
		coordenadas_correctas = 0; 
	}

	myrabot_arm_base_b::Servos posicion_servos_1;
	myrabot_arm_base_b::Servos velocidad_servos;
	myrabot_arm_base_b::Servos par_servos;	
	
	posicion_servos_1.base = ((alfa+150)*1023)/300;
	posicion_servos_1.arti1 = ((beta+60)*1023)/300;
	posicion_servos_1.arti2 = ((gamma-30)*1023)/300;
	posicion_servos_1.arti3 = ((delta-30)*1023)/300;	
	
	if (velocidad == 0)
	{
		velocidad_servos.base = abs(posicion_servos_1.base - posicion_servos_0.base)/5;
		if (velocidad_servos.base > 1023){velocidad_servos.base = 1023;}
		else if (velocidad_servos.base < 10){velocidad_servos.base = 10;}
		velocidad_servos.arti1 = abs(posicion_servos_1.arti1 - posicion_servos_0.arti1)/5;
		if (velocidad_servos.arti1 > 1023){velocidad_servos.arti1 = 1023;}
		else if (velocidad_servos.arti1 < 10){velocidad_servos.arti1 = 10;}
		velocidad_servos.arti2 = abs(posicion_servos_1.arti2 - posicion_servos_0.arti2)/5;
		if (velocidad_servos.arti2 > 1023){velocidad_servos.arti2 = 1023;}
		else if (velocidad_servos.arti2 < 10){velocidad_servos.arti2 = 10;}
		velocidad_servos.arti3 = abs(posicion_servos_1.arti3 - posicion_servos_0.arti3)/5;
		if (velocidad_servos.arti3 > 1023){velocidad_servos.arti3 = 1023;}
		else if (velocidad_servos.arti3 < 10){velocidad_servos.arti3 = 10;}
		
	}
	else
	{
		velocidad_servos.base = abs(posicion_servos_1.base - posicion_servos_0.base)*(velocidad/10);
		if (velocidad_servos.base > 1023){velocidad_servos.base = 1023;}
		else if (velocidad_servos.base < 10){velocidad_servos.base = 10;}
		velocidad_servos.arti1 = abs(posicion_servos_1.arti1 - posicion_servos_0.arti1)*(velocidad/10);
		if (velocidad_servos.arti1 > 1023){velocidad_servos.arti1 = 1023;}
		else if (velocidad_servos.arti1 < 10){velocidad_servos.arti1 = 10;}
		velocidad_servos.arti2 = abs(posicion_servos_1.arti2 - posicion_servos_0.arti2)*(velocidad/10);
		if (velocidad_servos.arti2 > 1023){velocidad_servos.arti2 = 1023;}
		else if (velocidad_servos.arti2 < 10){velocidad_servos.arti2 = 10;}
		velocidad_servos.arti3 = abs(posicion_servos_1.arti3 - posicion_servos_0.arti3)*(velocidad/10);
		if (velocidad_servos.arti3 > 1023){velocidad_servos.arti3 = 1023;}
		else if (velocidad_servos.arti3 < 10){velocidad_servos.arti3 = 10;}
		
	}
	
	myrabot_arm_base_b::Servos velocidad_servos_0;
	
	velocidad_servos_0.base = 0;
	velocidad_servos_0.arti1 = 0;
	velocidad_servos_0.arti2 = 0;
	velocidad_servos_0.arti3 = 0;
		
	par_servos.base = 1;
	par_servos.arti1 = 1;
	par_servos.arti2 = 1;
	par_servos.arti3 = 1;
	
	myrabot_arm_base_b::WriteServos move_arm;			
	
	if (coordenadas_correctas == 1 && (205 <= posicion_servos_1.base && posicion_servos_1.base <= 818) && (120 <= posicion_servos_1.arti1 && posicion_servos_1.arti1 <= 920) && posicion_servos_1.arti2 >= 50  && (posicion_servos_1.arti3 <= 828 && posicion_servos_1.arti3 >= 195)) {
		move_arm.posicion = posicion_servos_1;
		move_arm.velocidad = velocidad_servos;
		move_arm.par = par_servos;
		return move_arm;
	}
	else {
		std::cout<<"error coordenadas no validas o punto fuera del alcance"<<std::endl;
		move_arm.posicion = posicion_servos_0;
		move_arm.velocidad = velocidad_servos_0;
		move_arm.par = par_servos;
		return move_arm;				
	}	  
  }
  
myrabot_arm_base_b::WriteServos inversa(geometry_msgs::Point destino, int inclinacion_pinza, myrabot_arm_base_b::Servos posicion_servos_0, int velocidad, std::string elemento_final) 
  {
	  
	double x = destino.x;
  	double z = destino.y;
	double y = destino.z - z0;

	int coordenadas_correctas = 1;

	double alfa, alfa_p, beta, beta_a, beta_p, beta_pp, gamma, delta, delta_a, delta_p, epsilon;
	double z_p;
	double L_a, L;
	
	double Lf;
	double Dx, Dy;
	
	if (elemento_final == "dedo")
	{
		Lf = Ld;
		Dx = Dxd;
		Dy = Dyd;
	}
	else
	{
		Lf = Lp;
		Dx = Dxp;
		Dy = Dyp;
	}

	//correccion peso
	y = y + ((y/250) * 25);
 
	epsilon = (inclinacion_pinza*PI)/180;
 
	alfa_p = (atan2(x,z)*180)/PI;
	
 	x = x - Dx*cos(alfa_p);
 	
 	z = z - Dx*sin(alfa_p);
 	
 	alfa = alfa_p + ((atan2(x,z)*180)/PI);
 	
 	y = y - Dy;
 	
	z_p = sqrt(pow(z,2)+pow(x,2));
 
	L = sqrt(pow(z_p,2)+pow(y,2));
 
	L_a = sqrt(pow(y+(Lf*sin(epsilon)),2)+pow(z_p-(Lf*cos(epsilon)),2));
 
	beta_p = atan2(y+(Lf*sin(epsilon)),z_p-(Lf*cos(epsilon)));
	
	beta_pp = atan2(y,z_p);
 
	beta_a = acos((pow(L1,2)+pow(L_a,2)-pow(L2,2))/(2*L1*L_a));
 
	beta = ((beta_p+beta_a)*180)/PI;
 
	gamma = acos((pow(L1,2)+pow(L2,2)-pow(L_a,2))/(2*L1*L2));
 
	delta_a = PI-(beta_a+gamma);
 
	gamma = (gamma*180)/PI;
 
        delta_p = acos((pow(L_a,2)+pow(Lf,2)-pow(L,2))/(2*L_a*Lf));       
 
	if (beta_pp > beta_p) {
		delta = ((2*PI-(delta_p-delta_a))*180)/PI;
	}
	else {
		delta = ((delta_p+delta_a)*180)/PI;
 
		if (isnan(delta)) {
			delta = ((PI+delta_a)*180)/PI;
		}
	}
 
 
	if (isnan(gamma))
	{
		coordenadas_correctas = 0; 
	}

	myrabot_arm_base_b::Servos posicion_servos_1;
	myrabot_arm_base_b::Servos velocidad_servos;
	myrabot_arm_base_b::Servos par_servos;	
	
	posicion_servos_1.base = ((alfa+150)*1023)/300;
	posicion_servos_1.arti1 = ((beta+60)*1023)/300;
	posicion_servos_1.arti2 = ((gamma-30)*1023)/300;
	posicion_servos_1.arti3 = ((delta-30)*1023)/300;	
	
	if (velocidad == 0)
	{
		velocidad_servos.base = abs(posicion_servos_1.base - posicion_servos_0.base)/5;
		if (velocidad_servos.base > 1023){velocidad_servos.base = 1023;}
		else if (velocidad_servos.base < 10){velocidad_servos.base = 10;}
		velocidad_servos.arti1 = abs(posicion_servos_1.arti1 - posicion_servos_0.arti1)/5;
		if (velocidad_servos.arti1 > 1023){velocidad_servos.arti1 = 1023;}
		else if (velocidad_servos.arti1 < 10){velocidad_servos.arti1 = 10;}
		velocidad_servos.arti2 = abs(posicion_servos_1.arti2 - posicion_servos_0.arti2)/5;
		if (velocidad_servos.arti2 > 1023){velocidad_servos.arti2 = 1023;}
		else if (velocidad_servos.arti2 < 10){velocidad_servos.arti2 = 10;}
		velocidad_servos.arti3 = abs(posicion_servos_1.arti3 - posicion_servos_0.arti3)/5;
		if (velocidad_servos.arti3 > 1023){velocidad_servos.arti3 = 1023;}
		else if (velocidad_servos.arti3 < 10){velocidad_servos.arti3 = 10;}
		
	}
	else
	{
		velocidad_servos.base = abs(posicion_servos_1.base - posicion_servos_0.base)*(velocidad/10);
		if (velocidad_servos.base > 1023){velocidad_servos.base = 1023;}
		else if (velocidad_servos.base < 10){velocidad_servos.base = 10;}
		velocidad_servos.arti1 = abs(posicion_servos_1.arti1 - posicion_servos_0.arti1)*(velocidad/10);
		if (velocidad_servos.arti1 > 1023){velocidad_servos.arti1 = 1023;}
		else if (velocidad_servos.arti1 < 10){velocidad_servos.arti1 = 10;}
		velocidad_servos.arti2 = abs(posicion_servos_1.arti2 - posicion_servos_0.arti2)*(velocidad/10);
		if (velocidad_servos.arti2 > 1023){velocidad_servos.arti2 = 1023;}
		else if (velocidad_servos.arti2 < 10){velocidad_servos.arti2 = 10;}
		velocidad_servos.arti3 = abs(posicion_servos_1.arti3 - posicion_servos_0.arti3)*(velocidad/10);
		if (velocidad_servos.arti3 > 1023){velocidad_servos.arti3 = 1023;}
		else if (velocidad_servos.arti3 < 10){velocidad_servos.arti3 = 10;}
		
	}
	
	myrabot_arm_base_b::Servos velocidad_servos_0;
	
	velocidad_servos_0.base = 0;
	velocidad_servos_0.arti1 = 0;
	velocidad_servos_0.arti2 = 0;
	velocidad_servos_0.arti3 = 0;
		
	par_servos.base = 1;
	par_servos.arti1 = 1;
	par_servos.arti2 = 1;
	par_servos.arti3 = 1;
	
	myrabot_arm_base_b::WriteServos move_arm;			
	
	if (coordenadas_correctas == 1 && (205 <= posicion_servos_1.base && posicion_servos_1.base <= 818) && (120 <= posicion_servos_1.arti1 && posicion_servos_1.arti1 <= 920) && posicion_servos_1.arti2 >= 50  && (posicion_servos_1.arti3 <= 828 && posicion_servos_1.arti3 >= 195)) {
		move_arm.posicion = posicion_servos_1;
		move_arm.velocidad = velocidad_servos;
		move_arm.par = par_servos;
		return move_arm;
	}
	else {
		std::cout<<"error coordenadas no validas o punto fuera del alcance"<<std::endl;
		move_arm.posicion = posicion_servos_0;
		move_arm.velocidad = velocidad_servos_0;
		move_arm.par = par_servos;
		return move_arm;				
	}	  
  }
  
myrabot_arm_base_b::WriteServos controlPinza(myrabot_arm_base_b::Servos posicion_servos_1, myrabot_arm_base_b::Servos posicion_servos_0, myrabot_arm_base_b::Servos corriente_servos)
  {
	
	myrabot_arm_base_b::Servos velocidad_servos;	  
	myrabot_arm_base_b::Servos par_servos;
	
	velocidad_servos.pinza = 50;
	    
	par_servos.pinza = 1;
	
	myrabot_arm_base_b::WriteServos hand_arm;		
	
	if ((posicion_servos_1.pinza >= 480 && posicion_servos_1.pinza <= 680 && corriente_servos.pinza <= 300) || (posicion_servos_0.pinza > posicion_servos_1.pinza && posicion_servos_1.pinza >= 480))
	{
		hand_arm.posicion = posicion_servos_1;
		hand_arm.velocidad = velocidad_servos;
		hand_arm.par = par_servos;
		return hand_arm;
	}
	else
	{
		std::cout<<"Alcanzado límite de la pinza"<<std::endl;		
		hand_arm.posicion = posicion_servos_0;
		hand_arm.velocidad = velocidad_servos;
		hand_arm.par = par_servos;
		return hand_arm;
	}
  }
  
bool puntoAlcanzado(myrabot_arm_base_b::Servos posicion_servos_1, myrabot_arm_base_b::WriteServos posicion_servos_0, int precision)
  {
  	bool alcanzado = ((posicion_servos_1.base-precision) < posicion_servos_0.posicion.base && posicion_servos_0.posicion.base < (posicion_servos_1.base+precision)) && 
					 ((posicion_servos_1.arti1-precision) < posicion_servos_0.posicion.arti1 && posicion_servos_0.posicion.arti1 < (posicion_servos_1.arti1+precision)) && 
					 ((posicion_servos_1.arti2-precision) < posicion_servos_0.posicion.arti2 && posicion_servos_0.posicion.arti2 < (posicion_servos_1.arti2+precision)) && 
					 ((posicion_servos_1.arti3-precision) < posicion_servos_0.posicion.arti3 && posicion_servos_0.posicion.arti3 < (posicion_servos_1.arti3+precision));
  	    
  	return alcanzado;
  }
  
bool igualPosicion(myrabot_arm_base_b::Servos posicion_servos_1, myrabot_arm_base_b::WriteServos posicion_servos_0)
  {
  	bool igual = posicion_servos_1.base == posicion_servos_0.posicion.base && 
				 posicion_servos_1.arti1 == posicion_servos_0.posicion.arti1 && 
			     posicion_servos_1.arti2 == posicion_servos_0.posicion.arti2 &&  
				 posicion_servos_1.arti3 == posicion_servos_0.posicion.arti3;
  	    
  	return igual;
  }
  
struct desplazamiento 
{bool    en_posicion;
 double adelante_atras;
 double izquierda_derecha;
 double giro;};
  
desplazamiento dentroAlcance(geometry_msgs::Point destino, double orientacion, int inclinacion_pinza, myrabot_arm_base_b::Servos posicion_servos_0, int velocidad, std::string elemento_final)
  {				  	
  	myrabot_arm_base_b::WriteServos move;
  	
  	desplazamiento dentro_alcance;
				
	move = inversa(destino, inclinacion_pinza, posicion_servos_0, 0, elemento_final);
			
	if (!igualPosicion(posicion_servos_0, move) && std::abs(destino.x) <= max_lateral && std::abs(orientacion) <= 0.08)
	{
		dentro_alcance.en_posicion = true;
		
		dentro_alcance.adelante_atras = 0;
				
		dentro_alcance.izquierda_derecha = 0;
				
		dentro_alcance.giro = 0;
	}
	else
	{
		dentro_alcance.en_posicion = false;
		
		if (std::abs(orientacion) > 0.08)
		{
			dentro_alcance.giro = orientacion;
			
			double lateral = destino.y * sin(orientacion);
			
			if (std::abs(lateral) > max_lateral)
			{dentro_alcance.izquierda_derecha = -lateral;}
			else
			{dentro_alcance.izquierda_derecha = 0;}

			dentro_alcance.adelante_atras = 0;
		}
		else
		{
			dentro_alcance.giro = 0;
			
			if (destino.y > max_lejos)
			{
				dentro_alcance.adelante_atras = destino.y - max_lejos + ((max_lejos - max_cerca)/2);
				
				if (std::abs(destino.x) > max_lateral)
				{dentro_alcance.izquierda_derecha = destino.x;}
				else
				{dentro_alcance.izquierda_derecha = 0;}
			}
			else if (destino.y < max_cerca)
			{
				dentro_alcance.adelante_atras = destino.y - max_cerca - ((max_lejos - max_cerca)/2);
				
				if (std::abs(destino.x) > max_lateral)
				{dentro_alcance.izquierda_derecha = destino.x;}
				else
				{dentro_alcance.izquierda_derecha = 0;}		
			}
			else
			{
				dentro_alcance.adelante_atras = 0;
				
				if (std::abs(destino.x) > max_lateral)
				{dentro_alcance.izquierda_derecha = destino.x;}
				else
				{dentro_alcance.izquierda_derecha = 0;}
				
			}
		}
	}
	
	return dentro_alcance;
  }
  
geometry_msgs::Point home(myrabot_arm_base_b::Servos posicion_servos_0, myrabot_arm_base_b::Servos corriente_servos)
  { 
	  
  	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm" 	  
		
		geometry_msgs::Point punto_0;
		
		punto_0.x = 0;
		punto_0.z = 80 + z0;
		punto_0.y = 50;
		
		int inclinacion_pinza = 0;		
				
		myrabot_arm_base_b::WriteServos inicio_brazo = inversa(punto_0, inclinacion_pinza, posicion_servos_0, 0);
	
		myrabot_arm_base_b::Servos posicion_servos_1;
		
		posicion_servos_1.pinza = 511;
	
		myrabot_arm_base_b::WriteServos inicio_pinza = controlPinza(posicion_servos_1, posicion_servos_0, corriente_servos);
			
		
		if (!igualPosicion(posicion_servos_0, inicio_brazo))
		{
			move_pub_.publish(inicio_brazo);
		}
		
		if (posicion_servos_1.pinza != posicion_servos_0.pinza)
		{
			hand_pub_.publish(inicio_pinza);
		}
				
		return punto_0;

  }
  
geometry_msgs::Point home(myrabot_arm_base_b::Servos posicion_servos_0, myrabot_arm_base_b::Servos corriente_servos, std::string elemento_final)
  { 
	  
  	ros::NodeHandle n;
 
  	ros::Publisher move_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("move_arm", 1);   //Publicación del topic "move_arm"
  	
  	ros::Publisher hand_pub_=n.advertise<myrabot_arm_base_b::WriteServos>("hand_arm", 1);   //Publicación del topic "hand_arm" 	  
		
		geometry_msgs::Point punto_0;
		
		if (elemento_final == "dedo")
		{
			punto_0.x = 0;
			punto_0.z = 80 + z0;
			punto_0.y = 130;
		}
		else
		{
			punto_0.x = 0;
			punto_0.z = 80 + z0;
			punto_0.y = 50;
		}
		
		int inclinacion_pinza = 0;		
				
		myrabot_arm_base_b::WriteServos inicio_brazo = inversa(punto_0, inclinacion_pinza, posicion_servos_0, 0, elemento_final);
	
		myrabot_arm_base_b::Servos posicion_servos_1;
		
		posicion_servos_1.pinza = 511;
	
		myrabot_arm_base_b::WriteServos inicio_pinza = controlPinza(posicion_servos_1, posicion_servos_0, corriente_servos);
			
		if (!igualPosicion(posicion_servos_0, inicio_brazo))
		{
			move_pub_.publish(inicio_brazo);
		}
		
		if (posicion_servos_1.pinza != posicion_servos_0.pinza)
		{
			hand_pub_.publish(inicio_pinza);
		}
		
				
		return punto_0;

  }
  
myrabot_arm_base_b::WriteServosTrajectory trayectoria(geometry_msgs::Point origen, geometry_msgs::Point destino, int inclinacion_pinza, myrabot_arm_base_b::Servos posicion_servos_0, int velocidad, std::string elemento_final)
  { 
	double lambda = 0;
	
	myrabot_arm_base_b::Servos posicion_servos_i = posicion_servos_0;
	
	geometry_msgs::Point u, punto_i;
	
	//ecuación de la recta p1 = p0+lambda.u
	u.x = destino.x - origen.x;
	u.y = destino.y - origen.y;
	u.z = destino.z - origen.z;        
	
	int puntos = abs(sqrt(pow(u.x,2)+pow(u.y,2)+pow(u.z,2))/5);

	myrabot_arm_base_b::WriteServosTrajectory trayecto;
	
	trayecto.trayectoria = std::vector<myrabot_arm_base_b::WriteServos>(puntos);
	
	for (int i = 0; i < puntos; i++)
	{	
		lambda = lambda + 1/(double)puntos;
		
		punto_i.x = origen.x + (lambda)*u.x;
		punto_i.y = origen.y + (lambda)*u.y;
		punto_i.z = origen.z + (lambda)*u.z;
		
		trayecto.trayectoria[i] = inversa(punto_i, inclinacion_pinza, posicion_servos_i, 0, elemento_final);
		
		if (igualPosicion(posicion_servos_i, trayecto.trayectoria[i]))
		{
			trayecto.correcta = false;
			
			break;
		}
		
		myrabot_arm_base_b::Servos posicion_servos_i = trayecto.trayectoria[i].posicion;
		
		trayecto.correcta = true;
		
	}
	
	return trayecto;
	
  }
    
#endif
