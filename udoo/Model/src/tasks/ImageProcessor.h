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
#define MAX_IMG_COUNT 15
#define MAX_CSV_COUNT 5
#define MAX_CSV_SIZE 5*2e10 // 5kbs


#include <vector>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Magick++.h>

using namespace std;

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
    
    
    
    //holds the added data points as well as the interpolated values between
    int sensorData[HEIGHT+1][WIDTH+1];
    float csvSensorData[HEIGHT+1][WIDTH+1];
    
    //holds date/timestamp image name
    time_t timer;
    struct tm *timeinfo;
    string timeName;
    string fileName;
    
    string _outputDirectory;
    
    /**
     * returns date-time format as YYYY-MM-DD-HH-II-SS
     */
    std::string getCurrentDateTime();
    
    bool has_suffix(const std::string &str, const std::string &suffix);
    
    bool file_exists (const std::string& name);
    std::vector<std::string> listFilesInDirWithExtension(std::string dir, std::string ext);
    
    /**
     * Updates manifest.json with a list of images and csvs which can be accessed
     */
    void updateManifest();
    
    /**
     * Moves current.csv to cache if it's larger than MAX_CSV_SIZE (bytes)
     * and creates new current.csv if non exists with header fields
     */
    void updateCSVFile();
    
    /**
     * Removes oldest photos when outputDirectory contains more than 
     * MAX_IMG_COUNT images
     */
    void cleanOldFiles();
    
    void yCompile();
    void xCompileTo(int botY);
    
    BlockingQueue<SensorDataPoint> *input;
    
public:
    
    void addData (float value, int y, int x);
    void displayData ();
    
    /**
     * Creates and image from the data in sensorData and saves it
     * as the filename specified
     */
    void compileImage(std::string filename);
    
    void run(void* cookie);
    
    //constructor/reseter
    void empty();
    
    ImageProcessor(BlockingQueue<SensorDataPoint> *_input, std::string outputDirectory): Task("ImageProcessor", 20){
//    	empty();
        input = _input;
        _outputDirectory = outputDirectory;
    };
    

};

#endif //_IMAGEPROCESSOR_H
