#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <math.h>
#include "ControlUnit.hpp"
#include "Map.hpp"
#include "Location.hpp"
#include "Ball.hpp"
#include "Obstacle.hpp"
#include "Robot.hpp"

#define SAFE_WALKAROUND 80 // This value is used to walk around a obstacle without any collision
#define DANGER_DISTANCE 35 // This value is used to tell whether the robot is too closed to any obstacles
#define GRABBING_BALL_DISTANCE 20 // This value is used to tell the robot is already to grab a ball
#define SHOOTING_DISTANCE 40 // This value is used to tell the robot is to shoot when it arrives to the gate

using namespace std;

/* AI */
class Brain{
    Map map;
    Location targetPoint; // The point that the robot need to go to as a final goal
    Location targetBall; // A temp value used in analyse(), basically this holds the ball we want to reach
    int targetLock; // A temp value used in analyse() to see if need change target
    int state; // The state need to look at in analyse() and will return to outside the brain class
    WirelessUnit xbee;
    int brainCounter2, brainCounter4;

    public:

    Brain(){
	this->targetLock = 0;
	this->state = 0;
	this->brainCounter2 = 0;
	this->brainCounter4 = 0;
    }

    void sendState(int state){
	int dataSize = this->xbee.send(400+state, 0);
	cout << "\n$$$$$\nXbee send to update state: " << dataSize << "\n$$$$$\n";
    }

    // Update the state of the robot based on the events, locations from the map obj
    void updateState(Robot ourRobot){

	//STATES: 
	//0.Idle
	//1.Navigating to the ball
	//2.Grabbing the ball
	//3.Navigating to the gate
	//4.Shooting

	if(this->state == 0) this->state = 1;

	//cout << "\n*****\n" << "Current State: " << this->state << "\n*****\n";

	if( this->state == 1){
	    int distanceToBall = getDistance(this->targetBall, ourRobot.getLocation());
	    if( distanceToBall <= GRABBING_BALL_DISTANCE){
		// If the current robot location is closed to the ball, update the state to 2
		this->state = 2;
		sendState(this->state);
		cout << "\n*****\n" << "Update State to: " << this->state << "\n*****\n";
	    }
	}else if( this->state == 2){
	    // See if the ball is grabbed
	    // See the target ball is moved or disappeared
	    // FIXME need confirm the ball is in
	    for(int i=0; i<100; i++){
		sleep(0.01);
		unsigned char* msg = this->xbee.receive();
		if(msg[0] == '2'){
		    this->state = 3;
		    cout << "\n*****\n" << "Update State to: " << this->state << "\n*****\n";
		}

	    }

	    //FIXME need set a timeout
	    if(this->brainCounter2 == 30){
		this->brainCounter2 = 1;
		this->state = 1;
	    }
	    this->brainCounter2++;

	}else if( this->state == 3){
	    // Set the gate as target and adjust angle when arrived
	    int distanceToGate = getDistance(this->map.getGateLocation(), ourRobot.getLocation());
	    if( distanceToGate <= SHOOTING_DISTANCE){
		this->state = 4;
		sendState(this->state);
		cout << "\n*****\n" << "Update State to: " << this->state << "\n*****\n";
	    }
	}else if( this->state == 4){
	    // Trigger the shooting mechanism and return to state 1 at the end
	    for(int i=0; i<100; i++){
		sleep(0.01);
		unsigned char* msg = this->xbee.receive();
		if(msg[0] == 4){
		    this->state = 1;
		    cout << "\n*****\n" << "Update State to: " << this->state << "\n*****\n";
		}
	    }
	    if(this->brainCounter4 == 30){
		this->brainCounter4 = 0;
		this->state = 1;
	    }
	    this->brainCounter4++;
	}

    }

    int getState(){
	return this->state;
    }

