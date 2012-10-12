#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>
#include <iostream>
#include "Map.hpp"

using namespace std;

/* Image processing code */
class Camera {

    int cameraNo;
    Map map;

    public:

    // The Camera obj has to be atteched to the map obj
    Camera(int no, Map &aMap){
        cameraNo = no;
        // Init the map pointer to the map obj
        // Since then every operations on map will be on the actual map obj
        this->map = aMap;
    }
    
    // Core processing function, init all balls, obstacles, robots to the map obj.
    // Feel free to create additional func within this class.
    void updateMap(void){
        /* Image Processing and Camera connections go here.
         * During the processing, once we find a ball for example,
         * we can do:
         *      Ball aBall(x, y, ballNo);
         *      map.addBall(aBall);
         * to add an ball obj to the map.
         * Same to obstacles and robot, refer to Ball.hpp, Robot.hpp and Obstacle.hpp
         */
        Ball aBall(10,20,1);
        map.addBall(aBall);

        Ball bBall(15,25,2);
        map.addBall(bBall);

    }

};

#endif
