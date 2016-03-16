//
//  GPIO.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-16.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef GPIO_hpp
#define GPIO_hpp

typedef enum{
    MOTOR_CTRL_EN = 10,
    MOTOR_PWM_0 = 11,
    MOTOR_PWM_1 = 12,
    MOTOR_SENSE = 13,
    ARDUINO_SERIAL = 14,
} gpio_port;

typedef enum{
    PORT_NOT_INITALIZED,
    PORT_NOT_AVAILABLE
} gpio_error;

#include <stdio.h>

class GPIO{
public:
    /**
     * writes a digital value to the specified GPIO port
     *
     * Note: port must have already been intialized by initializePort(port);
     */
    static void digitalWrite(gpio_port port, bool value) throw (gpio_error)
    {
        // todo:
    }
    
    /**
     * read a digital value to the specified GPIO port
     *
     * Note: port must have already been intialized by initializePort(port);
     */
    static bool digitalRead(gpio_port port) throw (gpio_error)
    {
        // todo:
        return false;
    }
    
    /**
     * read an analog value from the specified GPIO port
     * 
     * Note: port must have already been intialized by initializePort(port);
     */
    static int analogRead(gpio_port port) throw (gpio_error)
    {
        // todo:
        return -1;
    }
    
    /**
     * sets up gpio port
     * 
     * Note: may throw PORT_NOT_AVAILABLE if port does not exist in system
     */
    static bool initializePort(gpio_port port) throw (gpio_error)
    {
        // todo:
        return false;
    }
};

#endif /* GPIO_hpp */
