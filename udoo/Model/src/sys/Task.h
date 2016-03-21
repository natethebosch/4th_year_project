/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _TASK_H
#define _TASK_H

#define TASK_WAIT_FOR_FIRST_RUN 1000

// xenomai
#include <task.h>

#include <stdlib.h>
#include "../sys/Debug.h"

class Task;

typedef struct TaskCallbackInfo{
    Task* task;
    void* arg;
} TaskCallbackInfo;

// can be ORed together
typedef enum {
    NONE = 0x0,
    TERMINATE = 0x1
} TaskSignal;


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
    
    /**
     * sleeps task for given milliseconds
     */
    void sleep_millis(size_t ms){
        for(size_t i = 0; i < ms; i++){
            sleep(/*rt_timer_ns2ticks*/(1000000));
        }
    }
    
    /**
     * blocks until task is complete
     */
    void join(){
        rt_task_join(&task_desc);
    }
    
    /**
     * checks for signals from other tasks
     */
    TaskSignal receiveSignal(){
        // allocate message block
        RT_TASK_MCB msgBlock;
        
        TaskSignal* data = (TaskSignal*)malloc(sizeof(TaskSignal));
        
        msgBlock.data = (caddr_t)data;
        msgBlock.size = sizeof(TaskSignal);
        
        // get message
        int status = rt_task_receive(&msgBlock, TM_NONBLOCK);
        
        // debug
        if(status <= 0){
            switch(status){
                case -EPERM:
                    Debug::output("EPERM - task::receiveSignal");
                    break;
                case -EINTR:
                    Debug::output("EINTR - task::receiveSignal");
                    break;
                case -ENOBUFS:
                    Debug::output("ENOBUFS - task::receiveSignal");
                    break;
                case -EWOULDBLOCK:
                    // don't output because this it normal execution
                    // Debug::output("EWOULDBLOCK - task::receiveSignal");
                    break;
                case -ETIMEDOUT:
                    Debug::output("ETIMEDOUT - task::receiveSignal");
                    break;
                default:
                    Debug::output("UNKNOWN - task::receiveSignal");
                    break;
            }
            
            free(data);
            
            return NONE;
        }
        
        Debug::output("Got signal");
        
        // return signal
        TaskSignal ts = *data;
        free(data);
        
        return ts;
    }
    
    /**
     * sends signal to this task
     */
    void signal(TaskSignal item){
        // signal task with terminate
        RT_TASK_MCB msgBlock;
        
        TaskSignal* data = (TaskSignal*)malloc(sizeof(TaskSignal));
        *data = item;

        msgBlock.data = (caddr_t)data;
        msgBlock.size = sizeof(TaskSignal);
        
        // send signal
        ssize_t status = rt_task_send(&task_desc, &msgBlock, NULL, 0);
        
        free(data);
        
        // debug errors
        if(status <= 0){
            switch(status){
                case -EINVAL:
                    Debug::output("EINVAL (queue is not a valid descriptor) - task::signal");
                    break;
                case -EPERM:
                    Debug::output("EPERM - task::signal");
                    break;
                case -ENOBUFS:
                    Debug::output("ENOBUFS - task::signal");
                    break;
                case -EWOULDBLOCK:
                    Debug::output("EWOULDBLOCK - task::signal");
                    break;
                case -EIDRM:
                    Debug::output("EIDRM (task doesn't exist) - task::signal");
                    break;
                case -EINTR:
                    Debug::output("EINTR - task::signal");
                    break;
                default:
                    Debug::output("UNKNOWN - task::signal");
                    break;
            }
        }
    }
    
    
    
    /**
     * sends the terminate signal to a task and 
     * waits until task has been removed
     */
    void kill(){
        
        this->signal(TERMINATE);
        this->join();
        
        // delete the task
        switch(rt_task_delete(&task_desc)){
            case 0: return;
            case -EINVAL:
                Debug::output("EINVAL");
                break;
            case -EPERM:
                Debug::output("EPERM");
                break;
            case -EINTR:
                Debug::output("EINTR");
                break;
            case -EIDRM:
                Debug::output("EIDRM");
                break;
            default:
                Debug::output("UNKNOWN");
                break;
        }
    }
    
    /**
     * checks for terminate signal
     */
    bool hasTerminateSignal(){
        if(this->receiveSignal() == TERMINATE){
            return true;
        }else{
            return false;
        }
    }
};

#endif //_TASK_H
