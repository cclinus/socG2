#ifndef CONTROLUNIT_HPP
#define CONTROLUNIT_HPP

#include <stdio.h>
#include "Location.hpp"
#include "Robot.hpp"
#include "WirelessUnit.hpp"

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
    Robot robot;
    WirelessUnit xbeeUnit;
    int state; 
    Location target;
    
    public:
        
        ControlUnit(Robot robot){
            this->robot = robot;
            this->state = 0; // init to idle
            // TODO: init xbee
        }

        void startRobot(void){
            this->state = 1;
        }

        void stopRobot(void){
            this->state = 0;
        }

        void updateState(int stateNo){
            this->state = stateNo;
        }

        int getState(void){
            return this->state;
        }

        int setTarget(Location target){
            this->target = target;
        }

        /*
         * Command: currentX,currentY,targetX,targetY,state
         */
        void send(){

        }

};

#endif
