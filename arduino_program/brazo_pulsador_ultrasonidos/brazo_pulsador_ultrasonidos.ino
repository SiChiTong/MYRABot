#include <ros.h>
#include <myrabot_arm_base/Servos.h>
#include <myrabot_arm_base/WriteServos.h>
#include <myrabot_arm_base/ReadServos.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Range.h>
#include <math.h>
 
#include <DynamixelSerial1.h>
 
ros::NodeHandle nh;
 
const int boton_pin = 8;
const int rele_pin = 7;

const int senSor_a = 32; 
const int senSor_b = 34;
const int senSor_c = 36;
const int senSor_d = 38;
const int senSor_e = 40;
 
const int control_a = 33;
const int control_b = 35;
const int control_c = 37;
const int control_d = 39;
const int control_e = 41;

int cont = 0;
int brazo_on = 0;

String control;
 
void mover(const myrabot_arm_base::WriteServos& brazo){
 
  myrabot_arm_base::Servos par = brazo.par;
 
  myrabot_arm_base::Servos vel = brazo.velocidad;
 
  myrabot_arm_base::Servos mover = brazo.posicion;
 
  if (par.base == 0){
    Dynamixel.torqueStatus(1,OFF);
  }
  else {
    Dynamixel.torqueStatus(1,ON);
 
    Dynamixel.moveSpeed(1,mover.base,vel.base);  
  }
 
  if (par.arti1 == 0){
    Dynamixel.torqueStatus(2,OFF);
  }
  else {
    Dynamixel.torqueStatus(2,ON);
 
    Dynamixel.moveSpeed(2,mover.arti1,vel.arti1);  
  }
 
  if (par.arti2 == 0){
    Dynamixel.torqueStatus(3,OFF);
  }
  else {
    Dynamixel.torqueStatus(3,ON);
 
    Dynamixel.moveSpeed(3,mover.arti2,vel.arti2);  
  }
 
  if (par.arti3 == 0){
    Dynamixel.torqueStatus(4,OFF);
  }
  else {
    Dynamixel.torqueStatus(4,ON);    
 
    Dynamixel.moveSpeed(4,mover.arti3,vel.arti3);  
  }
 
}
 
void pinza(const myrabot_arm_base::WriteServos& pinza){
 
  myrabot_arm_base::Servos par = pinza.par;
 
  myrabot_arm_base::Servos vel = pinza.velocidad;
 
  myrabot_arm_base::Servos mover = pinza.posicion;
 
  if (par.pinza == 0){
    Dynamixel.torqueStatus(5,OFF);
  }
  else {
    Dynamixel.torqueStatus(5,ON);    
 
    Dynamixel.moveSpeed(5,mover.pinza,vel.pinza);  
  }
 
}

void control_arduino(const std_msgs::String& control_arduino)
{ 
  
  control = control_arduino.data;
  
}

float ultrasonidos(const int x, const int y)
{  
  //digitalWrite(x, LOW);//HC-SR04 o RCW-0002
  //delayMicroseconds(2);//HC-SR04 o RCW-0002  
  digitalWrite(x, HIGH);
  delay (2);//MaxSonar
  //delayMicroseconds(10);//HC-SR04 o RCW-0002
  //digitalWrite(x, LOW);//HC-SR04 o RCW-0002
  unsigned long pulse = pulseIn(y, HIGH, 50000);
  float pulse_1 = pulse;
  float distance = (pulse_1/2000000)*343;
  //if (distance <= 0.09)//HC-SR04 o RCW-0002 
  //{
    //distance = 5;
  //}  
  digitalWrite(x, LOW);//MaxSonar
  return distance; 
}
 
ros::Subscriber<myrabot_arm_base::WriteServos> move_sub("move_arm", &mover );
ros::Subscriber<myrabot_arm_base::WriteServos> hand_sub("hand_arm", &pinza );
ros::Subscriber<std_msgs::String> arduino_control_sub("arduino_control", &control_arduino ); 
 

std_msgs::Bool pulsador;
sensor_msgs::Range range_msg;
myrabot_arm_base::ReadServos pec;

ros::Publisher pose_pub("pose_arm", &pec);
ros::Publisher button_pub("button", &pulsador);
ros::Publisher pub_range1( "/ultrasonidos_1", &range_msg);
ros::Publisher pub_range2( "/ultrasonidos_2", &range_msg);
ros::Publisher pub_range3( "/ultrasonidos_3", &range_msg);
ros::Publisher pub_range4( "/ultrasonidos_4", &range_msg);
ros::Publisher pub_range5( "/ultrasonidos_5", &range_msg);
 
