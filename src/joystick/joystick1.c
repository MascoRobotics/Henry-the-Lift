#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Hubs,  S4, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     SPEAK,          sensorSoundDB)
#pragma config(Sensor, S3,     comp,           sensorI2CHiTechnicCompass)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorC,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorD,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorB,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorA,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_1,     motorJ,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
/*
=========
*/


/*
=== INCLUDES ===
*/
#include "JoystickDriver.c"
/*
=========
*/


/*
=== VARS  (1) ===
*/
int stage1F = 2473;
int stage2F = 12720;
int stage2E = -3303;
int limit1 = 12689;
int limit2 = -6475;
float speed = 1;
bool ignorelimit;
/*
=========
*/


/*
=== INIT ROBOT ===
*/
void initializeRobot()
{
	//waitForStart();
	return;
}
/*
=========
*/


/*
=== AUTO MOVE MOTORE ===
*/
void setLevelE(int height)
{
	while (abs(nMotorEncoder[motorE] - height) > 10) {
		if (nMotorEncoder[motorE] > height)
			motor[motorE] = -40;
		else
			motor[motorE] = 40;
	}
	motor[motorE] = 0;
}
/*
=========
*/


/*
=== AUTO MOVE MOTORF ===
*/
void setLevelF(int height)
{
	while (abs(nMotorEncoder[motorF] - height) > 10) {
		if (nMotorEncoder[motorF] > height)
			motor[motorF] = -40;
		else
			motor[motorF] = 40;
	}
	motor[motorF] = 0;
}
/*
=========
*/



/*
=== VAR (2) ===
*/
float angle, magnitude, x, y;
bool up = false;
int threshold = 40;
bool pressed;
bool pressed1;
bool pressed2;
bool pressed3;
/*
=========
*/


