//Robot Keyboard control with wasd and arrow keys
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Keyboard.hpp>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <fstream>

#define TIME_STEP 64
//Define Speed here
#define D_SPEED 3
#define T_SPEED 3 
using namespace webots;
using namespace std;

int main(int argc, char **argv) {
  //Define Robot
  Robot *robot = new Robot();
  //Define Keyboard
  Keyboard K;
  K.enable(TIME_STEP);
  //Define Motors
  Motor* LMotor = robot->getMotor("left wheel motor");
  Motor* RMotor = robot->getMotor("right wheel motor");
  double LDist = 0.0;
  double RDist = 0.0;
  LMotor->setPosition(INFINITY);
  RMotor->setPosition(INFINITY);
  LMotor->setVelocity(0);
  RMotor->setVelocity(0);  
    
  
  while (robot->step(TIME_STEP) != -1) {
        
        //Check for control key to be pressed
        int q=K.getKey();
        
        //Move based on key
        if(q==315||q==87){
             LDist=D_SPEED;
             RDist=D_SPEED;
        }
        else if(q==317||q==83){
             LDist=-D_SPEED;
             RDist=-D_SPEED;   
        }
        else if(q==316||q==68){
             LDist=T_SPEED;
             RDist=-T_SPEED;   
        }else if(q==314||q==65){
             LDist=-T_SPEED;
             RDist=T_SPEED;   
        }
        else{
             LDist=0;
             RDist=0;   
        }
        
        //Drive
        robot->step(TIME_STEP);
        LMotor->setVelocity(LDist);
        RMotor->setVelocity(RDist);
  };
  
  delete robot;
  return 0;
}
