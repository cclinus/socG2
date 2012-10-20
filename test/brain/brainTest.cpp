#include <iostream>
#include <stdio.h>
#include "../../src/Brain.hpp"
#include "../../src/Map.hpp"

using namespace std;

int main () {

    // Test on full map
    Map map(480, 480);
    vector<Ball> ballVector;
    vector<Obstacle> obstacleVector;
    Brain brain;

    // Add some test objects
    Ball aBall(100,100,1);
    map.addBall(aBall);
    Ball bBall(200,200,2);
    map.addBall(bBall);
    Obstacle aObstacle(150,150);
    map.addObstacle(aObstacle);
    Obstacle bObstacle(180,60);
    map.addObstacle(bObstacle);
    Obstacle cObstacle(250,250);
    map.addObstacle(cObstacle);
    Obstacle dObstacle(319,260);
    map.addObstacle(dObstacle);

    ballVector = map.getBalls();
    obstacleVector = map.getObstacles();

    // Add our own robot
    Robot ourRobot(300,300,1);
    map.addRobot(ourRobot);

    cout << "\n\n" << "Ball Count: " << map.countBalls() << "\n";
    cout << "Obstacle Count: " << map.countObstacles() << "\n";

    // Get all positions of objects
    cout << "\nShow all balls positions: \n";
    for(int i=0; i<ballVector.size(); i++){
        Ball ball = ballVector.at(i);
        Location ballLocation = ball.getLocation();
        int x = ballLocation.getX();
        int y = ballLocation.getY();
        cout << " (" << x <<  "," << y << ") \n";
    }

    cout << "\nShow all obstacles positions: \n";
    for(int i=0; i<obstacleVector.size(); i++){
        Obstacle obstacle = obstacleVector.at(i);
        Location obstacleLocation = obstacle.getLocation();
        int x = obstacleLocation.getX();
        int y = obstacleLocation.getY();
        cout << " (" << x <<  "," << y << ") \n";
    }

    cout << "\nShow robot position:\n";
    Location robotLocation = ourRobot.getLocation();
    int x = robotLocation.getX();
    int y = robotLocation.getY();
    cout << " (" << x << "," << y << ")\n";

    // Test getDistance(): get distance between aBall and ourRobot
    int distance = brain.getDistance(bBall.getLocation(), ourRobot.getLocation());
    cout << "\nDistance between aBall and ourRobot: " << distance << "\n";

    // Test analyse()
    brain.analyse(map);
    Location nearestBallLocation = brain.getTarget();
    cout << "\nTarget Point: (" << nearestBallLocation.getX() << "," << nearestBallLocation.getY() << ")\n";

    // Test getLineDistance()
    int lineDistance = brain.getLineDistance(bObstacle.getLocation(), aBall.getLocation(), ourRobot.getLocation());
    cout << "\nLine Distance:" << lineDistance << "\n\n\n";

    return 0;
}
