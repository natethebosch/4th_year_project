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
#include <stdlib.h>

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
    matrix<int, HEIGHT+1, WIDTH+1> sensorData;
    
    //holds date/timestamp image name
    time_t timer;
    struct tm *timeinfo;
    string fileName;
    
    //list of saved files
    int listIndex;
    string fileList[100];
    
    void yCompile();
    void xCompileTo(int botY);
    
    BlockingQueue<SensorDataPoint> *input;
    
public:
    
    void addData (float value, int y);
    void displayData ();
    array2d<rgb_pixel>& compileImage();
    array2d<rgb_pixel>& getImage();
	//vector<vector<int>> getData();
    void run(void* cookie);
    
    //constructor/reseter
    void empty();
    
    ImageProcessor(BlockingQueue<SensorDataPoint> *_input): Task("ImageProcessor", 20){
    	empty();
    	listIndex=0;
        input = _input;
    };
    

};

#endif //_IMAGEPROCESSOR_H
