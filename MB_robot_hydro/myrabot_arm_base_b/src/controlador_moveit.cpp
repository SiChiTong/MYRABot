#include <ros/ros.h>
#include <ros/time.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <myrabot_arm_base_b/WriteServos.h>
#include <myrabot_arm_base_b/ReadServos.h>
#include <actionlib/server/simple_action_server.h>
#include <control_msgs/FollowJointTrajectoryAction.h>

#define PI 3.14159265

class MYRAbotArm
{
public:

  MYRAbotArm(std::string name) :
    as_(nh_, name, false),
    action_name_(name)
  {
    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&MYRAbotArm::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&MYRAbotArm::preemptCB, this));

    //subscribe to the data topic of interest
    sub_pose_arm_ = nh_.subscribe("/pose_arm", 1, &MYRAbotArm::analysisCB, this);

    //publish the data topic of interest
    pub_move_arm_ = nh_.advertise<myrabot_arm_base_b::WriteServos>("/move_arm", 1, this);

    as_.start();
  }

  ~MYRAbotArm(void)
  {
  }

  void goalCB()
  {
	i_ = 0;

	goal_ = as_.acceptNewGoal()->trajectory;
  }

  void preemptCB()
  {
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
  }

  void analysisCB(const myrabot_arm_base_b::ReadServos& arm)
  {
    // make sure that the action hasn't been canceled
    if (!as_.isActive())
      return;

    ros::Time ahora = ros::Time::now();

    feedback_.header.stamp = ahora;

    feedback_.joint_names.resize(4);
    feedback_.actual.positions.resize(4);
    feedback_.actual.effort.resize(4);

	for (int i = 0; i < 4; i++)
	{
		switch(i)
		{
			case 3:
				feedback_.joint_names[i] = "base_brazo";
				feedback_.actual.positions[i] = ((((arm.posicion.base * 300)/1023)*PI)/180)-2.62;
				feedback_.actual.effort[i] = ((((arm.corriente.base * 300)/1023)*PI)/180)-2.62;
			break;
			case 0:
				feedback_.joint_names[i] = "arti1";
				feedback_.actual.positions[i] = ((((arm.posicion.arti1 * 300)/1023)*PI)/180)-2.62;
				feedback_.actual.effort[i] = ((((arm.corriente.arti1 * 300)/1023)*PI)/180)-2.62;
			break;
			case 1:
				feedback_.joint_names[i] = "arti2";
				feedback_.actual.positions[i] = ((((arm.posicion.arti2 * 300)/1023)*PI)/180)-2.62;
				feedback_.actual.effort[i] = ((((arm.corriente.arti2 * 300)/1023)*PI)/180)-2.62;
			break;
			case 2:
				feedback_.joint_names[i] = "arti3";
				feedback_.actual.positions[i] = ((((arm.posicion.arti3 * 300)/1023)*PI)/180)-2.62;
				feedback_.actual.effort[i] = ((((arm.corriente.arti3 * 300)/1023)*PI)/180)-2.62;
			break;
		}
	}

	puntos_ = goal_.points.size();

	std::cout<<i_<<std::endl;

	if (i_ != puntos_)
	{
	    		std::cout<<puntos_<<std::endl;

	    		myrabot_arm_base_b::WriteServos move;

	    			if (i_ == 0
	    			   ||((feedback_.actual.positions[0] <= (goal_.points[i_-1].positions[0] + 0.1) && feedback_.actual.positions[0] >= (goal_.points[i_-1].positions[0] - 0.1))
	    			   && (feedback_.actual.positions[1] <= (goal_.points[i_-1].positions[1] + 0.1) && feedback_.actual.positions[1] >= (goal_.points[i_-1].positions[1] - 0.1))
	    			   && (feedback_.actual.positions[2] <= (goal_.points[i_-1].positions[2] + 0.1) && feedback_.actual.positions[2] >= (goal_.points[i_-1].positions[2] - 0.1))
	    			   && (feedback_.actual.positions[3] <= (goal_.points[i_-1].positions[3] + 0.1) && feedback_.actual.positions[3] >= (goal_.points[i_-1].positions[3] - 0.1))))
	    			{
	    				for (int j = 0; j < 4; j++)
	    				{
	    					switch (j)
	    					{
	    					case 3:
	    						move.posicion.base = ((((goal_.points[i_].positions[j]+2.62)*180)/PI)*1023)/300;
	    						move.velocidad.base = abs(goal_.points[i_].velocities[j]*511);
	    						move.par.base = 1;
	    					break;
	    					case 0:
	    						move.posicion.arti1 = ((((goal_.points[i_].positions[j]+2.62)*180)/PI)*1023)/300;
	    						move.velocidad.arti1 = abs(goal_.points[i_].velocities[j]*511);
	    						move.par.arti1 = 1;
	    					break;
	    					case 1:
	    						move.posicion.arti2 = ((((goal_.points[i_].positions[j]+2.62)*180)/PI)*1023)/300;
	    						move.velocidad.arti2 = abs(goal_.points[i_].velocities[j]*511);
	    						move.par.arti2 = 1;
	    					break;
	    					case 2:
	    						move.posicion.arti3 = ((((goal_.points[i_].positions[j]+2.62)*180)/PI)*1023)/300;
	    						move.velocidad.arti3 = abs(goal_.points[i_].velocities[j]*511);
	    						move.par.arti3 = 1;
	    					break;
	    					}
	    				}

	    				std::cout<<move<<std::endl;
	    				pub_move_arm_.publish(move);

	    				t_inicio = ros::Time::now();
	    			}
	    			else
	    			{
	    				std::cout<<i_<<std::endl;
	    				i_ = i_ - 1;
	    			}

	    			ros::Time ahora = ros::Time::now();

	    			ros::Duration duracion(1.0);

	    			if (ahora > (t_inicio + duracion))
	    			{
	    				result_.error_code = -1;
	    		        as_.setAborted(result_);
	    			}

				feedback_.desired = goal_.points[i_];

				feedback_.error.positions.resize(4);

				std::cout<<feedback_.desired.positions[0]<<" menos "<<feedback_.actual.positions[0]<<std::endl;
				std::cout<<feedback_.desired.positions[1]<<" menos "<<feedback_.actual.positions[1]<<std::endl;
				std::cout<<feedback_.desired.positions[2]<<" menos "<<feedback_.actual.positions[2]<<std::endl;
				std::cout<<feedback_.desired.positions[3]<<" menos "<<feedback_.actual.positions[3]<<std::endl;

				for (int j = 0; j < 4; j++)
				{
					feedback_.error.positions[j] = feedback_.desired.positions[j] - feedback_.actual.positions[j];
				}

				i_ ++;

    			std::cout<<feedback_.actual.positions[0]<<"se parece a"<<goal_.points[i_-1].positions[0]<<std::endl;
    			std::cout<<feedback_.actual.positions[1]<<"se parece a"<<goal_.points[i_-1].positions[1]<<std::endl;
    			std::cout<<feedback_.actual.positions[2]<<"se parece a"<<goal_.points[i_-1].positions[2]<<std::endl;
    			std::cout<<feedback_.actual.positions[3]<<"se parece a"<<goal_.points[i_-1].positions[3]<<std::endl;

    			std::cout<<"teta"<<i_<<std::endl;
  	}
	else
	{
		result_.error_code = 0;
	    as_.setSucceeded(result_);
	}
	as_.publishFeedback(feedback_);

  }

protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<control_msgs::FollowJointTrajectoryAction> as_;
  std::string action_name_;
  int puntos_, i_;
  ros::Time t_inicio;
  trajectory_msgs::JointTrajectory goal_;
  control_msgs::FollowJointTrajectoryResult result_;
  control_msgs::FollowJointTrajectoryFeedback feedback_;
  ros::Subscriber sub_pose_arm_;
  ros::Publisher pub_move_arm_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "brazo_controller/follow_join_trajectory");

  MYRAbotArm brazo(ros::this_node::getName());
  ros::spin();

  return 0;
}