    // Find where the robot need to go based on the state
    void analyse(Map aMap){
	this->map = aMap; // Update a new map for this cycle
	vector<Ball> ballVector = map.getBalls();
	vector<Obstacle> obstacleVector = this->map.getObstacles();
	Robot ourRobot = this->map.getOurRobot();
	Location nextTarget = ourRobot.getLocation();

	// TODO Remove objs with negative x,y
	// Need filter all negative
	if(ourRobot.getLocation().getX()<0 or ourRobot.getLocation().getY()<0) return;

	// Change the target according to the state
	updateState(ourRobot);
	if(this->state == 1){
	    nextTarget = getNearestBall();
	}else if(this->state == 3){
	    nextTarget = this->map.getGateLocation();
	}

	if(this->targetLock == 1){ // Target is locked, do not need to change target
	    // Check if the nearest ball is still the one
	    if(nextTarget.getX() == this->targetBall.getX() and nextTarget.getY() == this->targetBall.getY()){
		// Check if the path is still clear
		Location obstacleOnTheWay = getObstacleOnTheWay(this->targetPoint);
		if(obstacleOnTheWay.getX()==this->targetPoint.getX() and obstacleOnTheWay.getY()==this->targetPoint.getY()){
		    // Check if it is closed already
		    int distance = getDistance(this->targetPoint, ourRobot.getLocation());
		    if(distance<DANGER_DISTANCE){
			this->targetLock = 0;
		    }else{
			//cout << "Target Distance:"<<distance<<"\n";
		    }
		    return;
		}else{
		    this->targetLock = 0;
		}

	    }else{
		this->targetLock = 0;
	    }

	}else{ // Target is unlocked so we recalculate target

	    this->targetBall = nextTarget;
	    // Check if there is obj on its way
	    int loopCounter = 0;
	    while(loopCounter < 30){
		//cout << "\n*****Loop Count "<<loopCounter<<"*****\n";
		Location obstacleOnTheWay = getObstacleOnTheWay(nextTarget);
		//cout << "Nearest Ball (target): ("<<nextTarget.getX()<<","<<nextTarget.getY()<<")\n";
		//cout << "Robot Location: (" << ourRobot.getLocation().getX() << "," << ourRobot.getLocation().getY() << ")\n";
		if(obstacleOnTheWay.getX() == nextTarget.getX() and obstacleOnTheWay.getY() == nextTarget.getY()){
		    //If there's no obj on the way
		    break;
		}else{
		    // cout << "Obs on the way: (" << obstacleOnTheWay.getX() << "," << obstacleOnTheWay.getY() << ")\n";
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
		    //cout << "sin,cos:"<< sinA <<","<< cosA <<"\n";
		    //cout << "alpha1,alpha2:"<<alpha1<<","<<alpha2<<"\n";
		    // Decide which way to turn
		    double k3 = getSlop(obstacleOnTheWay, ourRobot.getLocation());
		    //cout << "k1, k2, k3:"<< k1 <<","<< k2 << "," << k3 <<"\n";
		    // We can walk around if alpha2 is too small
		    int xw = SAFE_WALKAROUND*cosA;
		    int yw = SAFE_WALKAROUND*sinA;
		    int xd, yd;

		    if(alpha1>=90 and alpha1<180){
			if(ourRobot.getLocation().getX()<nextTarget.getX()){
			    if(k3>k1){
				//cout<<"case1:"<<xw<<","<<yw<<"\n";
				xd = xa - xw;
				yd = ya - yw;
			    }else{
				//cout<<"case2:"<<xw<<","<<yw<<"\n";
				xd = xa + xw;
				yd = ya + yw;
			    }
			}else{
			    if(k3<k1){
				//cout<<"case3:"<<xw<<","<<yw<<"\n";
				xd = xa - xw;
				yd = ya - yw;
			    }else{
				//cout<<"case4:"<<xw<<","<<yw<<"\n";
				xd = xa + xw;
				yd = ya + yw;
			    }
			}
		    }else if(alpha1<90 and alpha1>0){
			if(ourRobot.getLocation().getX()<nextTarget.getX()){
			    if(k3>k1){
				//cout<<"case5:"<<xw<<","<<yw<<"\n";
				xd = xa + xw;
				yd = ya - yw;
			    }else{
				//cout<<"case6:"<<xw<<","<<yw<<"\n";
				xd = xa - xw;
				yd = ya + yw;
			    }
			}else{
			    if(k3<k1){
				//cout<<"case7:"<<xw<<","<<yw<<"\n";
				xd = xa + xw;
				yd = ya - yw;
			    }else{
				//cout<<"case8:"<<xw<<","<<yw<<"\n";
				xd = xa - xw;
				yd = ya + yw;
			    }
			}
		    }
		    //cout << "xd,yd:" <<xd<<","<<yd<<"\n";
		    nextTarget.setX(xd);
		    nextTarget.setY(yd);
		    this->targetLock = 1;

		}
		loopCounter++;
	    }
	    this->targetPoint = nextTarget;
	    //cout << "\n---------------------------------------------------\n";
	}
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
