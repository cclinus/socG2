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
#include "Robot.hpp"
using namespace cv;
using namespace std;

#define CAMERANO1 1
#define CAMERANO2 2
#define ROBOT_RED 1
#define ROBOT_BLUE 3
#define ROBOT_GREEN 2

int thresh = 228, N = 1;


/* Image processing code */
class Camera {

    int cameraNo;
    Map map;
    	VideoCapture cap;
	vector<Mat> slices;
	vector<vector<Point> >offset;
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
	    diaCount,
	    blurSize;
	int redX;
	int redY;
	int greX;
	int greY;
	//FIXME
	int ourRobotHead,ourRobotTail,enemyRobotHead,enemyRobotTail;
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
        HuethresH =86; 
	HuethresL =65;
	SatthresL =65;
	SatthresH = 255;
	ValthresL =78;
	ValthresH = 255;
	erosionCount = 2;
	diaCount = 3;
	blurSize = 7;
    }

	//FIXME
	void setOurHead(int color){
		this->ourRobotHead = color;
	}
	
	void setOurTail(int color){
		this->ourRobotTail = color;
	}
	
	void setEnemyHead(int color){
		this->enemyRobotHead = color;
	}

	void setEnemyTail(int color){
		this->enemyRobotTail = color;
	}
    
    // Core processing function, init all balls, obstacles, robots to the map obj.
    // Feel free to create additional func within this class.
    Map updateMap(Map aMap){

	this->map = aMap;
	cap >> camImage;
	if(blurSize == 0)
	    blurSize = 1;
	
	//get homography for the map first
	vector<Point2f>cornersOff( 4 );
	vector<Point2f>cornersReal( 4 );
	
	if(cameraNo == CAMERANO1){
		/*cornersOff[0].x = this->offset[0][2].x; cornersOff[0].y = this->offset[0][2].y;
		cornersOff[1].x = this->offset[0][3].x; cornersOff[1].y = this->offset[0][3].y;
		cornersOff[2].x = this->offset[0][1].x; cornersOff[2].y = this->offset[0][1].y;
		cornersOff[3].x = this->offset[0][0].x; cornersOff[3].y = this->offset[0][0].y;
		*/
		cornersOff[0].x = 196; cornersOff[0].y = 10;
		cornersOff[1].x = 364; cornersOff[1].y = 8;
		cornersOff[2].x = 196; cornersOff[2].y = 48;
		cornersOff[3].x = 369; cornersOff[3].y = 47;

		cornersReal[0].x = 155; cornersReal[0].y = 5;
		cornersReal[1].x = 325; cornersReal[1].y = 5;
		cornersReal[2].x = 155; cornersReal[2].y = 55;
		cornersReal[3].x = 325; cornersReal[3].y = 55;
	}else if(cameraNo == CAMERANO2){
		//FIXME
		/*
		cornersOff[0].x = this->offset[0][0].x; cornersOff[0].y = this->offset[0][0].y;
		cornersOff[1].x = this->offset[0][1].x; cornersOff[1].y = this->offset[0][1].y;
		cornersOff[2].x = this->offset[0][3].x; cornersOff[2].y = this->offset[0][3].y;
		cornersOff[3].x = this->offset[0][2].x; cornersOff[3].y = this->offset[0][2].y;
		*/
		cornersOff[0].x = 402; cornersOff[0].y = 106;
		cornersOff[1].x = 226; cornersOff[1].y = 101;
		cornersOff[2].x = 399; cornersOff[2].y = 69;
		cornersOff[3].x = 229; cornersOff[3].y = 64;

		cornersReal[0].x = 155; cornersReal[0].y = 415;
		cornersReal[1].x = 325; cornersReal[1].y = 415;
		cornersReal[2].x = 155; cornersReal[2].y = 475;
		cornersReal[3].x = 325; cornersReal[3].y = 475;
	}

	Mat H = findHomography(cornersOff,cornersReal,CV_RANSAC);
	
	blur(camImage, blurImage, Size(blurSize,blurSize));

	cvtColor(camImage, hsvImage,CV_RGB2HSV);
	
	if(blurSize == 0)
		blurSize = 1;
	blur(hsvImage, hsvImage, Size(blurSize,blurSize));



	//looping thru the to update each object 1=ball, 2=obstacle, 3=robot_color1, 4=robot_color2
	//we need to get robots first then get ball, obstacle since camera may misread robot color as ball or obstacle
	for(int i=1; i<=4; i++){
		//printf("round %i\n",i);
		getObj(i,H);
	}
	return this->map;
    }


	int initMap(){
		cap>>camImage;
		
		vector<vector<Point> > squares;
		findSquares(camImage, squares);
		int width;
		int length;
		int max=0;
		vector<vector<Point> >corners(1);
		
		vector<int> area( squares.size() );
		
		for(size_t i=0; i<squares.size(); i++){
			if(squares[i][0].y<=240){
				width = sqrt((squares[i][0].x-squares[i][1].x)^2-(squares[i][0].y-squares[i][1].y)^2);
				length = sqrt((squares[i][1].x-squares[i][2].x)^2-(squares[i][1].y-squares[i][2].y)^2);
				area[i]=width*length;

				if(area[i]>=max){
					max=area[i];
					//for(int j=0;j<4;j++){
						corners[0] = squares[i];
	//				//}
				}
			}
		}
		int cornerFlag=0;
		drawSquares(camImage, corners);
		imshow("camImage",camImage);
		for(int i=0; i <corners[0].size();i++){
			cout<<i<<"( "<<corners[0][i].x<<" "<<corners[0][i].y<<" ) ";
			//if(corners[0].size()>0)
			//break;
		}
		if(corners[0].size()>0){
			cornerFlag=1;
		}
		cout<<endl;	
	
		cvWaitKey(5);	
		//return corners;
		this->offset = corners;
		return cornerFlag;
	}
	static double angle( Point pt1, Point pt2, Point pt0 )
	{
    		double dx1 = pt1.x - pt0.x;
    		double dy1 = pt1.y - pt0.y;
    		double dx2 = pt2.x - pt0.x;
    		double dy2 = pt2.y - pt0.y;
    		return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
	}

	// returns sequence of squares detected on the image.
	// the sequence is stored in the specified memory storage
	static void findSquares( const Mat& image, vector<vector<Point> >& squares )
	{
	    squares.clear();
	    //Mat image8U(image.size(),CV_8U);
	    vector<vector<Point> > contours;
	    
	    Mat gray0(image.size(),CV_8U),gray;

	    
	    //cvtColor(image, imageRGB, CV_HSV2RGB);
	    // down-scale and upscale the image to filter out the noise
	    //pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
	    //pyrUp(pyr, timg, image.size());

	    // find squares in every color plane of the image
	    for( int c = 0; c < 3; c++ )
	    {	
		int ch[] = {c, 0};
		mixChannels(&image, 1, &gray0, 1, ch, 1);

		
		// try several threshold levels
		for( int l = 0; l < N; l++ )
		{
		    // hack: use Canny instead of zero threshold level.
		    // Canny helps to catch squares with gradient shading
		    if( l == 0 )
		    {
			// apply Canny. Take the upper threshold from slider
			// and set the lower to 0 (which forces edges merging)
			Canny(gray0, gray, 0, thresh, 3);
			// dilate canny output to remove potential
			// holes between edge segments
			dilate(gray, gray, Mat(), Point(-1,-1));
		    }
		    else
		    {
			// apply threshold if l!=0:
			//     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
			gray = gray0 >= (l+1)*255/N;
		    }
		    
		    // find contours and store them all as a list
		    findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		    vector<Point> approx;

		    // test each contour
		    for( size_t i = 0; i < contours.size(); i++ )
		    {
			// approximate contour with accuracy proportional
			// to the contour perimeter
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

			// square contours should have 4 vertices after approximation
			// relatively large area (to filter out noisy contours)
			// and be convex.
			// Note: absolute value of an area is used because
			// area may be positive or negative - in accordance with the
			// contour orientation
			if( approx.size() == 4 &&
			    fabs(contourArea(Mat(approx))) > 1000 &&
			    isContourConvex(Mat(approx)) )
			{
			    double maxCosine = 0;

			    for( int j = 2; j < 5; j++ )
			    {
				// find the maximum cosine of the angle between joint edges
				double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
				maxCosine = MAX(maxCosine, cosine);
			    }

			    // if cosines of all angles are small
			    // (all angles are ~90 degree) then write quandrange
			    // vertices to resultant sequence
			    if( maxCosine < 0.3 )
				squares.push_back(approx);
			}
		    }
		}
	    }
	}

	// the function draws all the squares in the image
