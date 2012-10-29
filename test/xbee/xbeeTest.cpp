#include <iostream>
#include "../../src/WirelessUnit.hpp"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main () {
    WirelessUnit xbee;

    int angle = 010;
    int cycle = 01;

    for(int i=0; i<20; i++){
	angle++;
	cycle++;
	printf("Angle: %d\n", angle);
	printf("Cycle: %d\n", cycle);

	int dataSize = xbee.send(angle, cycle);
	cout << dataSize << "\n";
	sleep(1);
    }
}
