#ifndef BALL_HPP
#define BALL_HPP

#include <stdio.h>
#include <iostream>
#include "Location.hpp"

using namespace std;

class Ball {
    int ballNo;
    Location location;
    
    public:
    
    Ball(int a, int b, int No){
        location.setX(a);
        location.setY(b);
        ballNo = No;
    }

    Location getLocation(void){
        return this->location;
    }

    int getNo(void){
        return ballNo;
    }

};

#endif
