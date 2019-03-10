#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib_tutorials/FibonacciAction.h>

using namespace actionlib_tutorials;
typedef actionlib::SimpleActionClient<FibonacciAction> Client;
int j=0;
float flag;
// Called once when the goal completes
void doneCb(const actionlib::SimpleClientGoalState& state,
            const FibonacciResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  //ROS_INFO("Answer: %f", result->sequence);

if(result->rank==1)
	{
	  ros::shutdown();
	}
}

// Called once when the goal becomes active
void activeCb()
   {
     ROS_INFO("Goal just went active");
   }

// Called every time feedback is received for the goal
void feedbackCb(const FibonacciFeedbackConstPtr& feedback)
{
  ROS_INFO("Got Feedback %f", feedback->sequence);

  flag=feedback->sequence;
}

void sudip(){


Client ac("fibonacci", true);

    ROS_INFO("Waiting for action server to start.");
    ac.waitForServer();
    ROS_INFO("Action server started, sending goal.");

    // Send Goal
    FibonacciGoal goal;
    //goal.order = 20;
  if(j==0)
    {
      std::cout<<"please give the initial guess"<<std::endl;

      std::cin>>goal.order;
      j++;
    }
  else
      {
        goal.order=flag;
      }
    ac.sendGoal(goal, &doneCb,&activeCb ,&feedbackCb);
	sudip();
  //goto input;
}




int main (int argc, char **argv)
{
 //input:
  
    ros::init(argc, argv, "test_fibonacci_callback");

    // Create the action client
    sudip();

  ros::spin();
  return 0;
}

