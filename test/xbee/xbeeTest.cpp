#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/WirelessUnit.hpp"

using namespace std;

int main () {
    WirelessUnit xbee;

    int angle = 140;
    int cycle = 10;

    //for(int i=0; i<20; i++){
    printf("Angle: %d\n", angle);
    printf("Cycle: %d\n", cycle);

    int dataSize = xbee.send(angle, cycle);
    cout << dataSize << "\n";
    sleep(0.3);
    //}
}
