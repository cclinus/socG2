#include <iostream>
#include <stdio.h>
#include "../../src/WirelessUnit.hpp"
using namespace std;

int main () {
	WirelessUnit xbee;
	//Test on xbee function
	int a;
	int dataSize = xbee.send("120");
	cout << dataSize << "\n";
}
