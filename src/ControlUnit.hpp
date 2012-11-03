#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include <stdio.h>
#include "Location.hpp"
#include "Robot.hpp"
#include <iostream>
#include <stdlib.h>
#include "WirelessUnit.hpp"
#include "Map.hpp"
#include "Brain.hpp"
#include <math.h>

using namespace std;

#define PI 3.1415926

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
    int angle;
    int state;

    public:

    ControlUnit(){
	this->state = 0;
	this->angle = 0;
    }

    // Based on the path, see if we need send command to robot
    // Call after brain analyse()
    void adjust(Map aMap, Location aTarget, int state){
	this->map = aMap;
	this->target = aTarget;
	// Check if angle is too different
	Robot ourRobot = this->map.getOurRobot();
	double angle = getAngle(ourRobot.getLocation(), ourRobot.getLocationB(), aTarget);
	cout << "\n\nAngle difference: "<<angle<<"\n\n";

    }

    // rA and rB are robot direction, C is the target location
    // Return the angle difference between robot direction and robot-target
    double getAngle(Location rA, Location rB, Location C){
	double x1 = rA.getX();
	cout<<"\nx1:"<<x1<<"\n";
	double y1 = rA.getY();
	cout<<"\ny1:"<<y1<<"\n";
	double x2 = rB.getX();
	cout<<"\nx2:"<<x2<<"\n";
	double y2 = rB.getY();
	cout<<"\ny2:"<<y2<<"\n";
	double x3 = C.getX();
	cout<<"\nx3:"<<x3<<"\n";
	double y3 = C.getY();
	cout<<"\ny3:"<<y3<<"\n";

	double a1 = atan2(y1-y2,x1-x2)*180/PI;
	if(a1<0) a1=360+a1;
	double a2 = atan2(y3-y2,x3-x2)*180/PI;
	if(a2<0) a2=360+a2;

	return a1-a2;
    }


    /*
     * Command: angle(>360: state update) cycle
     */
    void send(int angle, int cycle){
	int dataSize = this->xbee.send(angle, cycle);
	cout << "\n\n$$$$$\nXbee send: " << dataSize << "\n$$$$$\n\n";
    }

};

#endif
