#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     DEPTH,          sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     motorA,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorB,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorC,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorF,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*--------------------------------------------------------------------------------------------------------*\
|*                                                                                                        *|
|*                                    - Tetrix Simple Tank Drive -                                        *|
|*                                          ROBOTC on Tetrix                                              *|
|*                                                                                                        *|
|*  This program allows you to drive a robot via remote control using the ROBOTC Debugger.                *|
|*  This particular method uses "Tank Drive" where each side is controlled individually like a tank.      *|
|*                                                                                                        *|
|*                                        ROBOT CONFIGURATION                                             *|
|*    NOTES:                                                                                              *|
|*                                                                                                        *|
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port D                  motorD              12V                 Right motor                         *|
|*    Port E                  motorE              12V                 Left motor                          *|
\*---------------------------------------------------------------------------------------------------4246-*/

#include "JoystickDriver.c"
int movex, movey;
int threshold = 40;
int movement[10];
bool movefoward;
int guess = 0;
int guesscount = 0;
task main()
{
	waitForStart();
	movement[0] = -1;
	movement[1] = -100;
	movement[2] = -75;
	movement[3] = -50;
	movement[4] = -25;
	movement[5] = 0;
	movement[6] = 25;
	movement[7] = 50;
	movement[8] = 75;
	movement[9] = 100;
  while(true)
  {
  	servo[servo1]=0;
  	servo[servo2]=255;
  	//Discover movex
    movex = SensorValue[IR];
    movey = SensorValue[DEPTH];
    nxtDisplayString(0, "%d", movey);
    if (movey <= 100 && movex == 5 && guess == 2) {//change value to 70
    		motor[motorA] = 0;
 				motor[motorB] = 0;
 				motor[motorC] = 0;
 				motor[motorD] = 0;
 				continue;
    	}
    	if (movex == 0 && movefoward) { //Assume where infront of the pole
    		break;
    	}
    	if (movey >= 35 && movex == 0 && guess == 2) {
    		guess = 0;
    		movefoward = false;
    	}
    if (guess == 1 && movex != 0) {
    	guess = 2;
    	motor[motorA] = 0;
 				motor[motorB] = 0;
 				motor[motorC] = 0;
 				motor[motorD] = 0;
    }
    if (!movefoward) { //look at it
    	if (movex == 0) {
    			if (guesscount > 1000 && guess == 0) {
    				guess = 1;
    				motor[motorA] = -50;
   					motor[motorD] = -50;
   					motor[motorC] = 60;
   					motor[motorB] = -50;
   			}
   			guesscount++;
   			continue;
    	}
    	//nxtDisplayString(0, "%d", movey);

   		if (movex != 5) {
   			motor[motorA] = -movement[movex] / 2;
   			motor[motorD] = -movement[movex] / 2;
   			motor[motorC] = -movement[movex] / 2;
   			motor[motorB] = -movement[movex] / 2;
 			}
 			else {
 				motor[motorA] = 0;
 				motor[motorB] = 0;
 				motor[motorC] = 0;
 				motor[motorD] = 0;
 				movefoward = true;
 			}
 		}
 		else { //Go towards it
 			//nxtDisplayString(0, "GO TOWARDS IT!");
 			if (movex == 0) {
 				motor[motorA] = 0;
 				motor[motorB] = 0;
 				motor[motorC] = 0;
 				motor[motorD] = 0;
 				movefoward = false;
 			}
 			else if (movex == 5) {
 				motor[motorA] = (movey - 1) / 2;
 				motor[motorB] = (-movey - 1) / 2;
 				motor[motorC] = 50;
 				motor[motorD] = 50;
 			}
 			else {
 				motor[motorC] = movement[movex] / 2;
 				motor[motorD] = movement[movex] / 2;
 			}
 		}
  }
  while (true) {
  	servo[servo1]=0;
  	servo[servo2]=255;
    movey = SensorValue[DEPTH];
    if (movey <= 30) {
    	motor[motorA] = 0;
    	motor[motorB] = 0;
    	motor[motorC] = 0;
    	motor[motorD] = 0;
    	motor[motorE] = 100;
    	wait1Msec(2300);
    	motor[motorE] = 0;
    }
  }

}