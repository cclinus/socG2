#include <iostream>
#include <stdio.h>
#include "../src/Brain.hpp"
#include "../src/Map.hpp"

using namespace std;

int main () {

    // Test on full map
    Map map(480, 480);
    vector<Ball> ballVector;
    vector<Obstacle> obstacleVector;

    // Add some test objects
    Ball aBall(100,100,1);
    map.addBall(aBall);
    Ball bBall(200,200,2);
    map.addBall(bBall);
    Obstacle aObstacle(150,150);
    map.addObstacle(aObstacle);

    ballVector = map.getBalls();
    obstacleVector = map.getObstacles();

    // Add our own robot
    Robot ourRobot(300,300,1);

    cout << "Ball Count: " << map.countBalls() << "\n";
    cout << "Obstacle Count: " << map.countObstacles() << "\n";

    cout << "Show all balls positions: \n";
    // Get all balls position
    for(int i=0; i<ballVector.size(); i++){
        Ball ball = ballVector.at(i);
        Location ballLocation = ball.getLocation();
        int x = ballLocation.getX();
        int y = ballLocation.getY();
        cout << " (" << x <<  "," << y << ") \n";
    }

    return 0;

}
