/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "SensorCommunicator.h"
#include "../constants.h"

/**
 * SensorCommunicator implementation
 */

SensorCommunicator::SensorCommunicator(BlockingQueueSender<SensorDataPoint> _pipe) {
    super("SensorComm", 80);
    
    pipe = _pipe;
    
    /*
     * Serial interface setup
     * taken from https://en.wikibooks.org/wiki/Serial_Programming/Serial_Linux
     */
    tcgetattr(STDOUT_FILENO,&old_stdio);
    
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking
    
    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;
    
    tty_fd=open(SERIAL_PATH, O_RDWR | O_NONBLOCK);      
    cfsetospeed(&tio,BAUD_RATE);
    cfsetispeed(&tio,BAUD_RATE);
    
    tcsetattr(tty_fd,TCSANOW,&tio);
}

SensorCommunicator::~SensorCommunicator(){
    // close input
    close(tty_fd);
    
    // restore settings
    tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);
}

void SensorCommunicator::run(void* args){
    
    char c;
    char *section;
    SensorDataPoint dp;
    
    // read until there's no more characters
    while(read(tty_fd,&c,1)>0){
        // if new data is available on the serial port, print it out
        buffer[position++] = c;
        
        if(c == ';'){
            // add null termination
            buffer[position+1] = '\0';
            
            // copy buffer to section
            section = (char*)malloc(sizeof(char) * (position + 1));
            strcpy(section, (const char*)&buffer);
            
            // send off to be decoded
            dp = decodeString(section);
            pipe.put(dp);
            
            // reset position
            position = 0;
        }
    }
    
    // done.. wait for next period
}

/**
 * Receives string and decodes it
 * @param str should be in format x,y,value where x,y and value are max 16 characters
 * @return SensorDataPoint of string data
 */
SensorDataPoint SensorCommunicator::decodeString(char* str){
    // buffers
    char x[17];
    char y[17];
    char value[17];
    
    SensorDataPoint dp;
    
    // indexes
    int pos_value;
    int pos_x;
    int pos_y;
    
    // initialize
    pos_value = 0;
    pos_x = 0;
    pos_y = 0;
    
    // decoder state
    int section = 0;
    
    // loop over the string
    for(int i = 0; i < strlen(str); i++){
        // check for delimiter char
        if(str[i] == ','){
            section++;
        }else{
            switch(section){
                case 0:
                    x[pos_x++] = str[i];
                    x[pos_x] = '\0'; // ensure valid string
                    break;
                case 1:
                    y[pos_y++] = str[i];
                    y[pos_y] = '\0'; // ensure valid string
                    break;
                case 2:
                    value[pos_value++] = str[i];
                    value[pos_value] = '\0'; // ensure valid string
                    break;
                default:
                    break;
            }
        }
    }
    
    sscanf(value, "%f", &dp.value);
    sscanf(x, "%f", &dp.x);
    sscanf(y, "%f", &dp.y);
    
    return dp;
}