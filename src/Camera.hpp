#ifndef CAMERA_HPP
#define CAMERA_HPP

//using OPENCV LIBRARY
///##using openCV library##
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"


#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Map.hpp"

using namespace cv;
using namespace std;

/* Image processing code */
class Camera {

    int cameraNo;
    Map map;
    VideoCapture cap;
    vector<Mat> slices;
    // Cross Element for Erosion/Dilation
    Mat cross;
    // create matrices to hold image
    Mat camImage;		// raw image from webcam
    Mat blurImage;		// blur image
    Mat hsvImage;		// hsv image 
    Mat hue;			// hue channel
    Mat hue1;			// Hue upper bound
    Mat hue2;			// Hue lower bound
    Mat hue3;			// hue color filtering
    Mat sat;			// Sat channel
    Mat sat1;			// Sat upper bound
    Mat sat2;			// sat lower bound
    Mat sat3;			// sat color filtering
    Mat val;			// Val channel
    Mat val1;			// Val upper bound
    Mat val2;			// Val lower bound
    Mat val3;			// Val color filtering
    Mat erd;			// Erosion Image
    Mat dia;			// dialate image
    Mat HnS;			// sat and hue channel
    Mat HSV;			// HSV color fiter detected


    // threshold values
    int HuethresH, 
	HuethresL,
	SatthresL,
	SatthresH,
	ValthresL,
	ValthresH,
	erosionCount,
	blurSize;

    public:

    // The Camera obj has to be atteched to the map obj
    Camera(int no){
	this->cameraNo = no;
	cross = getStructuringElement(MORPH_CROSS, Size(5,5));


	cap.open(this->cameraNo);
	if(!cap.isOpened()){
	    cout<< "Can not find camera "<<this->cameraNo<<"\n";
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);	
	// threshold values
	HuethresH =85; 
	HuethresL =69;
	SatthresL =53;
	SatthresH = 255;
	ValthresL =80;
	ValthresH = 255;
	erosionCount = 2;
	blurSize = 15;
	//FIXME

	cvNamedWindow("Color Tune",CV_WINDOW_NORMAL);		

	//cvNamedWindow("Color Tune",CV_WINDOW_NORMAL);
	cvCreateTrackbar( "Hue UpperT","Color Tune", &HuethresH, 255, 0 );
	cvCreateTrackbar ("Hue LowerT","Color Tune", &HuethresL,255, 0);
	cvCreateTrackbar( "Sat UpperT","Color Tune", &SatthresH, 255, 0 );
	cvCreateTrackbar( "Sat LowerT","Color Tune", &SatthresL, 255, 0 );
	cvCreateTrackbar( "Val UpperT","Color Tune", &ValthresH, 255, 0 );
	cvCreateTrackbar( "Val LowerT","Color Tune", &ValthresL, 255, 0 );
	cvCreateTrackbar ("EroTime","Color Tune", &erosionCount,15, 0);
	cvCreateTrackbar ("BlurSize","Color Tune", &blurSize,15, 0);		
    }

