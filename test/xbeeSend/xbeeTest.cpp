#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/WirelessUnit.hpp"

using namespace std;

int main () {
    WirelessUnit xbee;

    while(1){
	int angle;
	int cycle;
	cout << "Enter angle and cycle:\n";
	scanf("%d", &angle);
	scanf("%d", &cycle);

	printf("Angle: %d\n", angle);
	printf("Cycle: %d\n", cycle);

	int dataSize = xbee.send(angle, cycle);
	cout << dataSize << "\n";
    }
}
