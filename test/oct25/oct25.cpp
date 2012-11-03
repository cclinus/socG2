#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "../../src/Map.hpp"
#include "../../src/Brain.hpp"
#include "../../src/Camera.hpp"
using namespace std;

int main () {

	Camera cameraOne(0);
	Brain brain;
	Location LastLoc(500,500);
	vector<vector<Point> > corners(1);
	
	for(int j=0;j<100;j++){
		printf("Looking for corner %i",j);
		corners=cameraOne.initMap();
		for(int i=0; i <corners[0].size();i++){
		cout<<i<<"( "<<corners[0][i].x<<" "<<corners[0][i].y<<" ) ";
		}
		if(corners[0].size()>0){
			break;
		}
		cout<<endl;	
	}
	
	while(1){

		// Update map
		Map map(480,480);
		map = cameraOne.updateMap(map,corners);

		//	Robot ourRobot(240,0,1);
		//	map.addRobot(ourRobot);

		ofstream guiFile;
		guiFile.open ("/home/cclinus/workplace/375/socG2/server/data");

		brain.analyse(map);
		// Add Balls
		vector<Ball> ballVector = map.getBalls();
		for(int i=0; i<ballVector.size();i++){
			Ball ball = ballVector.at(i);
			Location ballLocation = ball.getLocation();
			guiFile<<""<<ballLocation.getX()<<"b"<<ballLocation.getY()<<"\n";
		}
		// Add obstacles
		vector<Obstacle> obsVector = map.getObstacles();
		for(int i=0; i<obsVector.size();i++){
			Obstacle obs = obsVector.at(i);
			Location obsLocation = obs.getLocation();
			guiFile<<""<<obsLocation.getX()<<"o"<<obsLocation.getY()<<"\n";
		}
		// Add target point to data
		Location target = brain.getTarget();
		guiFile<<target.getX()<<"t"<<target.getY()<<"\n";

		// Add robot
		Robot ourRobot;
		ourRobot = map.getOurRobot();
		cout << "--->Robot Location in Test: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
		guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";

		guiFile.close();
		double slope1 = brain.getSlop(ourRobot.getLocation(),target);
		double slope2 = brain.getSlop(LastLoc, ourRobot.getLocation());
		double angle = atan(slope1) - atan(slope2);
		double distance = sqrt((ourRobot.getLocation().getX()-target.getX())^2 + (ourRobot.getLocation().getY()-target.getY())^2);

		LastLoc = ourRobot.getLocation();
		sleep(0.1);
	}

	return 0;
}

