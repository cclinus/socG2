#ifndef WIRELESSUNIT_HPP
#define WIRELESSUNIT_HPP

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rs232.h"
#include "rs232.c"

using namespace std;

class WirelessUnit{
    public:
	WirelessUnit(){
	}

	int send(int angle, int cycle){

	    // Convert negative angle
	    if(angle<0) angle += 360;
	    
            cout << "Xbee layer print angle: " << angle << ", " << cycle << "\n";

	    int i, n,
		cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
		bdrate = 57600;       /* 57600 baud */

	    if(OpenComport(cport_nr, bdrate)){
		printf("Can not open comport\n");
		return 0;
	    }

	    char angleChar[4];
	    char cycleChar[2];
	    sprintf(angleChar, "%3d", angle);
	    sprintf(cycleChar, "%2d", cycle);

	    angleChar[3] = ' ';
	    //cycleChar[2] = ' ';

	    unsigned char msg[6];
	    for (int i=0; i<=5; i++){
		if(i <= 3){
		    msg[i] = angleChar[i];
		}else if(i > 3){
		    msg[i] = cycleChar[i-4];
		}
	    }

	    return SendBuf(cport_nr, msg, 6);
	}

	unsigned char receive(void){
	    int i, n,
		cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
		bdrate=57600;       /* 9600 baud */
	    unsigned char buf[1];
	    if(OpenComport(cport_nr, bdrate)){
		printf("Can not open comport\n");
	    }else{
		//while(1){
		for(int i=0;i<2;i++){ 
		    n = PollComport(cport_nr, buf, 1);
		    if(n > 0){
			buf[n] = 0;   /* always put a "null" at the end of a string! */
			printf("received %i bytes: %s\n", n, (char *)buf);
			return buf[0];
		    }
		}
	    }

	    return '2'; //2: Nothing received;
	}

};
#endif
