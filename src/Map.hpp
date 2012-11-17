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
    int gateNo;
    Location gate;
    vector<Ball> ballVector;
    vector<Obstacle> obstacleVector;
    vector<Robot> robotVector;

    public:

    Map(){
        this->gate.setX(240);
        this->gate.setY(20);
	this->gateNo = 1;
    }

    Map(int w, int h){
        width = w;
        height = h;
        //FIXME: what is gate coordinates?
	this->gateNo = 1;
        this->gate.setX(240);
        this->gate.setY(20);
    }

    bool isNormal(){
	Robot ourRobot = getOurRobot();
	if(ourRobot.getLocation().getX()<0 or ourRobot.getLocation().getY()<0){
	    // Lost robot
	    return false;
	}
	return true;
    }

    // Gate 1 is defaul gate near (0,0)
    void changeGate(int gateNo){
	if(gateNo == 2){
	    this->gate.setX(240);
	    this->gate.setY(460);
	}else if(gateNo == 1){
	    this->gate.setX(240);
	    this->gate.setY(20);
	}
    }

    int getGateNo(){
	return this->gateNo;
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

    // For testing
    void moveRobot(Location target){
        for(int i = 0; i < this->robotVector.size(); i++){
            // Our robot NO. is 1
            if( this->robotVector.at(i).getNo() == 1 ){
                this->robotVector.at(i).moveHalf(target);
            }
        }
    }

    Robot getOurRobot(void){
        for(int i = 0; i < this->robotVector.size(); i++){
            // Our robot NO. is 1
            Robot robot = this->robotVector.at(i);
            if(robot.getNo() == 1){
                return this->robotVector.at(i);
            }
        }
    }

    Robot getEnemyRobot(void){
        for(int i = 0; i < this->robotVector.size(); i++){
            // Enemy robot NO. is 2
            Robot robot = this->robotVector.at(i);
            if(robot.getNo() == 2){
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

    int countBalls(void){
        return this->ballVector.size();
    }

    int countObstacles(void){
        return this->obstacleVector.size();
    }

};

#endif
