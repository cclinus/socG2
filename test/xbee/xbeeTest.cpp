#include <iostream>
#include "../../src/WirelessUnit.hpp"
using namespace std;

int main () {
    WirelessUnit xbee;
    //Test on xbee function
    xbee.send("12,100");
}
