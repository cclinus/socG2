#include <iostream>
#include "Camera.hpp"
#include "Map.hpp"

using namespace std;

/* main loop */
int main () {
    
    //FIXME: actual map size in coordinates?
    int mapWidth = 1000;
    int mapHeigth = 1000;
    Map map(mapWidth, mapHeigth);

    // We put cameras, they atteched to the only map obj here
    // FIXME: numbering the cams
    Camera camOne(1, map);
    Camera camTwo(2, map);

    //TODO: put a timer here indicating 1/x second of the loop
    while(1){// Assuming this is the timer for now

        // In every 1/x, we init all objs' locations
        camOne.findObjects();

        /* ... some actions go here */

    }
    
}