static void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3, CV_AA);
    }

}

    
    void getObj(int i, Mat H){
	//if(cameraNo == CAMERANO1){
	
	 	if(i == 1){
			HuethresH =103; HuethresL =58;
			SatthresL =70; SatthresH = 255;
			ValthresL =91; ValthresH = 255;
			erosionCount = 2;
			diaCount = 3;
			blurSize = 5;
		}else if(i == 2){
			HuethresH =38; HuethresL =0;
			SatthresL =34; SatthresH = 255;
			ValthresL =76; ValthresH = 255;
			erosionCount = 2;
			diaCount = 3;
			blurSize = 4;
		}else if(i == 3){//GET OUR ROBOT HEAD COLOR
			//cout<<this->ourRobotHead<<endl;
			if(this->ourRobotHead == ROBOT_RED){
				HuethresH =163; HuethresL =104;
				SatthresL =106; SatthresH = 255;
				ValthresL =78; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 4;
			}else if(this->ourRobotHead == ROBOT_BLUE){
				//FIXME
				HuethresH =33; HuethresL =0;
				SatthresL =35; SatthresH = 255;
				ValthresL =53; ValthresH = 255;
				erosionCount = 3;
				diaCount = 3;
				blurSize = 7;
			}else if(this->ourRobotHead == ROBOT_GREEN){
				//FIXME
				//cout<<"head"<<endl;
				HuethresH =73; HuethresL =21;
				SatthresL =93; SatthresH = 255;
				ValthresL =43; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 0;
			}
		}else if(i == 4){//GET OUR ROBOT TAIL COLOR
			if(this->ourRobotTail == ROBOT_GREEN){
				HuethresH =73; HuethresL =21;
				SatthresL =93; SatthresH = 255;
				ValthresL =43; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 0;
			}else if(this->ourRobotTail == ROBOT_BLUE){
				//FIXME
				HuethresH =33; HuethresL =0;
				SatthresL =35; SatthresH = 255;
				ValthresL =53; ValthresH = 255;
				erosionCount = 3;
				diaCount = 3;
				blurSize = 7;
				//FIXME
			}else if(this->ourRobotTail == ROBOT_RED){
				//FIXME
				//cout<<"tail"<<endl;
				HuethresH =163; HuethresL =104;
				SatthresL =106; SatthresH = 255;
				ValthresL =78; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 4;
			}
		}
/*
	 	if(i == 1){
			HuethresH =106; HuethresL =58;
			SatthresL =79; SatthresH = 255;
			ValthresL =72; ValthresH = 255;
			erosionCount = 2;
			diaCount = 3;
			blurSize = 7;
		}else if(i == 2){
			HuethresH =38; HuethresL =0;
			SatthresL =34; SatthresH = 255;
			ValthresL =76; ValthresH = 255;
			erosionCount = 2;
			diaCount = 3;
			blurSize = 4;
		}else if(i == 3){//GET OUR ROBOT HEAD COLOR
			if(ourRobotHead == ROBOT_RED){
				HuethresH =163; HuethresL =104;
				SatthresL =106; SatthresH = 255;
				ValthresL =78; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 4;
			}else if(ourRobotHead == ROBOT_BLUE){
				//FIXME
				HuethresH =33; HuethresL =0;
				SatthresL =35; SatthresH = 255;
				ValthresL =53; ValthresH = 255;
				erosionCount = 3;
				diaCount = 3;
				blurSize = 7;
			}else if(ourRobotHead == ROBOT_GREEN){
				//FIXME
				HuethresH =73; HuethresL =21;
				SatthresL =93; SatthresH = 255;
				ValthresL =43; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 0;
			}
		}else if(i == 4){//GET OUR ROBOT TAIL COLOR
			if(ourRobotTail == ROBOT_GREEN){
				HuethresH =73; HuethresL =21;
				SatthresL =93; SatthresH = 255;
				ValthresL =43; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 0;
			}else if(ourRobotTail == ROBOT_BLUE){
				//FIXME
				HuethresH =33; HuethresL =0;
				SatthresL =35; SatthresH = 255;
				ValthresL =53; ValthresH = 255;
				erosionCount = 3;
				diaCount = 3;
				blurSize = 7;
				//FIXME
			}else if(ourRobotTail == ROBOT_RED){/
				//FIXME
				HuethresH =163; HuethresL =104;
				SatthresL =106; SatthresH = 255;
				ValthresL =78; ValthresH = 255;
				erosionCount = 1;
				diaCount = 3;
				blurSize = 4;
			}
		}
*/	
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
	HSV = sat3 & hue3 & val3;

	// erode and dialation to reduce noise
	erode(HSV,erd,cross,Point(-1,-1),erosionCount); // do erode
	dilate(erd,dia,cross,Point(-1,-1),diaCount); // do dialate

	//draw circle around object
	int ballCnt = circleObj(dia,H,i);
	
	//imshow("Webcam Orignal", camImage);
	//imshow("HSV",HSV);
	//imshow("erd",dia);
	//cvWaitKey(5);
	//return this->map;
	
    }

    int circleObj(Mat hsv,Mat homo,int ii){
	Mat h=hsv;
	Mat hm = homo;
	RNG rng(12345);

	Mat threshold_out;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	threshold(h, threshold_out,100,255,THRESH_BINARY);

	findContours(threshold_out, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	int contoursSize;
	int dropFlag;
	contoursSize=contours.size();
	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
	vector<Point2f>ObjCtr( 1 );
	vector<Point2f>ObjCtrFix( 1 );
	int cnt=1;
	float tempX[2];
	float tempY[2];
	int findUpper=0;
	int findLower=0;
	for( int i = 0; i < contoursSize; i++ ){ 
		
	    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	    minEnclosingCircle( contours_poly[i], center[i], radius[i] );
	    if(radius[i]>=1.5){
		ObjCtr[0] = center[i];
		//cout<<cnt<<"origin ="<<"( "<<center[i].x<<","<<center[i].y<<" )"<<" ";
		perspectiveTransform(ObjCtr, ObjCtrFix, hm);
		if(ii == 1){
			if(radius[i]<=15){
			
			//if(cameraNo==CAMERANO1){// && ObjCtrFix[0].y<=240){
			//cout<<"x "<<ObjCtrFix[0].x<<"y "<<ObjCtrFix[0].y<<endl;

			if(cameraNo==CAMERANO2 && ObjCtrFix[0].y<=240){
				ObjCtrFix[0].x=ObjCtrFix[0].x*0.97826-7.01073;
				ObjCtrFix[0].y=ObjCtrFix[0].y*0.962567+44.117647;
			}	
				if(ObjCtrFix[0].x <0){
					ObjCtrFix[0].x=5;
				}else if(ObjCtrFix[0].y<0){
					ObjCtrFix[0].y=5;
				}
			//	if(cameraNo == CAMERANO01 && ObjCtrFix[0].y<=240){
				Ball aBall((int)ObjCtrFix[0].x, (int)ObjCtrFix[0].y,cnt);
	    			this->map.addBall(aBall);
			}	
					
			/*}else if(cameraNo==CAMERANO2 && ObjCtrFix[0].y>240){
				//FIXME
				//cout<<"ball ("<<ObjCtrFix[0].x<<" "<<ObjCtrFix[0].y<<")"<<endl;
				if(ObjCtrFix[0].x <0){
					ObjCtrFix[0].x=5;
				}else if(ObjCtrFix[0].y<0){
					ObjCtrFix[0].y=5;
				}
				Ball aBall((int)ObjCtrFix[0].x, (int)ObjCtrFix[0].y,cnt);
	    			this->map.addBall(aBall);
			}*/
			//}
	    	}else if(ii == 2){
			if(cameraNo == CAMERANO1){
				//	cout<<"obstacle ("<<ObjCtrFix[0].x<<" "<<ObjCtrFix[0].y<<")"<<endl;
				//ObjCtrFix[0].x = ObjCtrFix[0].x*0.8623+22.4596;
				//ObjCtrFix[0].y = ObjCtrFix[0].y*0.9217+91.8793;
				
				ObjCtrFix[0].x = ObjCtrFix[0].x*0.8623+22.4596;
				ObjCtrFix[0].y = ObjCtrFix[0].y*0.9217+61.8793;
				
				Obstacle aObstacle((int)ObjCtrFix[0].x, (int)ObjCtrFix[0].y);
				this->map.addObstacle(aObstacle);
			}	
		}else if(ii == 3){
			//cout<<"objctrfix red"<<ObjCtrFix[0].x<<" "<<ObjCtrFix[0].y<<endl;

			ObjCtrFix[0].x=ObjCtrFix[0].x*0.9372+15.3142;
			//ObjCtrFix[0].y=ObjCtrFix[0].y*0.8771+23.6617;
			ObjCtrFix[0].y=ObjCtrFix[0].y*0.8771+33.6617;
			//cout<<"objctrfix red"<<ObjCtrFix[0].x<<" "<<ObjCtrFix[0].y<<endl;
			if(cameraNo == CAMERANO1 && ObjCtrFix[0].y<=240){
				//if(ObjCtrFix[0].y <0 && ObjCtrFix[0].y > -20){
				//	ObjCtrFix[0].y = ObjCtrFix[0].y + 20;
				//}
				redX =(int)ObjCtrFix[0].x;
				redY =(int)ObjCtrFix[0].y;
			}else if(cameraNo == CAMERANO2 && ObjCtrFix[0].y>240){
				//cout<<"objctrfix red"<<ObjCtrFix[0].x<<endl;
				//==if(ObjCtrFix[0].y <0 && ObjCtrFix[0].y > -20){
				//	ObjCtrFix[0].y = ObjCtrFix[0].y + 20;
				//}
				redX =(int)ObjCtrFix[0].x;
				redY =(int)ObjCtrFix[0].y;
			}
		}else if(ii == 4){
			
			//cout<<"objctrfix green"<<ObjCtrFix[0].x<<" "<<ObjCtrFix[0].y<<endl;
			ObjCtrFix[0].x=ObjCtrFix[0].x*0.9372+15.3142;
			//ObjCtrFix[0].y=ObjCtrFix[0].y*0.8771+23.6617;
			ObjCtrFix[0].y=ObjCtrFix[0].y*0.8771+33.6617;
			if(cameraNo == CAMERANO1 && ObjCtrFix[0].y<=240){
				//if(ObjCtrFix[0].y <0 && ObjCtrFix[0].y > -20){
				//	ObjCtrFix[0].y = ObjCtrFix[0].y + 20;
				//}
				greX = (int)ObjCtrFix[0].x; 
				greY = (int)ObjCtrFix[0].y;
			
				//cout<<redX<<" "<<redY<<" "<<greX<<" "<<greY<<endl;
				Robot aRobot(redX, redY, greX, greY, cnt);
				this->map.addRobot(aRobot);
			}else if(cameraNo == CAMERANO2 && ObjCtrFix[0].y>240){
				//if(ObjCtrFix[0].y <0 && ObjCtrFix[0].y > -20){
				//	ObjCtrFix[0].y = ObjCtrFix[0].y + 20;
				//}
				greX = (int)ObjCtrFix[0].x; 
				greY = (int)ObjCtrFix[0].y;
			
				//cout<<redX<<" "<<redY<<" "<<greX<<" "<<greY<<endl;
				Robot aRobot(redX, redY, greX, greY, cnt);
				this->map.addRobot(aRobot);
			}			
			//cout<<aRobot.getLocationB().getX()<<" get "<<aRobot.getLocationB().getY()<<"\n";			
		}		
		
		cnt++;
	    }
	}
	/// Draw polygonal contour + bonding rects + circles
	
	Mat drawing = Mat::zeros( threshold_out.size(), CV_8UC3 );
	for( int i = 0; i< contoursSize; i++ )
	{
	    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	    circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
	}

	//imshow( "Contours", drawing );	
	//cvWaitKey(5);	
	return 	contoursSize;

    }

};

#endif
