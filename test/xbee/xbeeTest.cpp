#include <iostream>
#include "../../src/WirelessUnit.hpp"
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main () {
    WirelessUnit xbee;

    int angle = 0;
    int cycle = 0;

    for(int i=0; i<20; i++){
	angle++;
	cycle++;

	char msg1[3];
	sprintf(msg1, "%d", angle);

	char msg2[2];
	sprintf(msg2, "%d", cycle);

	strcat(msg1, " ");
	strcat(msg1, msg2);

	int dataSize = xbee.send(msg1);
	cout << dataSize << "\n";
	sleep(1);
    }
}
