#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <stdio.h>
#include <iostream>
#include "Location.hpp"

using namespace std;

class Obstacle {
    Location location;
    int diameter; // In pixel

    public:

    Obstacle(int a, int b){
        location.setX(a);
        location.setY(b);
	this->diameter = 23;
    }

    Location getLocation(void){
        return location;
    }

    int getDiameter(void){
	return this->diameter;
    }
    
};

#endif
