#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <action_home5/azioniAction.h>

int main (int argc, char **argv)
{
  ros::init(argc, argv, "client");


  actionlib::SimpleActionClient<action_home5::azioniAction> ac("server", true);

  
  ac.waitForServer(); 

  
  
   action_home5::azioniGoal goal;
  goal.goal_d = 1.0;
  goal.goal_s = 30;
 
  ac.sendGoal(goal);
  
   
  

 
  bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  
  return 0;
}
