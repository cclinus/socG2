#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <stdio.h>
#include "Location.hpp"

using namespace std;

class Robot {
    int width;
    int length;
    int robotNo;
    Location locationA; //We treat this point as the center of the robot
    Location locationB;
    
    public:
    Robot(){
	this->width = 25;
	this->length = 25;
    }

    Robot (int a, int b, int No){
        this->locationA.setX(a);
        this->locationA.setY(b);
        // Our robot NO. is 1
        robotNo = No;
	this->width = 25;
	this->length = 25;
    }

    void addLocationB(int a, int b){
	this->locationB.setX(a);
	this->locationB.setY(b);
    }

    int getNo(void){
        return robotNo;
    }

    Location getLocation(void){
        return locationA;
    }

    // For testing
    void moveHalf(Location target){
	int xt = ( target.getX() - this->locationA.getX() ) / 5 + this->locationA.getX();
	int yt = ( target.getY() - this->locationA.getY() ) / 5 + this->locationA.getY();
	this->locationA.setX(xt);
	this->locationA.setY(yt);
    }

    void moveFull(Location target){
	int xt = target.getX();
	int yt = target.getY();
	this->locationA.setX(xt);
	this->locationA.setY(yt);
    }

};

#endif
