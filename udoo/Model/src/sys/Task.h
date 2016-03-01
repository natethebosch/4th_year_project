/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _TASK_H
#define _TASK_H

#define TASK_WAIT_FOR_FIRST_RUN 1000

#include <native/task.h>
#include <stdlib.h>
#include "../sys/Debug.h"

class Task;

typedef struct TaskCallbackInfo{
    Task* task;
    void* arg;
} TaskCallbackInfo;


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
    virtual void run(void* cookie){
        Debug::output("Run is not implemented");
    }
    
    static void _run(void* taskInfo){
        TaskCallbackInfo *_taskInfo = (TaskCallbackInfo*) taskInfo;
        _taskInfo->task->run(_taskInfo->arg);
    }
    
    /**
     * Starts the Xenomai task
     */
    bool start(void* args);
    
    /**
     * alias for start(void* args);
     */
    bool start(){
        return start(0);
    }
    
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
    
    /**
     * pauses the task for a certain number of clock ticks
     * @param clockTicks long number of ticks to wait.
     */
    void sleep(size_t clockTicks){
        waitForClockTicks(clockTicks);
    }
};

#endif //_TASK_H
