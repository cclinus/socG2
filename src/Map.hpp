#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <stdio.h>
#include <vector>

#include "Location.hpp"
#include "Obstacle.hpp"
#include "Ball.hpp"
#include "Robot.hpp"

using namespace std;

class Map {
    int width, height;
    Location gate;
    vector<Ball> ballVector;
    vector<Obstacle> obstacleVector;
    vector<Robot> robotVector;

    public:

    Map(){}

    Map(int w, int h){
        width = w;
        height = h;
        //FIXME: what is gate coordinates?
        this->gate.setX(0);
        this->gate.setY(0);
    }

    void addBall(Ball aBall){
        this->ballVector.push_back(aBall);
    }

    void addObstacle(Obstacle anObstacle){
        this->obstacleVector.push_back(anObstacle);
    }

    void addRobot(Robot aRobot){
        this->robotVector.push_back(aRobot);
    }

    vector<Ball> getBalls(void){
        return this->ballVector;
    }

    vector<Obstacle> getObstacles(void){
        return this->obstacleVector;
    }

    Robot getOurRobot(void){
        for(int i = 0; i < this->robotVector.size(); i++){
            // Our robot NO. is 1
            if(this->robotVector.at(i).getNo() == 1){
                return this->robotVector.at(i);
            }
        }
    }

    Robot getEnemyRobot(void){
        for(int i = 0; i < this->robotVector.size(); i++){
            // Enemy robot NO. is 2
            if(this->robotVector.at(i).getNo() == 2){
                return this->robotVector.at(i);
            }
        }
    }

    void clearMap(void){
        this->ballVector.clear();
        this->obstacleVector.clear();
        this->robotVector.clear();
    }

    // Return the location of target gate
    Location getGateLocation(void){
        return this->gate;
    }

    // Return the nearest ball from our robot
    Ball getNearestBall(void){
        return this->ballVector.front();
    }

};

#endif
