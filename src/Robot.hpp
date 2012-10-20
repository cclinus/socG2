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
        location.setX(a);
        location.setY(b);
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

};

#endif
