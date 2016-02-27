/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _MOTORCONTROL_H
#define _MOTORCONTROL_H

#include "../sys/Task.h"


class MotorControl: public Task {
public: 
    int controlPort;
    int holeIndex;
    
    void homeScanner();
    
    void awayScanner();
    
    void scanHome();
    
    void scanAway();
    
    void stopScanner();
};

#endif //_MOTORCONTROL_H