void setup()
{
  nh.initNode();
  nh.subscribe(move_sub);
  nh.subscribe(hand_sub);
  nh.subscribe(arduino_control_sub); 
  
  nh.advertise(pose_pub); 
  
  nh.advertise(button_pub);
  pinMode(boton_pin, INPUT);
  digitalWrite(boton_pin, HIGH);
  
  pinMode(rele_pin, OUTPUT); 

  pinMode(senSor_a, INPUT);
  pinMode(senSor_b, INPUT);
  pinMode(senSor_c, INPUT);
  pinMode(senSor_d, INPUT);
  pinMode(senSor_e, INPUT);   
 
  pinMode(control_a, OUTPUT);
  pinMode(control_b, OUTPUT);
  pinMode(control_c, OUTPUT);
  pinMode(control_d, OUTPUT);  
  pinMode(control_e, OUTPUT);  

  nh.advertise(pub_range1);
  nh.advertise(pub_range2);
  nh.advertise(pub_range3);
  nh.advertise(pub_range4);
  nh.advertise(pub_range5);  
 
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.field_of_view = 0.56;
  range_msg.min_range = 0.2;
  range_msg.max_range = 7; 
  
  Dynamixel.begin(1000000,2);
  delay(1000);
}
 
void loop()
{
  
  if ((digitalRead(boton_pin) == false || (control == "brazo" && brazo_on == 0)) && cont == 0 ){
        digitalWrite(rele_pin, HIGH);
        cont = 1;
        brazo_on = 1;
        
	pulsador.data = true;
	button_pub.publish(&pulsador);        
        delay(200);        
  }
  else if (digitalRead(boton_pin) == false || (control == "ultrasonidos" && brazo_on == 1) {
        digitalWrite(rele_pin, LOW);
        cont = 0;
        
	pulsador.data = false;
	button_pub.publish(&pulsador);        
        delay(200);
  }  
  
  if (control == "brazo")
  {
    
    myrabot_arm_base::Servos pos;
    myrabot_arm_base::Servos est;
    myrabot_arm_base::Servos cor; 
   
    pos.base = Dynamixel.readPosition(1);
    pos.arti1 = Dynamixel.readPosition(2);
    pos.arti2 = Dynamixel.readPosition(3);
    pos.arti3 = Dynamixel.readPosition(4);
    pos.pinza = Dynamixel.readPosition(5);  
    
    est.base = Dynamixel.moving(1);
    est.arti1 = Dynamixel.moving(2);
    est.arti2 = Dynamixel.moving(3);
    est.arti3 = Dynamixel.moving(4);
    est.pinza = Dynamixel.moving(5);     
   
    cor.base = Dynamixel.readLoad(1);
    cor.arti1 = Dynamixel.readLoad(2);
    cor.arti2 = Dynamixel.readLoad(3);
    cor.arti3 = Dynamixel.readLoad(4);
    cor.pinza = Dynamixel.readLoad(5);
  
    pec.posicion = pos;
    pec.estado = est;
    pec.corriente = cor; 
    
    pose_pub.publish( &pec );
 
	delay(20);
  
  } 
  else if (control == "ultrasonidos")
  {
    range_msg.range = ultrasonidos(control_a, senSor_a);
      range_msg.header.stamp = nh.now();
      range_msg.header.frame_id =  "ultrasonidos_1_link";    
      pub_range1.publish(&range_msg);
    delay (20);// necessario to avoid crostalking 
   
    range_msg.range = ultrasonidos(control_b, senSor_b); 
      range_msg.header.stamp = nh.now();
      range_msg.header.frame_id =  "ultrasonidos_2_link";    
      pub_range2.publish(&range_msg);
    delay (20);
   
    range_msg.range = ultrasonidos(control_c, senSor_c);
      range_msg.header.stamp = nh.now();
      range_msg.header.frame_id =  "ultrasonidos_3_link";    
      pub_range3.publish(&range_msg);
    delay (20);
    
    range_msg.range = ultrasonidos(control_d, senSor_d); 
      range_msg.header.stamp = nh.now();
      range_msg.header.frame_id =  "ultrasonidos_4_link";    
      pub_range4.publish(&range_msg);
    delay (20);
  
    range_msg.range = ultrasonidos(control_e, senSor_e);  
      range_msg.header.stamp = nh.now();
      range_msg.header.frame_id =  "ultrasonidos_5_link";    
      pub_range5.publish(&range_msg);
    delay (20);
  }
  
  nh.spinOnce();

}
