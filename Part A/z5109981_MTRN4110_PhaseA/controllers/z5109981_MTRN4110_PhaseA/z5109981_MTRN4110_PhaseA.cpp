// File:          z5109981_MTRN4110_PhaseA.cpp
// Date:          20.06.20
// Description:   Assignment Part 1
// Author:        Samson Banzi
// Modifications:

//Header Files
#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Motor.hpp>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <stdlib.h> 

//Definitions
#define TIME_STEP 64
#define PI 3.14159
#define LINPOS 165.0/20.0
#define ROTPOS 28.3*PI/40.0+0.15
//New Path
//#define PATH "./../PathPlan.txt"
//Relative Path
#define PATH "D:\\0.University\\4.2\\MTRN4110\\Assignment\\Part A\\z5109981_MTRN4110_PhaseA\\PathPlan.txt"

// Namespaces
using namespace webots;
using namespace std;

//Main Function
int main(int argc, char **argv) 
{
    // Robot Initialisation
    Robot *robot = new Robot();

    //Sensor Initialisation
    //a.Distance
    DistanceSensor* dsL = robot->getDistanceSensor("dsL");
    DistanceSensor* dsR = robot->getDistanceSensor("dsR");
    DistanceSensor* dsF = robot->getDistanceSensor("dsF");

    dsL->enable(TIME_STEP);
    dsR->enable(TIME_STEP);
    dsF->enable(TIME_STEP);

    //b.Encoder
    PositionSensor *LEncoder = robot->getPositionSensor("left wheel sensor");
    PositionSensor *REncoder = robot->getPositionSensor("right wheel sensor");

    LEncoder->enable(TIME_STEP);
    REncoder->enable(TIME_STEP);
    


    //Motor Initialisation
    Motor* LMotor = robot->getMotor("left wheel motor");
    Motor* RMotor = robot->getMotor("right wheel motor");

    //Path Initialisation and Definition 
    ifstream PathFile;
    PathFile.open(PATH);
    string P;
    PathFile >>P;

    //Other Variables
    int StepCounter = 00;
    char Heading = P[2];
    int Row = (int)P[0] - 48;
    int Col = (int)P[1] - 48;
    double LDist = 0.0;
    double RDist = 0.0;
    double EVal = 0.0;
    double DS[3];
    char Wall[3];
    int PathLen = P.length();
     
    //Main Execution
    while (robot->step(TIME_STEP) != -1) 
    {
        //Velocity Set
        LMotor->setVelocity(PI);
        RMotor->setVelocity(PI);

        //Initial Print Statements
        cout << "Reading Path Plan from " << PATH << endl;
        cout << P << endl;
        printf("Done - Path plan read!\n");
        printf("Start - Execute Path plan!\n");

        //Step Loop
        for (int i=3;i<PathLen;i++)
        {

            //DistanceSensor Section
            DS[0] = dsL->getValue();
            DS[1] = dsF->getValue();
            DS[2] = dsR->getValue();
     
            for (int j = 0; j < 3; j++)
            {
                if (DS[j] > 500)
                {
                    Wall[j] = 'Y';
                }
                else
                {
                    Wall[j] = 'N';
                }
            }

            //Console Message
            printf("Step: %.2d, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c \n", StepCounter, Row, Col, Heading,Wall[0],Wall[1],Wall[2]);
            StepCounter++;
     
            //Path Check
            if(P[i]=='F')
            {
                //Forward Motion
                LDist+=LINPOS;
                RDist+=LINPOS;

                //Change Relative Position
                switch (Heading)
                {
                     case 'N':
                         Row--;
                         break;
                     case 'E':
                         Col++;
                         break;
                     case 'S':
                         Row++;
                         break;
                     case 'W':
                         Col--;
                         break;
                }
            }
     
            else if(P[i]=='L')
            {
                //Left Turn
                LDist-=ROTPOS;
                RDist+=ROTPOS;

                //Change Direction
                switch (Heading)
                {
                    case 'N':
                        Heading = 'W';
                        break;
                    case 'E':
                        Heading = 'N';
                        break;
                    case 'S':
                        Heading = 'E';
                        break;
                    case 'W':
                        Heading = 'S';
                        break;
                }
            }
     
            else if (P[i] == 'R')
            {
                //Right Turn
                LDist += ROTPOS;
                RDist -= ROTPOS;

                //Change Direction
                switch (Heading)
                {
                    case 'N':
                        Heading = 'E';
                        break;
                    case 'E':
                        Heading = 'S';
                        break;
                    case 'S':
                        Heading = 'W';
                        break;
                    case 'W':
                        Heading = 'N';
                        break;
                }
            }
     
             
            while (abs(EVal - LDist) > 0.005)
            {
                //Step Motion Application
                robot->step(TIME_STEP);
                LMotor->setPosition(LDist);
                RMotor->setPosition(RDist);
                EVal = LEncoder->getValue();
            }
        }
        //Final Step Protocols
        DS[0] = dsL->getValue();
        DS[1] = dsF->getValue();
        DS[2] = dsR->getValue();
     
        for (int j = 0; j < 3; j++)
        {
            if (DS[j] > 500)
            {
                Wall[j] = 'Y';
            }
            else
            {
                Wall[j] = 'N';
            }
        }

        //Final Step Console Print
        printf("Step: %.2d, Row: %d, Column: %d, Heading: %c, Left Wall: %c, Front Wall: %c, Right Wall: %c \n", StepCounter, Row, Col, Heading,Wall[0],Wall[1],Wall[2]);
        printf("Done - Path plan Executed!\n");
        break;
    }

    //Cleanup
    delete robot;
    return 0;    
};
