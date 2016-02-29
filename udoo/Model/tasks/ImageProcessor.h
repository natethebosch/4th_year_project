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
#include <pixel.h>
#include <array2d.h>
#include <image_io.h>
#include <image_transforms.h>



using namespace std;
using namespace dlib;

class ImageProcessor {
	private:
		int currentX; //hold the x value of the next data point to be added
		int *lastY; //array holding the y values of the previous row of data points
		int *currentY; //array holding the y values of the current row of data points
		bool first; //true if the current row of data points is the first in this image otherwise false
		int currentXCompile; //holds the y value to which the data array has been completely compiled
		array2d<hsi_pixel> img; //holds the image
		float sensorData[HEIGHT][WIDTH-xSpacing]; //holds the added data points as well as the interpolated values between
		void yCompile();
		void xCompileTo(int botY);
	
		
		
	
	public:
		ImageProcessor(int i); //constructor
		void addData (float value, int y);
		void displayData ();
		array2d<hsi_pixel>& compileImage();
		array2d<hsi_pixel>& getImage();
		float (&getData())[HEIGHT][WIDTH-xSpacing];
		

};



class ImageProcessor: public Task {
    BlockingQueueReceiver<SensorDataPoint> input;
    
public:
    ImageProcessor(BlockingQueueReceiver<SensorDataPoint> _input): Task("ImageProcessor", 20){
        input = _input;
    };
    
    void run(void* argc){
        // image processor main logic goes here

          SensorDataPoint dp;
          float lasty=0.0;
          
          ImageProcessor imgpros=new ImageProcessor (0);
          
          // infinite loop
          for(;;){ 
               // fetch from the buffer
         
               try{
                  dp = input.take();
                  imgpros[count].addData(dp.value, int(dp.y*5));//y value multiplied by 5 since height is 500 and the scanner is 100cm long
                  //finishs this image and moves on to the next one
				  if (lasty>(dp.y+1)){
                  	//assigns the data and image to the arrays
                  	assign_image(images[count,imgpros.compileImage()]);
                  	dataSets[count]=imgpros.getData();
                  	//moves to the next space in the data/image arrays
               		if (count<300){
               			count++;
			   		}
			   		else{
			   			count=0;
			   		}
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
