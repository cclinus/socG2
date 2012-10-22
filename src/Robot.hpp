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
    Location location;
    
    public:
    Robot(){
	this->width = 25;
	this->length = 25;
    }

    Robot (int a, int b, int No){
        this->location.setX(a);
        this->location.setY(b);
        // Our robot NO. is 1
        robotNo = No;
	this->width = 25;
	this->length = 25;
    }

    int getNo(void){
        return robotNo;
    }

    Location getLocation(void){
        return location;
    }

    // For testing
    void moveHalf(Location target){
	int xt = ( target.getX() - this->location.getX() ) / 5 + this->location.getX();
	int yt = ( target.getY() - this->location.getY() ) / 5 + this->location.getY();
	this->location.setX(xt);
	this->location.setY(yt);
    }

    void moveFull(Location target){
	int xt = target.getX();
	int yt = target.getY();
	this->location.setX(xt);
	this->location.setY(yt);
    }

};

#endif
