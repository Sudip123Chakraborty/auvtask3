#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <actionlib_tutorials/GateAction.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>
#include "opencv2/videoio/videoio.hpp"
//#include "opencv2/video.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/video/video.hpp"
  
	
using namespace cv;
using namespace std;
Mat img,frame,src;
void contour(Mat image);
int main(int argc ,char  **argv)
{
  ros::init(argc, argv, "test_gate");

  
   actionlib::SimpleActionClient<actionlib_tutorials::GateAction> ac("gate", true);

	  ROS_INFO("Waiting for action server to start.");
	  
	  ac.waitForServer(); 

	  ROS_INFO("Action server started, sending goal.");
	  
	  
  
/**********************************************************************************************************/
  int i,j;
  VideoCapture Video("/home/sudip/Desktop/winterworkshop/auv.avi");// capturing the video
  //Mat frame;
  Mat RGB,bw,hsv;
  while(1)
  {

      Video>>src;

      Mat binary(RGB.rows,RGB.cols,CV_8UC1,Scalar(0));
      Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);




      Mat fullImageHSV;
      cvtColor(src,RGB, CV_BGR2RGB);

      cvtColor(RGB,bw, CV_RGB2GRAY);//changing rgb to grayscale

      cvtColor(RGB,hsv, CV_RGB2HSV);

      threshold(bw,binary,10,255, THRESH_BINARY_INV);

      for(i=0;i<RGB.rows;i++)
      {
          for(j=0;j<RGB.cols;j++)
          {
            if((bw.at<uchar>(i,j)<40))
            binary.at<uchar>(i,j)=0;

            else binary.at<uchar>(i,j)=255;
          }
      }
/*********ERROSION for smoothing the LINES && DILATION for joining the holes*******************/
        erode(binary, binary, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );//errosion for clearing noise 
        dilate(binary, binary, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );//& diletioin for joining small gaps

         
        dilate( binary, binary, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
        erode(binary, binary, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );




      /*namedWindow("source",WINDOW_NORMAL);
      namedWindow("RGB image",WINDOW_NORMAL);
      namedWindow("HSV image",WINDOW_NORMAL);
      namedWindow("Binary Image",WINDOW_NORMAL);*/
       
      //cout<<bw.rows<<endl<<bw.cols<<endl<<"sudip"<<endl;
       Mat dst1, cdst;
       Canny(binary, dst1, 10, 255, 3);
       cvtColor(dst1, cdst, CV_GRAY2BGR);
   

	  ROS_INFO("Waiting for action server to start.");
	  
	  ac.waitForServer(); 

	  ROS_INFO("Action server started, sending goal.");
	  
	  actionlib_tutorials::GateGoal goal;
	   
	     Canny(gray, gray, 10, 40, 3);
	/******************CONTOUR DETECTION*********************/
	     vector<vector<Point> > contours;
	     vector<Vec4i> hierarchy;
	     RNG rng(12345);
	     findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );





    for (unsigned int i = 0;  i < contours.size();  i++)
    {
     


     if((contourArea(contours[i])>950))
     	{
      
         ////////////////////
          	 vector<Moments> mu(contours.size() );
            	 for( int i = 0; i < contours.size(); i++ )
               	 mu[i] = moments( contours[i], false ); 

      /*************** calculation of the center of the gate*******************/
            	vector<Point2f> mc( contours.size() );
            	for( int i = 0; i < contours.size(); i++ )
		       { 
		          mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		  	     }

             

             if(mc[i].x>200&&mc[i].y<330&&mc[i].x<510)
             {
                Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                drawContours( src, contours, i, color, 2, 8, hierarchy, 0, Point() );

                //cout<<"Center = "<<mc[i].x << ","<< mc[i].y<<endl;

		goal.x_cordinate=mc[i].x;
		goal.y_cordinate=mc[i].y;
		goal.x_center=src.rows;
		goal.y_center=src.cols;
		ac.sendGoal(goal);
		

               
             }





      }

	
    }


        vector<Vec4i> lines;
        HoughLinesP(dst1, lines, 1, CV_PI/180, 50, 50, 10);
        for( size_t i = 0; i < lines.size(); i++ )
        {
          Vec4i l = lines[i];
          line( src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
       }

      waitKey(1);

      }





/*****************************************************************************************************************/
 
bool finished_before_timeout = ac.waitForResult(ros::Duration(500));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  
  
	  
	  actionlib_tutorials::GateGoal goal;
  
  return 0;
}


/*******************************************************************************************************************/









