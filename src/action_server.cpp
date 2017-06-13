#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <action_home5/azioniAction.h>
#include <math.h>



typedef actionlib::SimpleActionServer<action_home5::azioniAction> Server;//typedef del server
                                                                        //variabili globali usate dalla funzione execute per monitorare la posizione e il  successo del goal
  bool successo=false;// flag per successo
  bool primo=false;// flag per prima chiamata a call_back odom per settare x e y
  float vel;
  float dis;
  float x_start;//x iniziale settata alla prima call_back di odom
  float y_start;// y iniziale 
  bool puoi=false; 
  ros::Publisher share;
  
void call_back_odom(const nav_msgs::Odometry::ConstPtr& msg){//callback della odometria
	    
		float current_x = msg->pose.pose.position.x;
		float current_y= msg->pose.pose.position.y;
		if(!puoi)return;
		if(!primo) {
			x_start = current_x;
			y_start = current_y;
			primo = true;
		}
		
			if(sqrt(pow(current_x-(x_start),2) + pow(current_y-(y_start),2) >= (dis))) {
				successo = true;
				
			}
		
	
}

void execute(const action_home5::azioniGoalConstPtr& goal, Server* as)//funzione eseguita dal server per il goal
{
                
               
vel=goal->goal_s;
dis=goal->goal_d;
  
  geometry_msgs::Twist m;
  
 
  
  puoi=true;
  while(!successo){// continuo a publicare su cmd_vel fino a quando non arrivo 
	  m.linear.x = vel;

	  share.publish(m);
  }
  m.linear.x=0;
  share.publish(m);
  puoi=false;
  successo=false;
  primo=false;
  as->setSucceeded();
}

int main(int argc, char** argv)
{
	
  ros::init(argc, argv, "server");
  ros::NodeHandle n;
  Server server(n, "server", boost::bind(&execute, _1, &server), false);
  
  ros::Subscriber sub=n.subscribe("odom",10,call_back_odom);
  
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
  share=pub;
  server.start();
  
  ros::spin();
  return 0;
}
