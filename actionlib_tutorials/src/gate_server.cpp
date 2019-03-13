    #include <ros/ros.h>
    #include <actionlib/server/simple_action_server.h>
    #include <actionlib_tutorials/GateAction.h>
    
    class GateAction
    {
    protected:
    
      ros::NodeHandle nh_;
     actionlib::SimpleActionServer<actionlib_tutorials::GateAction> as_; 
     std::string action_name_;
    
     actionlib_tutorials::GateFeedback feedback_;
     actionlib_tutorials::GateResult result_;
   
   public:
  
     GateAction(std::string name) :
       as_(nh_, name, boost::bind(&GateAction::executeCB, this, _1), false),
      action_name_(name)
     {
       as_.start();
     }
   
     ~GateAction(void)
     {
     }
   
     void executeCB(const actionlib_tutorials::GateGoalConstPtr &goal)
     {
       // helper variables
      ros::Rate r(1);
       bool success = true;

    feedback_.flag.clear();
      
       {
         
         if (as_.isPreemptRequested() || !ros::ok())
         {
           ROS_INFO("%s: Preempted", action_name_.c_str());
          
          as_.setPreempted();
           success = false;
           return;
         }
         
	
	if (goal->x_center>goal->x_cordinate)
	 {      
          feedback_.flag.push_back("Go Right");
          result_.name = "Go Right" ;
  	  }
        
        else {
              feedback_.flag.push_back("Go Left");
              result_.name = "Go Left" ;
              }
    

      

          
          if (goal->y_center>goal->y_cordinate )
           {
            feedback_.flag.push_back("Go up");
            result_.name = "Go up" ;
            }
          else {
            feedback_.flag.push_back("Go down");
            result_.name = "Go down" ;
           }
}

	

	ROS_INFO("go toward %f ,%f", goal->x_cordinate,goal->y_cordinate);
         
         as_.publishFeedback(feedback_);
         
         //r.sleep();
	
         
         // set the action state to succeeded
         as_.setSucceeded(result_);
       
       }
   
      // if(success)
       
	   
	     
     };
   
   
   
   
   
   int main(int argc, char** argv)
   {
     ros::init(argc, argv, "gate");
   
     GateAction gate("gate");
     ros::spin();
   
     return 0;
   }
