/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _SENSORCOMMUNICATOR_H
#define _SENSORCOMMUNICATOR_H

#include "../sys/Task.h"
#include "../type/SensorDataPoint.h"
#include "../helpers/BlockingQueue.h"

/**
 * Receives sensor data from the serial port
 * Data is expected in the format 
 * Px,y,value\n
 * Px,y,value\n
 * ..
 *
 * each of x, y and value may be 16 characters long
 * 
 */

enum SensorCommunicatorDecoderState{
    WAITING,NEW_LINE,READ
};


class SensorCommunicator: public Task {
    
    BlockingQueueSender<SensorDataPoint> *queue;
    
    // Serial Interface stuff
    int tty_fd;
    struct termios tio;
    struct termios old_stdio;
    struct termios stdio;
    
    // 16 characters * 3 items + 1 null termination character + 10 safety
    char buffer[16*3+1+10];
    int position;
    
public:
    
    SensorCommunicator(BlockingQueueSender<SensorDataPoint> *_queue);
    ~SensorCommunicator();
    
    virtual void run(void* args);
    /**
     * Receives string and decodes it
     * @param str should be in format x,y,value where x,y and value are max 16 characters
     * @return SensorDataPoint of string data
     */
    SensorDataPoint decodeString(char* str);
};

#endif //_SENSORCOMMUNICATOR_H
