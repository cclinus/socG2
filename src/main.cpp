#include <iostream>
#include "Camera.hpp"
#include "Map.hpp"
#include "ControlUnit.hpp"

using namespace std;

/* main loop */
int main () {
    
    //FIXME: actual map size in coordinates?
    int mapWidth = 1000;
    int mapHeight = 1000;
    Map map(mapWidth, mapHeight);

    //Brain brain;
    //ControlUnit control;

    // We put two cameras, they atteched to the only map obj here
    // FIXME: numbering the cams
//    Camera camOne(1, map);
//    Camera camTwo(2, map);

    //TODO: put a timer here indicating 1/x second of the loop
    while(1){// Assuming this is the timer for now

        // In every 1/x, we init all objs' locations and the map obj is updated
//        camOne.updateMap();
//        camTwo.updateMap();

        /*

          brain.analyse(map);
          int state = brain.getState()
          Location targetPoint = brain.getTarget();
          control.setTarget(targetPoint);
          control.updateState(state);
          control.send();
         
         */

    }
    
}

