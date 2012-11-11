#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/WirelessUnit.hpp"

using namespace std;

int main () {
    WirelessUnit xbee;
    int angle = 1;
    int cycle = 1;


    while(1){

	cout << "Sent: \n";
	printf("Angle: %d\n", angle);
	printf("Cycle: %d\n", cycle);

	int dataSize = xbee.send(angle, cycle);
/*
	while(1){
	    cout << "Waiting for return ... \n";
	    cout << "Received: " << xbee.receive() << "\n\n";
	}*/
	angle++;
	cycle++;
	sleep(2);
    }
}
