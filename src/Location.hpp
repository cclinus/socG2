#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

class Location{
    int x, y;

    public:

    Location(){}

    Location(int a, int b){
        x = a;
        y = b;
    }

    int getX(void){
        return x;
    }

    int getY(void){
        return y;
    }

    void setX(int a){
        this->x = a;
    }

    void setY(int b){
        this->y = b;
    }

};

#endif
