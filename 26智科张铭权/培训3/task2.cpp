#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("../视频/video.mp4");
    if(!cap.isOpened())
    {
        cout<<"视频无法打开"<<endl;
        return -1;
    }
    Mat frame;
    while(cap.read(frame))
    {
        Mat hsv;
        cvtColor(frame,hsv,COLOR_BGR2HSV);
        Mat mask1,mask2,red_mask;
        Scalar lower_red1=Scalar(0,120,70);
        Scalar upper_red1=Scalar(10,255,255);
        Scalar lower_red2=Scalar(170,120,70);
        Scalar upper_red2=Scalar(180,255,255);
        inRange(hsv,lower_red1,upper_red1,mask1);
        inRange(hsv,lower_red2,upper_red2,mask2);
        red_mask = mask1 | mask2;

        vector<vector<Point>>contours;
        vector<Vec4i>hierarchy;
        findContours(red_mask,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
        vector<RotatedRect> lights;

        for(int i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if(area < 30)
            {
                continue;
            }
            RotatedRect r = minAreaRect(contours[i]);
            lights.push_back(r);
            
            Point2f pts[4];
            r.points(pts);
            for(int j=0;j<4;j++)
            {
                line(frame, pts[j], pts[(j+1)%4], Scalar(0,255,255), 2);
            }
        }

        if(lights.size() == 2)
        {
            RotatedRect leftLight, rightLight;
            if(lights[0].center.x < lights[1].center.x)
            {
                leftLight = lights[0];
                rightLight = lights[1];
            }
            else
            {
                leftLight = lights[1];
                rightLight = lights[0];
            }

            Point2f l_pts[4];
            leftLight.points(l_pts);
            Point2f r_pts[4];
            rightLight.points(r_pts);

            
            Point2f l_out1 = l_pts[0], l_out2 = l_pts[1];
            if(l_out1.x > l_out2.x) swap(l_out1, l_out2);
            for(int k=2;k<4;k++)
            {
                if(l_pts[k].x < l_out1.x)
                {
                    l_out2 = l_out1;
                    l_out1 = l_pts[k];
                }
                else if(l_pts[k].x < l_out2.x)
                {
                    l_out2 = l_pts[k];
                }
            }
        
            Point2f P1, P4;
            if(l_out1.y < l_out2.y)
            {
                P1 = l_out1;
                P4 = l_out2;
            }
            else
            {
                P1 = l_out2;
                P4 = l_out1;
            }

            
            Point2f r_out1 = r_pts[0], r_out2 = r_pts[1];
            if(r_out1.x < r_out2.x) swap(r_out1, r_out2);
            for(int k=2;k<4;k++)
            {
                if(r_pts[k].x > r_out1.x)
                {
                    r_out2 = r_out1;
                    r_out1 = r_pts[k];
                }
                else if(r_pts[k].x > r_out2.x)
                {
                    r_out2 = r_pts[k];
                }
            }
       
            Point2f P2, P3;
            if(r_out1.y < r_out2.y)
            {
                P2 = r_out1;
                P3 = r_out2;
            }
            else
            {
                P2 = r_out2;
                P3 = r_out1;
            }

           
            circle(frame,P1,7,Scalar(0,255,0),-1);
            circle(frame,P2,7,Scalar(0,255,0),-1);
            circle(frame,P3,7,Scalar(0,255,0),-1);
            circle(frame,P4,7,Scalar(0,255,0),-1);

            putText(frame,"P1",Point(P1.x+8, P1.y-8),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,0),2);
            putText(frame,"P2",Point(P2.x+8, P2.y-8),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,0),2);
            putText(frame,"P3",Point(P3.x+8, P3.y+15),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,0),2);
            putText(frame,"P4",Point(P4.x+8, P4.y+15),FONT_HERSHEY_SIMPLEX,0.6,Scalar(0,255,0),2);

          
            line(frame,P1,P2,Scalar(0,255,0),2);
            line(frame,P2,P3,Scalar(0,255,0),2);
            line(frame,P3,P4,Scalar(0,255,0),2);
            line(frame,P4,P1,Scalar(0,255,0),2);
        }

        imshow("frame",frame);
        waitKey(20);
    }
    
    cap.release();
    destroyAllWindows();
}