    // Core processing function, init all balls, obstacles, robots to the map obj.
    // Feel free to create additional func within this class.
    Map updateMap(Map aMap){
	/* Image Processing and Camera connections go here.
	 * During the processing, once we find a ball for example,
	 * we can do:
	 *      Ball aBall(x, y, ballNo);
	 *      map.addBall(aBall);
	 * to add an ball obj to the map.
	 * Same to obstacles and robot, refer to Ball.hpp, Robot.hpp and Obstacle.hpp
	 */
	this->map = aMap;
	cap >> camImage;
	if(blurSize == 0)
	    blurSize = 1;

	//get homography for the map first
	vector<Point2f>cornersOff( 4 );
	vector<Point2f>cornersReal( 4 );
	cornersOff[0].x = 77; cornersOff[0].y = 56;
	cornersOff[1].x = 562; cornersOff[1].y = 34;
	cornersOff[2].x = 2; cornersOff[2].y = 200;
	cornersOff[3].x = 627; cornersOff[3].y = 184;

	cornersReal[0].x = 0; cornersReal[0].y = 0;
	cornersReal[1].x = 480; cornersReal[1].y = 0;
	cornersReal[2].x = 0; cornersReal[2].y = 240;
	cornersReal[3].x = 480; cornersReal[3].y = 240;


	Mat H = findHomography(cornersOff,cornersReal,CV_RANSAC);

	blur(camImage, blurImage, Size(blurSize,blurSize));

	cvtColor(camImage, hsvImage,CV_RGB2HSV);

	if(blurSize == 0)
	    blurSize = 1;
	blur(hsvImage, hsvImage, Size(blurSize,blurSize));

	split(hsvImage,slices);
	slices[0].copyTo (hue);
	slices[1].copyTo(sat);
	slices[2].copyTo(val);

	threshold(hue,hue1,HuethresL,255,CV_THRESH_BINARY);
	threshold(hue,hue2,HuethresH,255,CV_THRESH_BINARY_INV);
	hue3 = hue1 &hue2; //multiply 2 matrix to get the color range;
	// apply thresshold for Sat channel
	threshold (sat,sat1,SatthresL,255, CV_THRESH_BINARY); // get lower bound
	threshold (sat, sat2,SatthresH,255, CV_THRESH_BINARY_INV); // get upper bound
	sat3 = sat1 & sat2; // multiply 2 matrix to get the color range

	// apply thresshold for Val channel
	threshold (val,val1,ValthresL,255, CV_THRESH_BINARY); // get lower bound
	threshold (val, val2,ValthresH,255, CV_THRESH_BINARY_INV); // get upper bound
	val3 = val1 & val2; // multiply 2 matrix to get the color range
	//erode(HnS,erd,cross,Point(-1,-1),erosionCount); // do erode
	//dilate(HnS,dia,cross,Point(-1,-1),erosionCount);// do dialate
	// combine sat, val and hue filter together
	HSV = sat3 & hue3 & val3;

	// erode and dialation to reduce noise
	erode(HSV,erd,cross,Point(-1,-1),erosionCount); // do erode
	dilate(HSV,dia,cross,Point(-1,-1),erosionCount); // do dialate
	int ballCnt = circleObj(erd,H);

	imshow("Webcam Orignal", camImage);
	imshow("HSV",HSV);
	imshow("erd",erd);
	cvWaitKey(5);
	return this->map;
    }

    int circleObj(Mat hsv,Mat homo){
	Mat h=hsv;
	Mat hm = homo;
	RNG rng(12345);

	Mat threshold_out;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	threshold(h, threshold_out,100,255,THRESH_BINARY);

	findContours(threshold_out, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	int contoursSize;
	contoursSize=contours.size();
	//printf("ball number = %i\n", contoursSize);
	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
	vector<Point2f>ObjCtr( 1 );
	vector<Point2f>ObjCtrFix( 1 );
	int cnt=1;
	for( int i = 0; i < contoursSize; i++ ){ 
	    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
	    if(radius[i]>=1.5){
		ObjCtr[0] = center[i];
		cout<<cnt<<"origin ="<<"( "<<center[i].x<<","<<center[i].y<<" )"<<" ";
		perspectiveTransform(ObjCtr, ObjCtrFix, hm);
		Ball aBall((int)ObjCtrFix[0].x, (int)ObjCtrFix[0].y,cnt);
		this->map.addBall(aBall);
		cnt++;
	    }
	}
	printf("\n");
	//printf("%i\n",contoursSize);	
	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros( threshold_out.size(), CV_8UC3 );
	for( int i = 0; i< contoursSize; i++ )
	{
	    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	    //drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
	    //rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
	    circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
	}

	imshow( "Contours", drawing );	

	return 	contoursSize;

    }

};

#endif
