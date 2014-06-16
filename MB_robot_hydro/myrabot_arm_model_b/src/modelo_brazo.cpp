  #include "ros/ros.h"
  #include "myrabot_arm_base_b/Servos.h"
  #include "myrabot_arm_base_b/WriteServos.h"
  #include "myrabot_arm_base_b/ReadServos.h"
  #include "std_msgs/Float64.h"
  #include "sensor_msgs/JointState.h"
  #include "math.h"
 
  #define PI 3.14159265
 
 
  void mover(const myrabot_arm_base_b::WriteServos& move)
  {    
        ros::NodeHandle n;
 
        ros::Publisher base_pos_pub_=n.advertise<std_msgs::Float64>("brazo/base_pos_controller/command", 1);  
        ros::Publisher arti1_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti1_pos_controller/command", 1);
        ros::Publisher arti2_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti2_pos_controller/command", 1);           
        ros::Publisher arti3_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti3_pos_controller/command", 1);
       
        ros::Publisher base_vel_pub_=n.advertise<std_msgs::Float64>("brazo/base_vel_controller/command", 1);  
        ros::Publisher arti1_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti1_vel_controller/command", 1);
        ros::Publisher arti2_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti2_vel_controller/command", 1);           
        ros::Publisher arti3_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti3_vel_controller/command", 1);        
         
        myrabot_arm_base_b::Servos position = move.posicion;
        myrabot_arm_base_b::Servos speed = move.velocidad;
        myrabot_arm_base_b::Servos torque = move.par;
       
        std_msgs::Float64 base_pos_command, arti1_pos_command, arti2_pos_command, arti3_pos_command;
        std_msgs::Float64 base_vel_command, arti1_vel_command, arti2_vel_command, arti3_vel_command;   
       
        if (torque.base != 0 && (position.base >= 0 && position.base <= 1023) && (speed.base >= 0 && speed.base <= 1023))
        {              
                base_pos_command.data = ((((position.base * 300)/1023)*PI)/180)-2.62;
                base_vel_command.data = speed.base/511;
        }
        if (torque.arti1 != 0 && (position.arti1 >= 0 && position.arti1 <= 1023) && (speed.arti1 >= 0 && speed.arti1 <= 1023))
        {
                arti1_pos_command.data = ((((position.arti1*300)/1023)*PI)/180)-2.62;
                arti1_vel_command.data = speed.arti1/511;              
        }
        if (torque.arti2 != 0 && (position.arti2 >= 0 && position.arti2 <= 1023) && (speed.arti2 >= 0 && speed.arti2 <= 1023))
        {
                arti2_pos_command.data = ((((position.arti2*300)/1023)*PI)/180)-2.62;
                arti2_vel_command.data = speed.arti2/511;              
        }
        if (torque.arti3 != 0 && (position.arti3 >= 0 && position.arti3 <= 1023) && (speed.arti3 >= 0 && speed.arti3 <= 1023))
        {
                arti3_pos_command.data = ((((position.arti3*300)/1023)*PI)/180)-2.62;
                arti3_vel_command.data = speed.arti3/511;              
        }
       
       
        base_pos_pub_.publish(base_pos_command);
        arti1_pos_pub_.publish(arti1_pos_command);
        arti2_pos_pub_.publish(arti2_pos_command);
        arti3_pos_pub_.publish(arti3_pos_command);

        base_vel_pub_.publish(base_vel_command);
        arti1_vel_pub_.publish(arti1_vel_command);
        arti2_vel_pub_.publish(arti2_vel_command);
        arti3_vel_pub_.publish(arti3_vel_command);                                     
 
  }
  void pinza(const myrabot_arm_base_b::WriteServos& pinza)
  {      
        ros::NodeHandle n;
 
        ros::Publisher pinza_pos_pub_=n.advertise<std_msgs::Float64>("brazo/pinza_pos_controller/command", 1);
           
        ros::Publisher pinza_vel_pub_=n.advertise<std_msgs::Float64>("brazo/pinza_vel_controller/command", 1);                
         
        myrabot_arm_base_b::Servos position = pinza.posicion;
        myrabot_arm_base_b::Servos speed = pinza.velocidad;
        myrabot_arm_base_b::Servos torque = pinza.par;
       
        std_msgs::Float64 pinza_pos_command;
        std_msgs::Float64 pinza_vel_command;   
       
        if (torque.pinza != 0 && (position.pinza >= 0 && position.pinza <= 1023) && (speed.pinza >= 0 && speed.pinza <= 1023))
        {
                pinza_pos_command.data = ((((position.pinza*300)/1023)*PI)/180)-2.62;
                pinza_vel_command.data = speed.pinza/511;              
        }
       
        pinza_pos_pub_.publish(pinza_pos_command);
        pinza_vel_pub_.publish(pinza_vel_command);     
               
  }
  void pe(const sensor_msgs::JointState& pe)
  {
        ros::NodeHandle n;       
         
        ros::Publisher pose_pub_=n.advertise<myrabot_arm_base_b::ReadServos>("pose_arm", 1);  
         
        myrabot_arm_base_b::ReadServos estado;
         
        estado.posicion.base = ((((pe.position[3]+2.62)*180)/PI)*1023)/300;
        estado.posicion.arti1 = ((((pe.position[0]+2.62)*180)/PI)*1023)/300;
        estado.posicion.arti2 = ((((pe.position[1]+2.62)*180)/PI)*1023)/300;
        estado.posicion.arti3 = ((((pe.position[2]+2.62)*180)/PI)*1023)/300;
        estado.posicion.pinza = ((((pe.position[4]+2.62)*180)/PI)*1023)/300;
       
        estado.corriente.base = pe.effort[3]*1000;
        estado.corriente.arti1 = pe.effort[0]*1000;
        estado.corriente.arti2 = pe.effort[1]*1000;
        estado.corriente.arti3 = pe.effort[2]*1000;
        estado.corriente.pinza = pe.effort[4]*1000;
       
        pose_pub_.publish(estado);                             
       
  }      
  int main(int argc, char **argv)
  {
         
        ros::init(argc, argv, "modelo_brazo");  
 
        ros::NodeHandle n;
 
        ros::Subscriber move_sub_= n.subscribe("move_arm", 1, mover);
        ros::Subscriber hand_sub_= n.subscribe("hand_arm", 1, pinza);
        ros::Subscriber pe_sub_= n.subscribe("brazo/joint_states", 1, pe);
                         
 
        ros::Publisher base_pos_pub_=n.advertise<std_msgs::Float64>("brazo/base_pos_controller/command", 1);  
        ros::Publisher arti1_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti1_pos_controller/command", 1);
        ros::Publisher arti2_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti2_pos_controller/command", 1);           
        ros::Publisher arti3_pos_pub_=n.advertise<std_msgs::Float64>("brazo/arti3_pos_controller/command", 1);
        ros::Publisher pinza_pos_pub_=n.advertise<std_msgs::Float64>("brazo/pinza_pos_controller/command", 1);
               
        ros::Publisher base_vel_pub_=n.advertise<std_msgs::Float64>("brazo/base_vel_controller/command", 1);  
        ros::Publisher arti1_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti1_vel_controller/command", 1);
        ros::Publisher arti2_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti2_vel_controller/command", 1);           
        ros::Publisher arti3_vel_pub_=n.advertise<std_msgs::Float64>("brazo/arti3_vel_controller/command", 1);  
        ros::Publisher pinza_vel_pub_=n.advertise<std_msgs::Float64>("brazo/pinza_vel_controller/command", 1);
       
        ros::Publisher pose_pub_=n.advertise<myrabot_arm_base_b::ReadServos>("pose_arm", 1);              
       
        ros::spin();  
 
        return 0;
  }
