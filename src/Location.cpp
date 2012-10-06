#include <iostream>
#include <stdio.h>

using namespace std;

class Location{
    int x, y;
    
    public:

        Location(){
        
        }
    
        Location(int a, int b){
            x = a;
            y = b;
        }

        int getX(void){
            return x;
        }

        int getY(void){
            return y;
        }

        void setX(int a){
            x = a;
        }

        void setY(int b){
            y = b;
        }
};
