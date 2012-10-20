#include <iostream>
#include <stdio.h>
#include "../../src/Camera.hpp"
#include "../../src/Map.hpp"

using namespace std;

int main () {

    Map map(480, 240);
    Camera cameraOne(1);
    CameraOne.Init();
	
    map = cameraOne.updateMap(map);
    cout << "Ball Count: " << map.countBalls() << "\n";
   
    return 0;

}
