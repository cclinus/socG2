#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../src/Map.hpp"
#include "../../src/Brain.hpp"

using namespace std;

int random_in_range (unsigned int min, unsigned int max){
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range       = max - min,
	remainder   = RAND_MAX % range,
	bucket      = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval
       within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) {
	return min + base_random/bucket;
    } else {
	return random_in_range (min, max);
    }
}

// Set random map for test
Map getMap(){

    Map map(480,480);

    Ball aBall(random_in_range(0,480), random_in_range(0,480), 1);
    map.addBall(aBall);
    Ball bBall(random_in_range(0,480), random_in_range(0,480),2);
    map.addBall(bBall);
    Obstacle aObs(random_in_range(0,480), random_in_range(0,480));
    map.addObstacle(aObs);
    Robot ourRobot(random_in_range(0,480), random_in_range(0,480),1);
    map.addRobot(ourRobot);
    Ball cBall(random_in_range(0,480), random_in_range(0,480), 3);
    map.addBall(cBall);
    Ball dBall(random_in_range(0,480), random_in_range(0,480),4);
    map.addBall(dBall);
    Obstacle aObstacle(random_in_range(0,480), random_in_range(0,480));
    map.addObstacle(aObstacle);
    Obstacle bObstacle(random_in_range(0,480), random_in_range(0,480));
    map.addObstacle(bObstacle);
    Obstacle cObstacle(random_in_range(0,480), random_in_range(0,480));
    map.addObstacle(cObstacle);
    Obstacle dObstacle(random_in_range(0,480), random_in_range(0,480));
    map.addObstacle(dObstacle);

    return map;
}

int main () {
    // For random time
    srand(time(NULL));

    Brain brain;
    Map map = getMap();

    // Init map
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

    sleep(2);

    while(1){

	ofstream guiFile;
	guiFile.open ("../../server/data");

	// Add robot
	Robot ourRobot = map.getOurRobot();
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

	brain.analyse(map);
	// Add target point to data
	Location target = brain.getTarget();
	guiFile<<target.getX()<<"t"<<target.getY()<<"\n";

	ourRobot.moveFull(target);
	guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";

	guiFile.close();

	sleep(2);
    }

    return 0;
}

