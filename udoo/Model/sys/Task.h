/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _TASK_H
#define _TASK_H

#define TASK_WAIT_FOR_FIRST_RUN 1000

#include <native/task.h>

class Task {
    RT_TASK task_desc;
    
public: 
    
    /**
     * Creates a new Xenomai task with the given name and priority
     * @param taskName a unique name for this task
     * @param priority 1..99 (1 = lowest priority)
     */
    Task(const char* taskName, int priority);
    
    ~Task();
    
    /**
     * The worker function
     */
    virtual void run(void* args){
        Debug::output("Start is not overridden");
    }
    
    /**
     * Starts the Xenomai task
     */
    bool start(void* args);
    
    /**
     * Starts a periodic Xenomai task with given name, priority and period
     * @param taskName  a unique name for this task
     * @param priority  1..99 (1 = lowest priority)
     * @param periodClockTicks
     */
    bool startPeriodic(size_t periodClockTicks);
    
    
    void waitForNextPeriod();
    
    /**
     * Allows other tasks to work
     */
    void yeild();
    
    /**
     * pauses the task for a certain number of clock ticks
     * @param clockTicks long number of ticks to wait.
     */
    void waitForClockTicks(size_t clockTicks);
};

#endif //_TASK_H