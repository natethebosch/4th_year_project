/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "ImageProcessor.h"


using namespace std;
using namespace dlib;


/**
 * initialises an image of HEIGHT and WIDTH
 */
void ImageProcessor::empty (){
	currentX =0;
	currentY =new int[MAJORXS];
	lastY =new int[MAJORXS];
	currentXCompile=0;
	first=true;
	for (int i=0; i<HEIGHT;i++){
		for (int n=0; n<WIDTH-xSpacing; n++){
			sensorData[i][n]=0;
		}		
	}
}

/**
 * adds data point collected by sensors
 */
void ImageProcessor::addData(float value, int y){
	
	//puts the y value into the array of current y values
	currentY[currentX]=y;
	
	//assigns the value to the coresponding spot in the data array
	sensorData[y][currentX*xSpacing]=value;
	
	//moves to the next major column
	currentX++;
	
	//if the previous column was the last major colum
	if (currentX==MAJORXS){
		
		currentX=0;
		
		//if first row don't compile and say its no longer the first row
		if(first==true){ 
			first=false;
		}
		
		//if its not the first row compile the major columns then compile the rows to the heighest y data point
		else{
			yCompile();
			xCompileTo(currentY[0]);
		}
		
		//assigns the values of the current row of y values to the array of the previous row of y values
		for (int i=0; i<MAJORXS; i++){
			lastY[i]=currentY[i];
		}
	} //end if was last major column
}

/**interpolates along the major columns*/
void ImageProcessor::yCompile(){
	double row;
	double diff;
	int subColumn;
	int lastYloc;
	int currentYloc;
	double topVal;
	double botVal;
	
	//goes through all major columns and interpolates the data for the major columns
	for (int column=0; column < MAJORXS; column++){
		row=1.0;
		
		//index of the major column in the data array
		subColumn=column*xSpacing;
		
		//the upper data points y value
		lastYloc=lastY[column];
		
		//the lower data points y value
		currentYloc=currentY[column]; 
		
		//value of data point above
		topVal=sensorData[lastYloc][subColumn];
		
		//value of data point below
		botVal=sensorData[currentYloc][subColumn];
		
		//number of y values that need to be interpolated
		diff=double(currentYloc-lastYloc);
		while (row<diff){
			sensorData[int(row+lastYloc)][subColumn]=((diff-row)/diff)*topVal+(row/diff)*botVal;
			row+=1.0;
		}
	}
}

/**interpolates allong the rows between the major columns*/
void ImageProcessor::xCompileTo(int botY){
	for (int row = currentXCompile; row<=botY; row++){
		for (int column=1; column<WIDTH; column++){
			
			//finds the x value of the major column to the left of the point 
			int leftMajorColumn= (column/xSpacing)*xSpacing;
			
			//determines how far over from the major column the current column is
			int shift= column%xSpacing;
			
			//if the current column is not a major column set the points value to the weighted average
			//of the values in the major columns imediatly to its left and right
			if (shift != 0){
				sensorData[row][column]= ((xSpacing-shift)/xSpacing)*sensorData[row][leftMajorColumn] +(shift/xSpacing)*sensorData[row][(leftMajorColumn+xSpacing)];
			}
		}
	}
	currentXCompile=botY;
}

/**dispays numerical data for testing purposes*/
void ImageProcessor::displayData(){
	for (int a=0; a<HEIGHT; a++){
			for (int b=0; b<WIDTH; b++){
				 cout<<sensorData[a][b]<<"	";
			}
			cout << "\n\n";
		}
		cout << "\n\n";
}

/**creates an image from the data and returns a refrence to it*/
array2d<hsi_pixel>& ImageProcessor::compileImage(){
	
	//creates the image
	assign_image(img, array2d<hsi_pixel> (HEIGHT, (WIDTH-xSpacing)));
	
	hsi_pixel* pixel;
	for (int row=0; row<HEIGHT; row++){
		for (int column=0; column<(WIDTH-xSpacing); column++){
			pixel = &img[row][column];
			//sets the intensity and sateration to levels to make the colour change visable and sets the hue based on the data collected/interpolated
			if (sensorData[row][column]){
				pixel->i=150;
				pixel->s=255;
				pixel->h=((sensorData[row][column]*255)/MAXREADING);
			}
		}
	}
	return img;
}

/**returns a reference to the rasterized image*/
array2d<hsi_pixel>& ImageProcessor::getImage(){
	return img;
}

/**returns a refernece to the array containing the numerical data
*from the scan and the interpolation*/

float* ImageProcessor::getData(){
	return *sensorData;
}

