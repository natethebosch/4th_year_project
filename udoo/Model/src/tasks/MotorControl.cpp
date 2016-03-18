/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "MotorControl.h"

/**
 * MotorControl implementation
 */


using namespace std;

void MotorControl::run(void* cookie){
	//set-up
	
	//looping performing continuous scans
	while (true){
		//resets scanner in home position to scan
		homeScanner();
		//says scan is starting
		
		//scans
		scanAway();
		//sys scan is ended
		
	}
}


void MotorControl::homeScanner() {
	io.digitalWrite(MOTORCONTROL, true);
	io.digitalWrite (MOTORHOME,true);
	io.digitalWrite(MOTORCONTROL,false);
	
	while (!io.digitalRead(HOMESENSOR)){
		
	}
	stopScanner();
}

void MotorControl::awayScanner() {
	io.digitalWrite(MOTORCONTROL, true);
	io.digitalWrite (MOTORAWAY,true);
	io.digitalWrite(MOTORCONTROL,false);
	
	while (!io.digitalRead(AWAYSENSOR)){
		
	}
	stopScanner();
}

void MotorControl::scanHome() {
	io.digitalWrite(MOTORCONTROL, true);
	io.digitalWrite (MOTORHOME,true);
	io.digitalWrite(MOTORCONTROL,false);
	
	while (!io.digitalRead(HOMESENSOR)){
		sched_yield();
	}
	stopScanner();
}

void MotorControl::scanAway() {
	io.digitalWrite(MOTORCONTROL, true);
	io.digitalWrite (MOTORAWAY,true);
	io.digitalWrite(MOTORCONTROL,false);
	
	while (!io.digitalRead(AWAYSENSOR)){
		sched_yield();
	}
	stopScanner();
}

void MotorControl::stopScanner() {
	io.digitalWrite(MOTORCONTROL,true);
	io.digitalWrite(MOTORHOME,false);
	io.digitalWrite(MOTORAWAY,false);
	io.digitalWrite(MOTORCONTROL,false);
}
