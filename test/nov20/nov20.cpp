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
Robot ourRobot(240,0,240,10,1);

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
    Ball bBall(random_in_range(30,450), random_in_range(400,450),2);
    map.addBall(bBall);

    Obstacle aObs(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(aObs);

    Ball cBall(random_in_range(30,450), random_in_range(320,450), 3);
    map.addBall(cBall);

    Ball dBall(random_in_range(30,450), random_in_range(320,450),4);
    map.addBall(dBall);

    Obstacle aObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(aObstacle);

    Obstacle bObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(bObstacle);
    Obstacle cObstacle(random_in_range(30,450), random_in_range(160,320));
    map.addObstacle(cObstacle);

    /*
       Ball aBall(100,450,1);
       map.addBall(aBall);
    //Ball bBall(300,240,2);
    //map.addBall(bBall);
    Obstacle aObs(100,420);
    map.addObstacle(aObs);
     */
    //    Robot ourRobot(250,10,1);
    //    map.addRobot(ourRobot);
    //    Obstacle aObstacle(380,170);
    //    map.addObstacle(aObstacle);

    //    Obstacle bObstacle(240,50);
    //    map.addObstacle(bObstacle);

    return map;
}

void updateGui(Map map){

    ofstream guiFile;
    guiFile.open ("/home/cclinus/workplace/375/socG2/server/data");

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
    ourRobot = map.getOurRobot();
    // Fake the locationB of the robot
    //ourRobot.addLocationB(ourRobot.getLocation().getX(), ourRobot.getLocation().getY());

    //cout << "--->Robot Location in Test: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
    guiFile<<ourRobot.getLocation().getX()<<"r"<<ourRobot.getLocation().getY()<<"\n";
    guiFile<<ourRobot.getLocationB().getX()<<"n"<<ourRobot.getLocationB().getY()<<"\n";

    guiFile.close();
}

// This function is to check whether stop or start the game
bool isStart(){
    // Check start file
    if (fopen("../../server/start", "r") == NULL) {
	return false;
    }
    else {
	return true;
    }
}

int main () {

    // robot colors
    int ourHead, ourTail, enemyHead, enemyTail;

    srand(time(NULL));
    Camera cameraOne(1);
    Camera cameraTwo(2);

    // Init camera
    map = cameraOne.updateMap(map);
    cout<<"Initializing camera one.\n";
    sleep(3);
    map = cameraTwo.updateMap(map);
    cout<<"Initializing camera two.\n";
    sleep(3);
    cout<<"Cameras are ready. \n";

    /*
    // Read and init robot colors
    // Lines: ourHead, ourTail, enemyHead, enemyTail
    // Color: 1.red; 2.green; 3.blue;
    string line;
    ifstream colorfile ("../../server/setting");
    if (colorfile.is_open()){
	int i=1;
	while ( colorfile.good() ){
	    getline (colorfile,line);
	    if(i==1) ourHead = atoi(&line);
	    if(i==2) ourTail = atoi(&line);
	    if(i==3) enemyHead = atoi(&line);
	    if(i==4) enemyTail = atoi(&line);
	    i++;
	}
	colorfile.close();
    }

    // Set colors of robots
    cameraOne.setOurHead(ourHead);
    cameraTwo.setOurHead(ourHead);
    */

    cameraOne.setOurHead(1);
    cameraTwo.setOurHead(1);
    cameraOne.setOurTail(2);
    cameraTwo.setOurTail(2);

    ControlUnit control;

    while(1){

	//Check start and stop command
	while(!isStart()){
	    cout << "Waiting to start\n";
	    sleep(1);
	}

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

	usleep(100000);
    }

    return 0;
}

