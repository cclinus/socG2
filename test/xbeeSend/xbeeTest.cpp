#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/WirelessUnit.hpp"

using namespace std;

int main () {
    WirelessUnit xbee;

    int angle = 404;
    int cycle = 3;

    printf("Angle: %d\n", angle);
    printf("Cycle: %d\n", cycle);

    int dataSize = xbee.send(angle, cycle);
    cout << dataSize << "\n";
    sleep(0.3);
}
