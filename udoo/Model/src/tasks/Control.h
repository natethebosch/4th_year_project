/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */



#ifndef _MOTORCONTROL_H
#define _MOTORCONTROL_H

#include "../sys/Task.h"
#include "../sys/GPIO.h"
#include "../helpers/BlockingQueue.h"
#include "../sys/AudioRecorder.h"

typedef enum {
    TOWARDS_HOME,
    TOWARDS_AWAY,
    STOPPED
} Direction;

typedef enum{
    UNKNOWN,
    SCANNING,
    HOME,
    AWAY
} Position;

#define DEBOUNCE_PARAM 100
#define SCAN_EACH_N_HOLES 10
#define HOLE_TIMER_NS 10 * 10e6 // 100 ms
#define N_SENSORS 20

class Control : public Task {
    
    void motorHome();
    void motorAway();
    
public:
    
    // hold index
    int holeIndex = -1;
    int debounceCountDown = 0;
    bool holeIndexValue = false;
    RTIME lastCheck;
    
    // line scan
    int lineScanPosition = 0;
    
    GPIO io;
    Direction scanDirection;
    Position scanPosition;
    AudioRecorder ar;
    
    BlockingQueue<SensorDataPoint> *_queue;
    
    Control(BlockingQueue<SensorDataPoint> *queue) : Task("Controller", 80){
        _queue = queue;
        scanDirection = STOPPED;
        scanPosition = UNKNOWN;
        
        io.initializePort(MOTOR_A);
        io.initializePort(MOTOR_B);
        io.initializePort(MOTOR_CTRL_EN);
        io.initializePort(HOME_SENSOR);
        io.initializePort(AWAY_SENSOR);
        io.initializePort(BREAK_BEAM);
        
        lastCheck = rt_timer_read();
    }
    
    void run(void* cookie){
        // go home
        while(scanHome()){
            yeild();
        }
        
        holeIndex = 0;
        scanPosition = HOME;
        
        // do scan
        scanDirection = TOWARDS_AWAY;
        scanPosition = SCANNING;
        while(scanAway()){
            updateHoleIndex();
            
            if(isAtScanPosition()){
                while(performLineScan()){
                    updateHoleIndex();
                    yeild();
                }
                
                resetLineScanner();
            }else{
                yeild();
            }
        }
        
        scanPosition = AWAY;
        
        // go back to starting point
        scanDirection = TOWARDS_HOME;
        scanPosition = SCANNING;
        while(scanHome()){
            updateHoleIndex();
            yeild();
        }
    }
    
    bool homeScanner();
    
    bool awayScanner();

    /**
     * returns true until it reaches the end and
     * stops
     */
    bool scanHome();
    
    /**
     * returns true until it reaches the end and 
     * stops
     */
    bool scanAway();
    
    /**
     * returns false if the holeIndex is changed
     */
    bool updateHoleIndex();
    
    void stopScanner();
    
    /**
     * has the holdIndex reached a start of a scan
     */
    bool isAtScanPosition();
    
    /**
     * returns true until scan is complete
     */
    bool performLineScan();
    
    /**
     * resets the line scan position
     */
    void resetLineScanner();
    
};

#endif //_MOTORCONTROL_H
