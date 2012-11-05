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
#define ANGLE_TOLERANCE 5
#define CYCLE_DISTANCE 49

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

    public:

    ControlUnit(){}

    // Based on the path, see if we need send command to robot
    // Call after brain analyse()
    void adjust(Map aMap, Location aTarget){
	this->map = aMap;
	this->target = aTarget;
	// Check if angle is too different
	Robot ourRobot = this->map.getOurRobot();
	double angle = getAngle(ourRobot.getLocation(), ourRobot.getLocationB(), aTarget);
	cout << "\n\n====>:Angle difference: "<<angle<<"\n\n";
	// See if it is out of path, we need update the robot the right angle
	if(angle > ANGLE_TOLERANCE or angle < (-1)*ANGLE_TOLERANCE){
	    int distance = getDistance(ourRobot.getLocation(), aTarget);
	    int cycle = distance/CYCLE_DISTANCE;
	    send(angle,cycle);
	    // We need give the robot some time to adjust
	    sleep(1.5);
	}

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
