#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <stdio.h>
#include "Location.hpp"

using namespace std;

class Robot {
    int robotNo;
    Location location;
    
    public:
    
    Robot(){}

    Robot (int a, int b, int No){
        location.setX(a);
        location.setY(b);
        // Our robot NO. is 1
        robotNo = No;
    }

    int getNo(void){
        return robotNo;
    }

    Location getLocation(void){
        return location;
    }

};

#endif
