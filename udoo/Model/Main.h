/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _MAIN_H
#define _MAIN_H

class Main {
public: 
    MotionGuard guard;
    MotorControl motorControl;
    WebServer webServer;
    SensorCommunicator sensorComm;
    ImageProcessor imageProcessor;
    
    void initial();
};

#endif //_MAIN_H