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

    Ball aBall(random_in_range(30,450), random_in_range(400,450), 1);
    map.addBall(aBall);
/*    Ball bBall(random_in_range(30,450), random_in_range(30,450),2);
    map.addBall(bBall);

    Obstacle aObs(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(aObs);

    Robot ourRobot(random_in_range(30,450), random_in_range(30,140),1);
    map.addRobot(ourRobot);

    Ball cBall(random_in_range(30,450), random_in_range(320,450), 3);
    map.addBall(cBall);

    Ball dBall(random_in_range(30,450), random_in_range(320,450),4);
    map.addBall(dBall);
*/
    Obstacle aObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(aObstacle);

    Obstacle bObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(bObstacle);
/*
    Obstacle cObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(cObstacle);
  */
/*
    Ball aBall(171,320,1);
    map.addBall(aBall);
    Ball bBall(300,240,2);
    map.addBall(bBall);
    Obstacle aObs(339,188);
    map.addObstacle(aObs);
*/
    Robot ourRobot(250,10,1);
    map.addRobot(ourRobot);
//    Obstacle aObstacle(380,170);
//    map.addObstacle(aObstacle);

//    Obstacle bObstacle(240,50);
//    map.addObstacle(bObstacle);

    return map;
}

int main () {
    // For random time
    srand(time(NULL));

    Map map = getMap();
    Brain brain;

    // Init map
    ofstream guiFile;
    guiFile.open ("/home/cclinus/workplace/375/socG2/server/data");

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

    sleep(1);

    while(1){

	ofstream guiFile;
	guiFile.open ("../../server/data");

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
	map.moveRobot(target);
	Robot ourRobot = map.getOurRobot();
	cout << "--->Robot Location in Test: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
	guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";

	guiFile.close();

	sleep(1);
    }

    return 0;
}

