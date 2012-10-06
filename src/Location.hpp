#include <iostream>
#include <stdio.h>

#ifndef LOCATION_H
#define LOCATION_H

using namespace std;

class Location{
    int x, y;
    
    public:

        Location();
        
        Location(int, int);

        int getX(void);

        int getY(void);

        void setX(int);

        void setY(int);
};

#endif
