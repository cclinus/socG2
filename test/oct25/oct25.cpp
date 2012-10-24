#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../src/Map.hpp"
#include "../../src/Brain.hpp"
#include "../../src/Camera.hpp"

using namespace std;

int main () {

    Camera cameraOne(0);
    Brain brain;

    while(1){

	// Update map
	Map map(480,480);
	map = cameraOne.updateMap(map);

	Robot ourRobot(240,0,1);
	map.addRobot(ourRobot);

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
	cout << "--->Robot Location in Test: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
	guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";

	guiFile.close();

	sleep(2);
    }

    return 0;
}

