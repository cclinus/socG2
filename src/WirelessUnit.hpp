#ifndef WIRELESSUNIT_HPP
#define WIRELESSUNIT_HPP

#include <string>
#include <stdio.h>
#include <iostream>
#include "rs232.h"
#include "rs232.c"


using namespace std;

class WirelessUnit{
    public:
	WirelessUnit(){
	}

	int send(string angle){

	    int i, n,
		cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
		bdrate = 57600;       /* 57600 baud */

	    if(OpenComport(cport_nr, bdrate)){
		printf("Can not open comport\n");
		return 0;
	    }

	    int TempNumOne = angle.size();
	    unsigned char msg[TempNumOne];
	    for (int i=0; i<=TempNumOne; i++){
		msg[i] = angle[i];
	    }

	    return SendBuf(cport_nr, msg, TempNumOne);
	}
};
#endif
