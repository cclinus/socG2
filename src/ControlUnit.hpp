#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include <stdio.h>
#include "Location.hpp"
#include "Robot.hpp"
#include <iostream>
#include <stdlib.h>
#include "WirelessUnit.hpp"
#include "Robot.hpp"

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
    
    public:
        
        ControlUnit(){}

	// Based on the path, see if we need send command to robot
	void adjust(Map map, Location target){
	    this->map = map;
	    this->target = target;
	    
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
