#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
int main ()
{
Size boardSize(10,7);
float squareSize = 15;
vector<vector<Point3f>>objectPoints;
vector<vector<Point2f>>imagePoints;
vector<Point3f>obj;
for(int j=0;j<7;j++)
{
for(int i=0;i<10;i++)
        {
obj.push_back(Point3f(i*squareSize,j*squareSize,0));
        }
    }
vector<string>images;
glob("image/*.jpg",images);
Mat gray;
Mat sampleImg;
for(size_t i=0;i<images.size();i++)
{
Mat img= imread(images[i],1);
if(i==0) sampleImg = img.clone();
cvtColor(img ,gray,COLOR_BGR2GRAY);
vector<Point2f>corners;
bool found = findChessboardCorners(gray, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
if(found)
        {
cornerSubPix(gray, corners, Size(11,11), Size(-1,-1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,30,0.1));
imagePoints.push_back(corners);
objectPoints.push_back(obj);
drawChessboardCorners(img,boardSize,corners,found);
imshow("board",img);
waitKey(200);
        }
    }
Mat cameraMatrix, distCoeffs;
vector<Mat>rvecs,tvecs;
double error =calibrateCamera(objectPoints,imagePoints,gray.size(),cameraMatrix,distCoeffs,rvecs,tvecs);
cout<<"重投影误差 error:"<<error<<endl;
cout <<"相机内参 cameraMatrix:"<< endl << cameraMatrix <<endl;
cout <<"畸变系数 distCoeffs:"<< endl << distCoeffs <<endl;

Mat dst;
undistort(sampleImg,dst,cameraMatrix,distCoeffs);
imwrite("undistort_result.jpg",dst);
imshow("undistort",dst);
waitKey(0);
return 0;
}
