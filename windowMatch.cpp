#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/stitching/stitcher.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sys/time.h>

using namespace std;
using namespace cv;

int mx,my,clicked=0;
void mouse_callback(int event, int x, int y, int flags, void* param)
{
	clicked=0;
	
	//This is called every time a mouse event occurs in the window
	if (event == CV_EVENT_LBUTTONDOWN) { //This is executed when the left mouse button is clicked
		//Co-ordinates of the left click are assigned to global variables and flag is set to 1
		cout<<event<<endl;
		mx = x;
		my = y;
		clicked=1;
	}
}

void GetCursorPos(string s,Point* mousePos){
		setMouseCallback(s,mouse_callback,NULL);
		mousePos->x=mx;
		mousePos->y=my;
}

void setThreshold(Mat& im,int* rthresh,int* bthresh,int* gthresh,int* Rthresh,int* Bthresh,int* Gthresh){
	int r=(int)im.at<cv::Vec3b>(1,1)[2],g=(int)im.at<cv::Vec3b>(1,1)[1],b=(int)im.at<cv::Vec3b>(1,1)[0];
	int R=(int)im.at<cv::Vec3b>(1,1)[2],G=(int)im.at<cv::Vec3b>(1,1)[1],B=(int)im.at<cv::Vec3b>(1,1)[0];
	cout<<"colors:"<<(int)im.at<cv::Vec3b>(1,1)[0]<<" "<<(int)im.at<cv::Vec3b>(1,1)[1]<<" "<<(int)im.at<cv::Vec3b>(1,1)[2]<<endl;
	float rf=0,gf=0,bf=0;
	for(int i=1;i<im.rows;i++){
		for(int j=1;j<im.cols;j++){
			if((int)im.at<cv::Vec3b>(i,j)[2]>R)R=(int)im.at<cv::Vec3b>(i,j)[2];
			if((int)im.at<cv::Vec3b>(i,j)[2]<r)r=(int)im.at<cv::Vec3b>(i,j)[2];
			if((int)im.at<cv::Vec3b>(i,j)[2]>G)G=(int)im.at<cv::Vec3b>(i,j)[1];
			if((int)im.at<cv::Vec3b>(i,j)[2]<g)g=(int)im.at<cv::Vec3b>(i,j)[1];
			if((int)im.at<cv::Vec3b>(i,j)[2]>B)B=(int)im.at<cv::Vec3b>(i,j)[0];
			if((int)im.at<cv::Vec3b>(i,j)[2]<b)b=(int)im.at<cv::Vec3b>(i,j)[0];
			rf+=(int)im.at<cv::Vec3b>(i,j)[2]/(float)(im.rows*im.cols);
			gf+=(int)im.at<cv::Vec3b>(i,j)[1]/(float)(im.rows*im.cols);
			bf+=(int)im.at<cv::Vec3b>(i,j)[0]/(float)(im.rows*im.cols);
		}
	}
	cout<<"inFunction"<<endl;
	cout<<r<<","<<R<<" "<<rf<<endl;
	cout<<g<<","<<G<<" "<<gf<<endl;
	cout<<b<<","<<B<<" "<<bf<<endl;
	cout<<"-------"<<endl;
	*rthresh=rf*0.87;
	*Rthresh=rf*1.13;
	*gthresh=gf*0.87;
	*Gthresh=gf*1.13;
	*bthresh=bf*0.87;
	*Bthresh=bf*1.13;
	
	/**rthresh=r;
	*Rthresh=R;
	*bthresh=b;
	*Bthresh=B;
	*gthresh=g;
	*Gthresh=G;	*/
}

Mat threshold(Mat im,int rthresh,int Rthresh,int gthresh,int Gthresh,int bthresh,int Bthresh,int* ax,int* ay){
	for(int i=0;i<im.rows;i++){
		for(int j=0;j<im.cols;j++){
			int d=0;
			if((int)im.at<Vec3b>(i,j)[2]<rthresh || (int)im.at<Vec3b>(i,j)[2]>Rthresh)d=1;
			if((int)im.at<Vec3b>(i,j)[1]<gthresh || (int)im.at<Vec3b>(i,j)[1]>Gthresh)d=1;
			if((int)im.at<Vec3b>(i,j)[0]<bthresh || (int)im.at<Vec3b>(i,j)[0]>Bthresh)d=1;
			if(d==1){
				im.at<Vec3b>(i,j)[0]=0;im.at<Vec3b>(i,j)[1]=0;im.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	return im;
}

int main(){
	namedWindow("image",1);
	VideoCapture capture(0);
	if(!capture.isOpened()){
		cout<<"camera not working"<<endl;
	}
	
	Point p1,p2;
	Mat im;
	capture>>im;
	imshow("image",im);
	while(p1==Point(0,0) || p2==Point(0,0)){
	setMouseCallback("image",mouse_callback,NULL);
	if(clicked && p1==Point(0,0)){
		//cout<<"clicked p1"<<endl;
		GetCursorPos("image",&p1);
		clicked=0;}
	if(clicked){
		//cout<<"clicked p2"<<endl;
		GetCursorPos("image",&p2);}
	waitKey(33);
	}
	cout<<p1.x<<" p1 "<<p1.y<<endl;
	cout<<p2.x<<" p2 "<<p2.y<<endl;
	
	Mat im1;
	for(int i=0;i<30;i++){
	capture>>im1;}
	//rectangle(im1,p1,p2,Scalar(0,0,255));
	int sx,sy,ex,ey;
	if(p1.x>p2.x){sx=p2.x;ex=p1.x;}
	else {sx=p1.x;ex=p2.x;}
	if(p1.y>p2.y){sy=p2.y;ey=p1.y;}
	else {sy=p1.y;ey=p2.y;}
	Mat im2(ex-sx,ey-sy,CV_8UC3);
	im1(Rect(sx,sy,ex-sx,ey-sy)).copyTo(im2);
	//Point mousePos;
	imshow("image2",im2);
	cout<<"#colors:"<<(int)im2.at<cv::Vec3b>(2,2)[0]<<" "<<(int)im2.at<cv::Vec3b>(2,2)[1]<<" "<<(int)im2.at<cv::Vec3b>(2,2)[2]<<endl;
	int rthresh=0,gthresh=0,bthresh=0;
	int Rthresh=0,Gthresh=0,Bthresh=0;
	setThreshold(im2,&rthresh,&bthresh,&gthresh,&Rthresh,&Bthresh,&Gthresh);
	cout<<rthresh<<","<<Rthresh<<endl;
	cout<<gthresh<<","<<Gthresh<<endl;
	cout<<bthresh<<","<<Bthresh<<endl;
	while(1){
		capture>>im1;
		int ax,ay;
	im1=threshold(im1,rthresh,Rthresh,gthresh,Gthresh,bthresh,Bthresh,&ax,&ay);
	
	rectangle(im1,Point(ax-im2.rows/2,ay-im2.cols/2),Point(ax+im2.rows/2,ay+im2.cols/2),Scalar(255,255,255));
	imshow("image",im1);
	if(waitKey(33)==27)break;
	}
	
}
