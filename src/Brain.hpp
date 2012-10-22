#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <math.h>
#include "Map.hpp"
#include "Location.hpp"
#include "Ball.hpp"
#include "Obstacle.hpp"
#include "Robot.hpp"

#define SAFE_WALKAROUND 65
#define DANGER_DISTANCE 35

using namespace std;

/* AI */
class Brain{
    Map map;
    Location targetPoint;
    int state;

    public:

    Brain(){}

    void analyse(Map aMap){
	this->map = aMap;
	vector<Ball> ballVector = map.getBalls();
	vector<Obstacle> obstacleVector = this->map.getObstacles();

	Location nextTarget = getNearestBall();
	Robot ourRobot = this->map.getOurRobot();

	// Check if there is obj on its way
	int loopCounter = 0;
	while(loopCounter < 10){
	    cout << "\n*****Loop Count "<<loopCounter<<"*****\n";
	    Location obstacleOnTheWay = getObstacleOnTheWay(nextTarget);
	    cout << "Nearest Ball (target): ("<<nextTarget.getX()<<","<<nextTarget.getY()<<")\n";
	    cout << "Robot Location: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
	    if(obstacleOnTheWay.getX() == nextTarget.getX() and obstacleOnTheWay.getY() == nextTarget.getY()){
		//If there's no obj on the way
		break;
	    }else{
		cout << "Obs on the way: (" << obstacleOnTheWay.getX() << "," << obstacleOnTheWay.getY() << ")\n";
		// If there is obj, need walk around
		// Go to another point
		// FIXME 
		// What if go out of boundary? 
		// What if ball has same location with an obstacle
		int xa = obstacleOnTheWay.getX(); 
		int ya = obstacleOnTheWay.getY();
		double k1 = getSlop(nextTarget, ourRobot.getLocation());
		double k2 = (-1)/k1;
		double alpha1 = atan(k1);
		// k1 could be negative
		if(alpha1<0) {
		    alpha1 *= (-1);
		    alpha1 += 90;
		}
		double alpha2 = atan(k2);
		double sinA = sin(alpha2);
		double cosA = cos(alpha2);
		if(sinA<0) sinA *= (-1);
		if(cosA<0) cosA *= (-1);
		cout << "sin,cos:"<< sinA <<","<< cosA <<"\n";
		cout << "alpha1,alpha2:"<<alpha1<<","<<alpha2<<"\n";
		// Decide which way to turn
		double k3 = getSlop(obstacleOnTheWay, ourRobot.getLocation());
		cout << "k1, k2, k3:"<< k1 <<","<< k2 << "," << k3 <<"\n";
		// We can walk around if alpha2 is too small
		int xw = SAFE_WALKAROUND*cosA;
		int yw = SAFE_WALKAROUND*sinA;
		int xd, yd;

		if(alpha1>=90 and alpha1<180){
		    if(ourRobot.getLocation().getX()<nextTarget.getX()){
			if(k3>k1){
			    cout<<"case1:"<<xw<<","<<yw<<"\n";
			    xd = xa - xw;
			    yd = ya - yw;
			}else{
			    cout<<"case2:"<<xw<<","<<yw<<"\n";
			    xd = xa + xw;
			    yd = ya + yw;
			}
		    }else{
			if(k3<k1){
			    cout<<"case3:"<<xw<<","<<yw<<"\n";
			    xd = xa - xw;
			    yd = ya - yw;
			}else{
			    cout<<"case4:"<<xw<<","<<yw<<"\n";
			    xd = xa + xw;
			    yd = ya + yw;
			}
		    }
		}else if(alpha1<90 and alpha1>0){
		    if(ourRobot.getLocation().getX()<nextTarget.getX()){
			if(k3>k1){
			    cout<<"case5:"<<xw<<","<<yw<<"\n";
			    xd = xa + xw;
			    yd = ya - yw;
			}else{
			    cout<<"case6:"<<xw<<","<<yw<<"\n";
			    xd = xa - xw;
			    yd = ya + yw;
			}
		    }else{
			if(k3<k1){
			    cout<<"case7:"<<xw<<","<<yw<<"\n";
			    xd = xa + xw;
			    yd = ya - yw;
			}else{
			    cout<<"case8:"<<xw<<","<<yw<<"\n";
			    xd = xa - xw;
			    yd = ya + yw;
			}
		    }
		}
		cout << "xd,yd:" <<xd<<","<<yd<<"\n";
		nextTarget.setX(xd);
		nextTarget.setY(yd);

	    }
	    loopCounter++;
	}
	this->targetPoint = nextTarget;
	cout << "\n---------------------------------------------------\n";
    }

    Location getTarget(){
	return this->targetPoint;
    }

    // Get slop of CB
    double getSlop(Location B, Location C){
	double k = ((double)B.getY()-(double)C.getY())/((double)B.getX()-(double)C.getX());
	return k;
    }

    // Check if the point is out of field
    bool isOutOfField(Location aPoint){
	if(aPoint.getX()>480 
		or aPoint.getY()>480
		or aPoint.getX()<0
		or aPoint.getY()<0){
	    return true;
	}else{
	    return false;
	}
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
    // If it's clear on its way then return the target
    Location getObstacleOnTheWay(Location target){
	// check obstacles
	int minDistance = 1000;//getDistance(this->map.getOurRobot().getLocation(), target);
	Location nearestObstacle = target;
	Robot ourRobot = this->map.getOurRobot();
	// FIXME also need to check the other robot
	for(int i=0;i<this->map.getObstacles().size();i++){
	    Location obstacleLocation = this->map.getObstacles().at(i).getLocation();
	    double lineDistance = getLineDistance(obstacleLocation, ourRobot.getLocation(), target);
	    if(lineDistance < DANGER_DISTANCE and !isOutOfLine(ourRobot.getLocation(), target, obstacleLocation) ){// FOUND OBSTACLE: Obstacle radius + Robot width
		// We need find the nearest obstacle that on the way
		int distance = getDistance(ourRobot.getLocation(), obstacleLocation);
		if(distance < minDistance){
		    minDistance = distance;
		    nearestObstacle = obstacleLocation;
		}
	    }
	}
	return nearestObstacle;
    }

    // Check if the obstacle is out of real line range: A is robot, B is target, C is obstacle
    bool isOutOfLine(Location A, Location B, Location C){
	double d_ab = getDistance(A,B);
	double d_ac = getDistance(A,C);
	double d_bc = getDistance(B,C);
	if( d_ac > d_ab or d_bc > d_ab ) return true;
	else return false;
    }

    // Get distance from A to line BC
    double getLineDistance(Location A, Location B, Location C){
	// calculate k
	double k = getSlop(B,C);
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
	//FIXME xp or yp could be 0
	return sqrt(xp+yp);                                                           
    }   

    double getAngle(Location A, Location B, Location C){
	double k1 = getSlop(C,B);
	double k2 = getSlop(B,A);
	double tanAngle = (k1-k2)/(1+k1*k2);
	if(tanAngle<0) tanAngle = tanAngle*(-1);
	double angle = atan(tanAngle);
	return angle;
    }
};

#endif
