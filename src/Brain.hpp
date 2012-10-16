#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <math.h>
#include "Map.hpp"
#include "Location.hpp"
#include "Ball.hpp"
#include "Obstacle.hpp"
#include "Robot.hpp"

#define OBSTACLE_DIAMETER 23
#define SAFE_WALKAROUND 69

using namespace std;

/* AI */
class Brain{
    Map map;
    Location targetPoint;
    int state;

    public:

    Brain(){}

    void analyse(Map map){
	this->map = map;
	vector<Ball> ballVector = map.getBalls();
	vector<Obstacle> obstacleVector = map.getObstacles();
	Location nextTarget = getNearestBall();
	cout << "\nNearest Ball: ("<<nextTarget.getX()<<","<<nextTarget.getY()<<")\n";
	// Check if there is obj on its way
	int loopCounter = 0;
	while(loopCounter < 10){
	    Location obstacleOnTheWay = getObstacleOnTheWay(nextTarget);
	    cout << "\nObs on the way: (" << obstacleOnTheWay.getX() << "," << obstacleOnTheWay.getY() << ")\n";
	    if(obstacleOnTheWay.getX() == nextTarget.getX() and obstacleOnTheWay.getY() == nextTarget.getY()){
		//If there's no obj on the way
		break;
	    }else{
		// If there is obj, need walk around
		// Go to another point
		// Caculate A distance
		int xa = obstacleOnTheWay.getX();
		int ya = obstacleOnTheWay.getY();
		int oa = sqrt( (pow(xa,2)+pow(ya,2)) );
		int xd = ((oa+SAFE_WALKAROUND)*xa)/oa;
		int yd = ((oa+SAFE_WALKAROUND)*ya)/oa;
		nextTarget.setX(xd);
		nextTarget.setY(yd);
	    }
	    loopCounter++;
	}
	this->targetPoint = nextTarget;
    }

    Location getTarget(){
	return this->targetPoint;
    }

    // Get nearest ball regarding on our Robot's location
    Location getNearestBall(void){
	Location shortestLocation;
	int shortestDistance = 1000; // distance can never reach 1000
	for(int i=0; i<this->map.getBalls().size();i++){
	    Ball someBall = this->map.getBalls().at(i);
	    Location ballLocation = someBall.getLocation();
	    Robot ourRobot = this->map.getOurRobot();
	    Location robotLocation = ourRobot.getLocation();
	    int distance = getDistance(ballLocation, robotLocation);
	    if(distance < shortestDistance){
		shortestDistance = distance;
		shortestLocation = someBall.getLocation();
	    }
	}
	return shortestLocation;
    }

    // Check if there is obj on the way and return the nearest one
    Location getObstacleOnTheWay(Location target){
	// check obstacles
	int nearestObstacleDistance = 1000;
	Location nearestObstacle = target;
	Robot ourRobot = this->map.getOurRobot();
	for(int i=0;i<this->map.getObstacles().size();i++){
	    Location obstacleLocation = this->map.getObstacles().at(i).getLocation();
	    double lineDistance = getLineDistance(obstacleLocation, ourRobot.getLocation(), target);
	    if(lineDistance < OBSTACLE_DIAMETER){// Obstacle diameter
		int distance = getDistance(ourRobot.getLocation(), obstacleLocation);
		if(distance < nearestObstacleDistance){
		    nearestObstacleDistance = distance;
		    nearestObstacle = obstacleLocation;
		}
	    }
	}
	return nearestObstacle;
    }

    // Get distance from A to line BC
    double getLineDistance(Location A, Location B, Location C){
	// calculate k
	int k = (B.getY()-C.getY())/(B.getX()-C.getX());
	// calculate b
	int b = B.getY()-k*B.getX();
	// calculate num
	double num = k*A.getX()-A.getY()+(B.getY()-k*B.getX());
	if(num<0) num = 0-num;
	// calculate den
	double den = sqrt((pow(k,2)+1));
	double distance = num/den;
	return distance;
    }

    // Get distance from A and B
    int getDistance(Location A, Location B){                                   
	int x1 = A.getX();                                                            
	int y1 = A.getY();                                                            
	int x2 = B.getX();                                                            
	int y2 = B.getY();                                                            
	int x = x1-x2;                                                                
	int y = y1-y2;                                                                
	int xp = pow(x,2);                                                            
	int yp = pow(y,2);                                                            
	return sqrt(xp+yp);                                                           
    }   

};

#endif
