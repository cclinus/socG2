#include <stdio.h>
#include <iostream>
#include "Location.hpp"

using namespace std;

class Obstacle {
    Location location;

    public:

    Obstacle(int a, int b){
        location.setX(a);
        location.setY(b);
    }

    Location getLocation(void){
        return location;
    }

};
