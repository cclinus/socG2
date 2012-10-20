#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../src/Map.hpp"
#include "../../src/Brain.hpp"

using namespace std;

// Set random map for test
Map getMap(){
    Map map(480,480);
    Ball aBall(400,400,1);
    map.addBall(aBall);
    Ball bBall(100,350,2);
    map.addBall(bBall);
    Obstacle aObs(320,100);
    map.addObstacle(aObs);
    Robot ourRobot(160,160,1);
    map.addRobot(ourRobot);
    Ball cBall(100,100,3);
    map.addBall(cBall);
    Ball dBall(200,200,4);
    map.addBall(dBall);
    Obstacle aObstacle(150,250);
    map.addObstacle(aObstacle);
    Obstacle bObstacle(180,360);
    map.addObstacle(bObstacle);
    Obstacle cObstacle(250,250);
    map.addObstacle(cObstacle);
    Obstacle dObstacle(319,260);
    map.addObstacle(dObstacle);

    return map;
}

int main () {
    while(1){
	sleep(1);
	Map map = getMap();
	ofstream guiFile;
	guiFile.open ("../../server/data");
	// Add robot
	Robot ourRobot = map.getOurRobot();
	guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";
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
	guiFile.close();
    }
    return 0;
}

