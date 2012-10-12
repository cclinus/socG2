#include <iostream>
#include <stdio.h>
#include "../src/Camera.hpp"
#include "../src/Map.hpp"

using namespace std;

int main () {
    Map map(480, 240);
    Camera cameraOne(1,map);

    cameraOne.updateMap();

    cout << "Ball Count: " << map.countBalls() << "\n";

    return 0;
}
