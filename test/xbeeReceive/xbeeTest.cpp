#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/WirelessUnit.hpp"

using namespace std;

int main () {
    WirelessUnit xbee;

    cout << xbee.receive() << "\n";
}
