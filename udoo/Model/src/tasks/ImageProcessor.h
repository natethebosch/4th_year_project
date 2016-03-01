/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#ifndef _IMAGEPROCESSOR_H
#define _IMAGEPROCESSOR_H

#include "../sys/Task.h"
#include "../helpers/BlockingQueue.h"
#include "../type/SensorDataPoint.h"


#define HEIGHT 		500
#define WIDTH 		480
#define MAJORXS 	24
#define MAXREADING 	(255*MAJORXS)
#define xSpacing	(WIDTH/MAJORXS)

#include <vector>
#include <iostream>
#include <string>

#include "dlib/pixel.h"
#include "dlib/array2d.h"
#include "dlib/image_io.h"
#include "dlib/image_transforms.h"


using namespace std;
using namespace dlib;

class ImageProcessor: public Task {
private:
    
    //hold the x value of the next data point to be added
    int currentX;
    
    //array holding the y values of the previous row of data points
    int *lastY;
    
    //array holding the y values of the current row of data points
    int *currentY;

    //true if the current row of data points is the first in this image otherwise false
    bool first;
    
    //holds the y value to which the data array has been completely compiled
    int currentXCompile;
    
    //holds the image
    array2d<hsi_pixel> img;
    
    //holds the added data points as well as the interpolated values between
    float sensorData[HEIGHT][WIDTH];
    
    void yCompile();
    void xCompileTo(int botY);
    
    BlockingQueueReceiver<SensorDataPoint> *input;
    
public:
    
    void addData (float value, int y);
    void displayData ();
    array2d<hsi_pixel>& compileImage();
    array2d<hsi_pixel>& getImage();
    
    //constructor/reseter
    void empty();
    
    ImageProcessor(BlockingQueueReceiver<SensorDataPoint>* _input): Task("ImageProcessor", 20){
    	empty();
        input = _input;
    };
    
    void run(){
        // image processor main logic goes here
          SensorDataPoint dp;
          float lasty=0.0;
          
          ImageProcessor imgpros=new ImageProcessor (0);

          
          // infinite loop
          for(;;){ 
               // fetch from the buffer
         
               try{
                  dp = input.take();
                
                   // y value multiplied by 5 since height is 500 and the scanner is 100cm long
                  imgpros[count].addData(dp.value, int(dp.y*5));

                  //finishs this image and moves on to the next one
				  if (lasty>(dp.y+1)){
                  	//Sends the image and the numerical data set to the web server
                  	
                  	
			   		//creates a new ImageProcessor object to hold the next scan
			   		imgpros=new ImageProcessor (0);
				  }
               }catch(BlockingQueueStatus s){
                   if(s == BLOCKING_QUEUE_TIMEOUT){
                       continue;
                   }else{
                       Debug::output("Irrecoverable error. Exiting...");
                       exit(-1);
                   }
               }
          }
    }
};

#endif //_IMAGEPROCESSOR_H