/*
=== MAIN ===
*/
task main()
{
	/*
	=== SETUP ==
	*/
	//initializeRobot();
	servoChangeRate[servo1] = 10000;
	nMotorEncoder[motorF] = 0;
	PlaySoundFile("cmc.rso");
	/*
	=========
	*/

	while(true) {
		/*
		=== DRIVING CODE ===
		*/
		if (speed == 0)
			speed = 1;
		magnitude = sqrt((joystick.joy1_x1)*(joystick.joy1_x1)+(joystick.joy1_y1)*(joystick.joy1_y1));
		if(joystick.joy1_x1 > 0)
			angle = asin((joystick.joy1_y1)/magnitude)+(3.1415292/4);
		else
			angle = ((3.1415292/2)-asin((joystick.joy1_y1)/magnitude))+(3.1415292/2)+(3.1415292/4);
		x = cos(angle)*magnitude;
		y = sin(angle)*magnitude;
		writeDebugStream("int x is: %d", magnitude);
		writeDebugStream("int angle is: %d", angle);
		getJoystickSettings(joystick);
		if(abs(joystick.joy1_x2) > threshold){
			motor[motorA] = joystick.joy1_x2 / speed;
			motor[motorB] = joystick.joy1_x2 / speed;
			motor[motorD] = joystick.joy1_x2 / speed;
			motor[motorC] = joystick.joy1_x2 / speed;
		}
		else if(abs(joystick.joy1_x1) > threshold || abs(joystick.joy1_y1) > threshold){
			motor[motorA] = y / speed;
			motor[motorD] = x / speed;
			motor[motorB] = -y / speed;
			motor[motorC] = -x / speed;
		}
		else{
			motor[motorA] = 0;
			motor[motorD] = 0;
			motor[motorB] = 0;
			motor[motorC] = 0;
		}
		/*
		=========
		*/


		/*
		=== SOUNDS ===
		*/
		if (joy1Btn(4) == 1 || joy1Btn(3) == 1) {
			if (joy1Btn(4) == 1)
				PlaySound(soundFastUpwardTones);
			else
				PlaySound(soundBeepBeep);
		}
		else {
			ClearSounds();
		}
		/*
		=========
		*/


		/*
		=== RAMP MOVEMENT ===
		*/
		if (joy1Btn(5) || joy1Btn(7)) {
			if (joy1Btn(5)) {
				motor[motorK] = 100;
			}
			else {
				motor[motorK] = -100;
			}
		}
		else
			motor[motorK] = 0;
	  if (joy1Btn(6) || joy1Btn(8)) {
	  	if (joy1Btn(6)) {
	  		motor[motorJ] = 100;
	  	}
	  	else {
	  		motor[motorJ] = -100;
	  	}
	  }
	  else
	  	motor[motorJ] = 0;
		/*
		=========
		*/


		/*
		=== ARM MOVEMENT ===
		*/
		if (joy2Btn(2) == 1) {
			pressed = true;
		}
		else {
			if (pressed) {
				up = !up;
			}
			pressed = false;
		}
		if (up)
			servo[servo1] = 145;
		else
			servo[servo1] = 255;
		/*
		=========
		*/


		/*
		=== SPEED ADJUSTER ===
		*/
		if (joystick.joy1_TopHat == 0) {
			pressed2 = true;
		}
		else {
			if (pressed2)
				speed = speed / 2;
			pressed2 = false;
		}
		if (joystick.joy1_TopHat == 4) {
			pressed3 = true;
		}
		else {
			if (pressed3)
				speed = speed * 2;
			pressed3 = false;
		}
		/*
		=========
		*/


		/*
		=== DEBUG TOGGLE ===
		*/
		if (joy2Btn(3) == 1) {
			pressed1 = true;
		}
		else {
			if (pressed1) {
				ignorelimit = !ignorelimit;
				nxtDisplayString(0, "Ignore limit: %d", ignorelimit);
			}
			pressed1 = false;
		}
		/*
		=========
		*/


		/*
		=== RESET TOGGLE ===
		*/
		if (joy2Btn(9)) {
			nxtDisplayString(0, "Motor F reset");
			nMotorEncoder[motorF] = 0;
		}
		if (joy2Btn(10)) {
			nxtDisplayString(0, "Motor E reset");
			nMotorEncoder[motorE] = 0;
		}
		/*
		=========
		*/


		/*
		=== MACRO TOGGLE ===
		*/
		if (joy2Btn(1) || joy2Btn(4)) {
			if (joy2Btn(1)) {
				setLevelE(0);
				setLevelF(stage1F);
			}
			else if (joy2Btn(4)) {
				setLevelE(stage2E);
				setLevelF(stage2F);
			}
		}
		/*
		=========
		*/


		/*
		=== DISPLAY DEBUG DATA ===
		*/
		nxtDisplayString(0, "                       ");
		nxtDisplayString(0, "%d : %d : %d", nMotorEncoder[motorF], nMotorEncoder[motorE], SensorValue[S3]);
		/*
		=========
		*/


		/*
		=== LIFT SYSTEM (DEBUG/NORMAL) ===
		*/
		if (!ignorelimit) {
			if(joy2Btn(6) == 1 || joy2Btn(8) == 1) {
				if (joy2Btn(6) == 1) {
					if (limit1 - nMotorEncoder[motorF] > 10) {
						motor[motorF]= 100;
					}
					else {
						motor[motorF] = 0;
					}
					if ((limit2 - nMotorEncoder[motorE] < -10) && limit1 - nMotorEncoder[motorF] <= 10) {
						motor[motorE] = -100;
					}
					else if (limit2 - nMotorEncoder[motorE] >= -10 && limit1 - nMotorEncoder[motorF] <= 10) {
						motor[motorE] = 0;
					}
				}
				else {
					if (nMotorEncoder[motorE] < -10)
						motor[motorE] = 100;
					else
						motor[motorE] = 0;
					if (nMotorEncoder[motorF] > 10)
						motor[motorF] = -100;
					else if (nMotorEncoder[motorF] <= 10 && nMotorEncoder[motorE] >= -10)
						motor[motorF] = 0;
				}
			}
			else {
				motor[motorF] = 0;
				motor[motorE] = 0;
			}
		}
		else {
			nxtDisplayString(0, "                  ");
			nxtDisplayString(0, "%d : %d", nMotorEncoder[motorF], nMotorEncoder[motorE]);
			if (joy2Btn(8) || joy2Btn(6)) {
				if (joy2Btn(6))
					motor[motorF] = 100;
				else
					motor[motorF] = -100;
			}
			else
				motor[motorF] = 0;
			if (joy2Btn(5) || joy2Btn(7)) {
				if (joy2Btn(5))
					motor[motorE] = -100;
				else
					motor[motorE] = 100;
			}
			else
				motor[motorE] = 0;
		}
		/*
		=========
		*/
	}
}
/*
=========
*/
