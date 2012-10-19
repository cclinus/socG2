#ifndef CAMERA_HPP
#define CAMERA_HPP

//using OPENCV LIBRARY
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"

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


    public:

    // The Camera obj has to be atteched to the map obj
    Camera(int no){
	this->cameraNo = no;
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

	cap.open(this->cameraNo);

	if(!cap.isOpened()){
	    cout<< "Can not find camera "<<this->cameraNo<<"\n";
	    return this->map;
	}else

	cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);


	vector<Mat> slices;
	// Cross Element for Erosion/Dilation
	Mat cross = getStructuringElement(MORPH_CROSS, Size(5,5));
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
	int HuethresH =94, 
	    HuethresL =66,
	    SatthresL =74,
	    SatthresH = 255,
	    ValthresL =87,
	    ValthresH = 255,
	    erosionCount = 15,
	    blurSize = 15;

	cap >> camImage;

	if(blurSize == 0)
	    blurSize = 1;

	blur(camImage, blurImage, Size(blurSize,blurSize));

	cvtColor(camImage, hsvImage,CV_RGB2HSV);

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

	erode(HnS,erd,cross,Point(-1,-1),erosionCount); // do erode
	dilate(HnS,dia,cross,Point(-1,-1),erosionCount);// do dialate
	// combine sat, val and hue filter together
	HSV = sat3 & hue3 & val3;

	// erode and dialation to reduce noise
	erode(HSV,erd,cross,Point(-1,-1),erosionCount); // do erode
	dilate(HSV,dia,cross,Point(-1,-1),erosionCount); // do dialate

	int ballCnt = circleObj(HSV);	

	printf("Ball Number = %i", ballCnt);
	return this->map;
    }

    int circleObj(Mat hsv){
	Mat h=hsv;
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
	for( int i = 0; i < contoursSize; i++ ){ 
	    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
	    // Ball aBall(center[i].x, center[i].y);
	    // this->map.addBall(aBall);

	    printf("radius = %f\n",radius[i]);
	}
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
