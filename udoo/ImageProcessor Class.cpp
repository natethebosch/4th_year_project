#include "ImageProcessor Class.h"


using namespace std;
using namespace dlib;


//initialises an image of HEIGHT and WIDTH
ImageProcessor::ImageProcessor (int i){
	//HEIGHT=y;
	//WIDTH=x;
	//xSpacing=WIDTH/MAJORXS;
	//assign_image(img, array2d<hsi_pixel> (HEIGHT, (WIDTH-xSpacing)));
	currentX =0;
	currentY =new int[MAJORXS];
	lastY =new int[MAJORXS];
	currentXCompile=0;
	first=true;
}

//adds data point collected by sensors
void ImageProcessor::addData(int value, int y){
	
	currentY[currentX]=y;
	sensorData[y][currentX*xSpacing]=value;
	currentX++;
	if (currentX==MAJORXS){
		currentX=0;
		if(first==true){ 
			first=false;
		}
		else{
			yCompile();
			xCompileTo(currentY[0]);
		}
		for (int i=0; i<MAJORXS; i++){
			lastY[i]=currentY[i];
		}
	} 
}

//interpolates along the major columns
void ImageProcessor::yCompile(){
	double row;
	double diff;
	int subColumn;
	int lastYloc;
	int currentYloc;
	double topVal;
	double botVal;
	for (int column=0; column < MAJORXS; column++){
		row=1.0;
		subColumn=column*xSpacing;
		lastYloc=lastY[column];
		currentYloc=currentY[column];
		topVal=sensorData[lastYloc][subColumn];
		botVal=sensorData[currentYloc][subColumn];
		diff=double(currentYloc-lastYloc);
		while (row<diff){
			sensorData[int(row+lastYloc)][subColumn]=((diff-row)/diff)*topVal+(row/diff)*botVal;
			row+=1.0;
		}
	}
}

//interpolates allong the rows between the major columns
void ImageProcessor::xCompileTo(int botY){
	for (int row = currentXCompile; row<=botY; row++){
		for (int column=1; column<WIDTH; column++){
			int leftMajorColumn= (column/xSpacing)*xSpacing;
			double shift= column%xSpacing; 
			if (shift != 0){
				sensorData[row][column]= ((xSpacing-shift)/xSpacing)*sensorData[row][leftMajorColumn] +(shift/xSpacing)*sensorData[row][(leftMajorColumn+xSpacing)];
			}
		}
	}
	currentXCompile=botY;
}

void ImageProcessor::displayData(){
	for (int a=0; a<HEIGHT; a++){
			for (int b=0; b<WIDTH; b++){
				 cout<<sensorData[a][b]<<"	";
			}
			cout<< "\n\n";
		}
		cout<< "\n\n";
}


array2d<hsi_pixel>& ImageProcessor::compileImage(){
	int high=currentY[MAJORXS];
	assign_image(img, array2d<hsi_pixel> (HEIGHT, (WIDTH-xSpacing)));
	hsi_pixel* pixel;
	for (int row=0; row<HEIGHT; row++){
		for (int column=0; column<(WIDTH-xSpacing); column++){
			pixel = &img[row][column];
			if (sensorData[row][column]){
				pixel->i=150;
				pixel->s=255;
				pixel->h=((sensorData[row][column]*255)/MAXREADING);
			}
		}
	}
	return img;
}

array2d<hsi_pixel>& ImageProcessor::getImage(){
	return img;
}

