#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include <stdio.h>
#include "Location.hpp"
#include "Robot.hpp"
#include <iostream>
#include <stdlib.h>
#include "WirelessUnit.hpp"
#include "Map.hpp"
#include <math.h>

using namespace std;

#define PI 3.1415926
#define ANGLE_TOLERANCE 25 // This value is used to tell whether wee need adjust robot
#define DISTANCE_TOLERANCE 15 //This value is used to tell whether we need adjust robot
#define CYCLE_DISTANCE 56 // This is the scale between pixel and wheel cycle
#define ADJUST_FREQUENCY 30// This value is used to control how frequent we adjust robot

/*
 *  STATES:
 *
 *  IDLE = 0;
 *  NAVIGATING_BALL = 1;
 *  GREBBING = 2;
 *  NAVIGATING_GATE = 3;
 *  SHOOTING = 4;
 */

class ControlUnit{
    WirelessUnit xbee;
    Location target;
    Map map;
    int controlCounter;

    public:

    ControlUnit(){
	this->controlCounter = 0;
    }

    // Based on the path, see if we need send command to robot
    // Call after brain analyse()
    void adjust(Map aMap, Location aTarget){
	// Make sure it gives sometime to robot to adjust
	//cout << "\n(Control)Control counter: "<<this->controlCounter<<"\n";

	if( this->controlCounter % ADJUST_FREQUENCY == 0 ){
	    this->map = aMap;
	    this->target = aTarget;
	    // Check if angle is too different
	    Robot ourRobot = this->map.getOurRobot();
	    double angle = getAngle(ourRobot.getLocation(), ourRobot.getLocationB(), aTarget);
	    int distance = getDistance(ourRobot.getLocation(), aTarget);
	    //cout << "\n(Control)Angle difference: "<<angle<<"\n";
	    // See if it is out of path, we need update the robot the right angle
	    if(angle > ANGLE_TOLERANCE or angle < (-1)*ANGLE_TOLERANCE
		or distance > DISTANCE_TOLERANCE 
		){
		int cycle = distance/CYCLE_DISTANCE;
		if(cycle == 0) cycle = 1;
		// We need convert the negative value
		// We try to handle this in WirelessUnit level
		//if(angle<0) angle += 360;
		send(angle,cycle);
		cout << "\n(Control)angle,cycle:"<<angle<<","<<cycle<<"\n";
	    }
	}
	this->controlCounter++;

    }

    // rA and rB are robot direction, C is the target location
    // Return the angle difference between robot direction and robot-target
    double getAngle(Location rA, Location rB, Location C){
	double x1 = rA.getX();
	//cout<<"\nx1:"<<x1<<"\n";
	double y1 = rA.getY();
	//cout<<"\ny1:"<<y1<<"\n";
	double x2 = rB.getX();
	//cout<<"\nx2:"<<x2<<"\n";
	double y2 = rB.getY();
	//cout<<"\ny2:"<<y2<<"\n";
	double x3 = C.getX();
	//cout<<"\nx3:"<<x3<<"\n";
	double y3 = C.getY();
	//cout<<"\ny3:"<<y3<<"\n";
	double a1 = atan2(y1-y2,x1-x2)*180/PI;
	if(a1<0) a1=360+a1;
	double a2 = atan2(y3-y1,x3-x1)*180/PI;
	if(a2<0) a2=360+a2;
	return a1-a2;
    }


    /*
     * Command: angle(>360: state update) cycle
     * Send to xbee
     */
    void send(int angle, int cycle){
	int dataSize = this->xbee.send(angle, cycle);
	cout << "\n$$$$$\nXbee send to update angle and cycle: " << dataSize << "\n$$$$$\n";
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
	return abs(sqrt(xp+yp));
    }

};

#endif
