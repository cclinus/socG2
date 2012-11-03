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

using namespace std;

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
	    // Check if angle is too different
	    Robot ourRobot = this->map.getOurRobot();
	    int angle = getAngle(ourRobot.getLocation(), ourRobot.getLocationB(), aTarget);
	    cout << "\n\nAngle difference: "<<angle<<"\n\n";

	    this->target = aTarget;
	    
	}

	// rA and rB are robot direction, C is the target location
	// Return the angle difference between robot direction and robot-target
	int getAngle(Location rA, Location rB, Location C){
	    Brain tempBrain;
	    // Slop of robot direction
	    double robotSlop = tempBrain.getSlop(rA, rB);
	    // Angle between robot direction and positive X axis
	    double robotAngle = atan(robotSlop);
	    // Slop of robot-target
	    double robotTargetSlop = tempBrain.getSlop(rA, C);
	    // Angle between robot-target and positive X axis
	    double robotTargetAngle = atan(robotTargetSlop);
	    // Angle difference
	    int angle = robotAngle - robotTargetAngle;
	    return angle;
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
