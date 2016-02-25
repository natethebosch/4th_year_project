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
#define MAXREADING 	(255*24)
#define xSpacing	(WIDTH/MAJORXS)

#include <vector>
#include <iostream>
#include <string>
#include <dlib/pixel.h>
#include <dlib/array2d.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

using namespace std;
using namespace dlib;

class ImageProcessor: public Task {
private:
    int currentX;
    int rowLength;
    int *lastY;
    int *currentY;
    bool first;
    int currentXCompile;
    array2d<hsi_pixel> img;
    float sensorData[HEIGHT][WIDTH];
    void yCompile();
    void xCompileTo(int botY);
    
    BlockingQueueReceiver<SensorDataPoint> input;
    
public:
    ImageProcessor(BlockingQueueReceiver<SensorDataPoint> _input): Task("ImageProcessor", 20){
        input = _input;
    };
    
    void addData (float value, int y);
    void displayData ();
    array2d<hsi_pixel>& compileImage();
    array2d<hsi_pixel>& getImage();
    
    void run(){
        // image processor main logic goes here

          SensorDataPoint dp;
          int count=0;
          float lasty=0.0;
          
          imgpros[0]=new ImageProcessor (0);
          
          // infinite loop
          for(;;){ 
               // fetch from the buffer
         
               try{
                  dp = input.take();
                  imgpros[count].addData(dp.value, int(dp.y/2));
                  //finishs this image and moves on to the next one
				  if (lasty>(dp.y+1)){
                  	imgpros[count].compileImage();
               		if (count<300){
               			count++;
			   		}
			   		else{
			   			count=0;
			   		}
			   		//creates a new ImageProcessor object to hold the next scan
			   		imgpros[count]=new ImageProcessor (0);
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
