#include "Task.h"

// xenomai
#include <timer.h>

/**
 * Starts a new Xenomai task with the given name and priority
 * @param taskName a unique name for this task
 * @param priority 1..99 (1 = lowest priority)
*/
Task::Task(const char* taskName, int priority){
   // create task
   int status = rt_task_create(&this->task_desc, taskName, 0, priority, 0);

   // check for errors
   if(status == 0){
       // success
       return;
   }else{
       // handle error
       switch(status){
           case ENOMEM:
               Debug::output("Not enough memory to create task");
               break;
           case EEXIST:
               Debug::output("A task with that name already exists");
               break;
           case EPERM:
               Debug::output("Called from an Async context");
               break;
           default:
               Debug::output("Unknown error");
               break;
       }

       return;
   }
}

Task::~Task(){
    // cleanup task
    int status = rt_task_delete(&this->task_desc);

    // check for errors
    if(status != 0){
        switch(status){
            case EINVAL: 
                Debug::output("task is not a task descriptor.");
                break;
            case EPERM: 
                Debug::output("task is NULL but not called from a task context, or this service was called from an asynchronous context");
                break;
            case EINTR:
                Debug::output("rt_task_unblock() has been invoked for the caller while it was waiting for task to exit a safe section. In such a case, the deletion process has been aborted and task remains unaffected.");
                break;
            case EIDRM:
                Debug::output("task is a deleted task descriptor.");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
    }
}

bool Task::start(void* args){

    // must wrap the callback information into a struct so we can pass the "this" reference
    // and the arguements
    TaskCallbackInfo *tfi = (TaskCallbackInfo*)malloc(sizeof(TaskCallbackInfo));
    tfi->task = this;
    tfi->arg = args;

    int status = rt_task_start(&this->task_desc, Task::_run, tfi);
    
    // check for errors
    if(status != 0){
        switch(status){
            case -EINVAL:
                Debug::output("Invalid task descriptor");
                break;
            case -EIDRM:
                Debug::output("Task descriptor has already been deleted");
                break;
            case -EBUSY:
                Debug::output("Task has already started");
                break;
            case -EPERM:
                Debug::output("Task is being called from an async context");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
        
        return false;
    }
    
    return true;
    
}
    
/**
 * Allows other tasks to work
*/
void Task::yeild(){
   if(rt_task_yield()){
       return;
   }else{
       Debug::output("service was called from a context which cannot sleep");
   }
}

/**
 * Starts a periodic Xenomai task with given name, priority and period
 * @param taskName  a unique name for this task
 * @param priority  1..99 (1 = lowest priority)
 * @param periodClockTicks
*/
bool Task::startPeriodic(size_t periodClockTicks){
    // setup periodic
    int status = rt_task_set_periodic(&task_desc, rt_timer_read() + TASK_WAIT_FOR_FIRST_RUN, periodClockTicks);
    
    // check for errors
    if(status != 0){
        switch(status){
            case EINVAL: 
                Debug::output("task is not a task descriptor, or period is different from TM_INFINITE but shorter than the scheduling latency value for the target system, as available from /proc/xenomai/latency.");
                break;
            case EIDRM:
                Debug::output("task is a deleted task descriptor");
                break;
            case ETIMEDOUT:
                Debug::output("date is different from TM_INFINITE and represents a date in the past");
                break;
            case EWOULDBLOCK:
                Debug::output("the system timer is not active");
                break;
            case EPERM:
                Debug::output("task is NULL but not called from a task context");
                break;
            default:
                Debug::output("Unknown error");
                break;
        }
        return false;
    }
    
    return true;
}

void Task::waitForNextPeriod(){
    int status;

    while(true){
        status = rt_task_wait_period(NULL);
        switch(status){
            case ETIMEDOUT:
                Debug::output("Periodic task timing miss");
                return;
            case EPERM:
                Debug::output("this service was called from a context which cannot sleep");
                return;
            case EINTR:
                // rt_task_unblock has been called
                return;
            case EWOULDBLOCK:
                Debug::output("this task is not registered as periodic");
                return;
            case 0:
                // successful wait executed
                return;
            default:
                Debug::output("Unknown error");
                return;
        }
    }
}

/**
 * pauses the task for a certain number of clock ticks
 * @param clockTicks long number of ticks to wait.
*/
void Task::waitForClockTicks(size_t clockTicks){
   // compute scheduled time from current time
   RTIME scheduledTimer = clockTicks + rt_timer_read();

   // call sleep
   int status = rt_task_sleep_until(scheduledTimer);

   // check for errors
   if(status == 0){
       return;
   }else{
       switch(status){
           case EINTR: 
               Debug::output("rt_task_unblock() has been called for the sleeping task before the sleep time has elapsed.");
               break;
           case ETIMEDOUT:
               Debug::output("Date is in the past");
               break;
           case EWOULDBLOCK:
               Debug::output("the system timer is inactive");
               break;
           case EPERM:
               Debug::output("this service was called from a context which cannot sleep");
               break;
           default:
               Debug::output("Unknown error");
               break;
       }
       return;
   }
}
