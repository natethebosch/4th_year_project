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
#define xSpacing	(WIDTH/MAJORXS)
#define MAXVAL		1023
#define CHROMA		1

#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>

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
    array2d<rgb_pixel> img;
    
    //holds the added data points as well as the interpolated values between
    int sensorData[HEIGHT][WIDTH];
    
    //holds date/timestamp image name
    time_t timer;
    struct tm *timeinfo;
    string fileName;
    
    void yCompile();
    void xCompileTo(int botY);
    
    BlockingQueue<SensorDataPoint> *input;
    
public:
    
    void addData (float value, int y);
    void displayData ();
    array2d<rgb_pixel>& compileImage();
    array2d<rgb_pixel>& getImage();
    int* getData();
    void run();
    
    //constructor/reseter
    void empty();
    
    ImageProcessor(BlockingQueue<SensorDataPoint> *_input): Task("ImageProcessor", 20){
    	empty();
        input = _input;
    };
    
    virtual void run(void* cookie){
        // image processor main logic goes here
          SensorDataPoint dp;
          float lasty=0.0;
          ImageProcessor *imgpros = new ImageProcessor(0);

          
          // infinite loop
          for(;;){ 
               // fetch from the buffer
         
               try{
                  dp = input->take();
                
                   // y value divided by 2 since height is 500 and the scanner is 1000mm long
                  imgpros->addData(dp.value, int(dp.y/2));

                  //finishs this image and moves on to the next one
				  if (lasty>(dp.y+1)){
                  	//generates date/time stamp to save the image
                  	time(&timer);
                  	timeinfo=localtime(&timer);
                  	fileName=asctime(timeinfo);
                  	
                  	//saves image as image in as the file specified by the time stamp
					save_bmp(compileImage(), "/"+fileName+".bmp");
                  	
			   		//creates a new ImageProcessor object to hold the next scan
			   		imgpros = new ImageProcessor (0);
				  }
               }catch(BlockingQueueStatus s){
                   if(s == BQ_TIMEOUT){
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
