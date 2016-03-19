//
//  GPIO.hpp
//  4th_year_project_r2
//
//  Created by nate bosscher on 2016-03-16.
//  Copyright © 2016 nate bosscher. All rights reserved.
//

#ifndef GPIO_hpp
#define GPIO_hpp

#define GPIO_FOLDER "/sys/class/gpio"
#define GPIO_EXPORT_FILE "/sys/class/gpio/export"

typedef enum{
    MOTOR_CTRL_EN = 143,
    MOTOR_A = 205,
    MOTOR_B = 35,
    MOTOR_SENSE = -1,

    MUX_A0 = 101,
    MUX_A1 = 145,
    MUX_A2 = 105,
    MUX_A3 = 57,
    MUX_A4 = 55,
    HOME_SENSOR = 123,
    AWAY_SENSOR = 125,
    BREAK_BEAM = 127
} gpio_port;

typedef enum{
    PORT_NOT_INITALIZED,
    PORT_NOT_AVAILABLE
} gpio_error;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

class GPIO{

public:
    /**
     * writes a digital value to the specified GPIO port
     *
     * Note: port must have already been intialized by initializePort(port);
     */
    static void digitalWrite(gpio_port port, bool value) throw (gpio_error)
    {
        std::string gpioFilename = getFilenameForGPIOPort(port);
        
        // check if port is already initalized
        if(!fileExists(gpioFilename)){
            throw PORT_NOT_INITALIZED;
        }
        
        // update the direction
        std::ofstream gpioDirection;
        gpioDirection.open(gpioFilename + "/direction");
        gpioDirection << "out";
        gpioDirection.close();
        
        // write the value
        gpioDirection.open(gpioFilename + "/value");
        
        if(value){
            gpioDirection << 1;
        }else{
            gpioDirection << 0;
        }
        
        gpioDirection.close();
    }
    
    /**
     * read a digital value to the specified GPIO port
     * if GPIO value is 0 returns false else returns true
     *
     * Note: port must have already been intialized by initializePort(port);
     */
    static bool digitalRead(gpio_port port) throw (gpio_error)
    {
        std::string gpioFilename = getFilenameForGPIOPort(port);
        
        // check if port is already initalized
        if(!fileExists(gpioFilename)){
            throw PORT_NOT_INITALIZED;
        }
        
        // update the direction
        std::ofstream gpioDirection;
        gpioDirection.open(gpioFilename + "/direction");
        gpioDirection << "in";
        gpioDirection.close();
        
        // get the value
        char value[32];
        
        std::ifstream gpioValue;
        gpioValue.open(gpioFilename + "/value");
        gpioValue.getline(value, sizeof(value) / sizeof(char));
        
        int iValue = std::stoi(value);
        
        if(iValue == 0){
            return false;
        }else{
            return true;
        }
    }
    
    /**
     * read an analog value from the specified GPIO port
     * 
     * Note: port must have already been intialized by initializePort(port);
     */
    static int analogRead(gpio_port port) throw (gpio_error)
    {
        std::string gpioFilename = getFilenameForGPIOPort(port);
        
        // check if port is already initalized
        if(!fileExists(gpioFilename)){
            throw PORT_NOT_INITALIZED;
        }
        
        // update the direction
        std::ofstream gpioDirection;
        gpioDirection.open(gpioFilename + "/direction");
        gpioDirection << "in";
        gpioDirection.close();
        
        // read the value
        char value[32];
        
        std::ifstream gpioValue;
        gpioValue.open(gpioFilename + "/value");
        gpioValue.getline(value, sizeof(value) / sizeof(char));
        
        return std::stoi(value);
    }
    
    /**
     * check if file exists
     */
    static bool fileExists(std::string name){
        std::ifstream file;
        file.open(name);
        
        if(file.good()){
            file.close();
            return true;
        }else{
            file.close();
            return false;
        }
    }
    
    /**
     * sets up gpio port
     * 
     * Note: may throw PORT_NOT_AVAILABLE if port does not exist in system
     */
    static void initializePort(gpio_port port) throw (gpio_error)
    {
        
        std::string gpioFilename = getFilenameForGPIOPort(port);
        
        // check if port is already initalized
        if(fileExists(gpioFilename)){
            return;
        }

        // export GPIO port
        std::string exportFilename(GPIO_EXPORT_FILE);

        std::ofstream file;
        file.open (exportFilename);
        file << port;
        file.close();
        
        // check if the export was unsuccessful
        if(!fileExists(gpioFilename)){
            throw PORT_NOT_AVAILABLE;
        }
        
        return;
        
    }
        
private:
    /**
     * build filename for given GPIO port
     */
    static std::string getFilenameForGPIOPort(gpio_port port){
        // build filename for port
        std::string gpioFilename(GPIO_FOLDER);
        gpioFilename += "/gpio";
        gpioFilename += port;
        
        return gpioFilename;
    }
};

#endif /* GPIO_hpp */
