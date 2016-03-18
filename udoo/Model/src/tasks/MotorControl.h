/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */

//pins
#define MOTORHOME		MOTOR_PWM_0
#define	MOTORAWAY		MOTOR_PWM_1
#define MOTORCONTROL	MOTOR_CTRL_EN
#define HOMESENSOR		MOTOR_SENSE
#define AWAYSENSOR		MOTOR_SENSE


#ifndef _MOTORCONTROL_H
#define _MOTORCONTROL_H

#include "../sys/Task.h"
#include "../sys/GPIO.h"




class MotorControl: public Task {
public: 
    int controlPort;
    int holeIndex;
    GPIO io;
    
    void run(void* cookie);
    
    void homeScanner();
    
    void awayScanner();
    
    void scanHome();
    
    void scanAway();
    
    void stopScanner();
};

#endif //_MOTORCONTROL_H
