#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../src/Map.hpp"
#include "../../src/Brain.hpp"
#include "../../src/Camera.hpp"
#include "../../src/ControlUnit.hpp"


using namespace std;

Brain brain;
//Robot ourRobot(240,0,240,10,1);

void updateGui(Map map){

    ofstream guiFile;
    guiFile.open ("/home/opencvdev/workplace6/socG2/server/data");

    // Add to gui
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

    // Add robot and locationB
    Robot ourRobot = map.getOurRobot();
    // Fake the locationB of the robot
    //ourRobot.addLocationB(ourRobot.getLocation().getX(), ourRobot.getLocation().getY());

    cout << "--->Robot Location in Test: (" << ourRobot.getLocationB().getX() << "," << ourRobot.getLocationB().getY() << ")\n";
    guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";
    guiFile<<ourRobot.getLocationB().getX()<<"n"<<ourRobot.getLocationB().getY()<<"\n";

    guiFile.close();
}

int main () {

    Camera cameraOne(1);
    Camera cameraTwo(2);
    int cornerFlag;
    ControlUnit control;
    for(int j=0;j<100;j++){
	cornerFlag = cameraOne.initMap();
	cornerFlag = cameraTwo.initMap();
    }

    // Test control unit
    while(1){

	Map map(480,480);
	map = cameraOne.updateMap(map);
	map = cameraTwo.updateMap(map);
	brain.analyse(map);
	Location target = brain.getTarget();
	int state = brain.getState();
	// We only need adjust path with state 1 or 3
	if(state==1 or state==3){
	    control.adjust(map, target);
	}

	updateGui(map);

	sleep(0.5);
    }

    return 0;
}

