/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "ImageProcessor.h"


using namespace std;
using namespace dlib;

float getRed(int hue);
float getGreen(int hue);
float getBlue(int hue);

/**
 * initialises an image of HEIGHT and WIDTH
 */
void ImageProcessor::empty (){
	currentX =0;
	currentY =new int[MAJORXS];
	lastY =new int[MAJORXS];
	currentXCompile=0;
	first=true;
	
	//sensorData= new matrixHEIGHT, WIDTH);
	
//	for (int i=0; i<HEIGHT;i++){
//		for (int n=0; n<WIDTH-xSpacing; n++){
//			sensorData[i][n]=0;
//		}		
//	}
}

void ImageProcessor::run(void* cookie){
    // image processor main logic goes here
      SensorDataPoint dp;
      float lasty=0.0;
      ImageProcessor *imgpros = this;

      
      // infinite loop
      for(;;){ 
           // fetch from the buffer
     
           try{
              dp = input->take();
            
               // y value divided by 2 since height is 500 and the scanner is 1000mm long
              imgpros->addData(dp.value, int(dp.y/2));

              //finishs this image and moves on to the next one
			  if (lasty>(dp.y)){
              	//generates date/time stamp to save the image
              	time(&timer);
              	timeinfo=localtime(&timer);
              	fileName="/media/sf_Model/images/";
				timeName=asctime(timeinfo);
				fileName+=timeName.substr(0,timeName.size()-1);
				fileName+=".jpeg";
              	
              	Debug::output(fileName.c_str());
              	//saves image as image in as the file specified by the time stamp
				compileImage();
				save_jpeg(getImage(), fileName);
				
				//removes the image from 100 scans ago so as not to fill up memory
				remove (fileList[listIndex].c_str());
				fileList[listIndex]=fileName;
				listIndex++;
				
				//resets list index after 100 entries
				if (listIndex>99) listIndex=0;
              	
		   		//creates a new ImageProcessor object to hold the next scan
		   		imgpros->empty();
			  }
			  lasty=dp.y/2;
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

/**
 * adds data point collected by sensors
 */
void ImageProcessor::addData(float value, int y){
	
	//puts the y value into the array of current y values
	currentY[currentX]=y;
	
	//assigns the value to the coresponding spot in the data array
	sensorData(y,currentX*xSpacing)=value;
	
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
		topVal=sensorData(lastYloc,subColumn);
		
		//value of data point below
		botVal=sensorData(currentYloc,subColumn);
		
		//number of y values that need to be interpolated
		diff=double(currentYloc-lastYloc);
		while (row<diff){
			sensorData(int(row+lastYloc),subColumn)=((diff-row)/diff)*topVal+(row/diff)*botVal;
			row+=1.0;
		}
	}
}

/**interpolates allong the rows between the major columns*/
void ImageProcessor::xCompileTo(int botY){
	//Debug::output("x Compiling to ");
	//Debug::output(to_string(botY).c_str());
	for (int row = currentXCompile; row<=botY; row++){
		for (int column=1; column<WIDTH; column++){
			
			//finds the x value of the major column to the left of the point 
			int leftMajorColumn= (column/xSpacing)*xSpacing;
			
			//determines how far over from the major column the current column is
			int shift= column%xSpacing;
			
			//if the current column is not a major column set the points value to the weighted average
			//of the values in the major columns imediatly to its left and right
			if (shift != 0){
				sensorData(row,column)= ((xSpacing-shift)/xSpacing)* (sensorData(row,leftMajorColumn) +(shift/xSpacing)*(sensorData(row,(leftMajorColumn+xSpacing))));
			}
		}
	}
	//Debug::output("Compiled");
	currentXCompile=botY;
}

/**dispays numerical data for testing purposes*/
void ImageProcessor::displayData(){
	for (int a=0; a<HEIGHT; a++){
			for (int b=0; b<WIDTH; b++){
				 Debug::output((to_string(sensorData(a,b))).c_str());
				 Debug::output("	");
			}
			Debug::output("\n\n");
		}
		Debug::output ("\n\n");
}

/**creates an image from the data and returns a refrence to it*/
array2d<rgb_pixel>& ImageProcessor::compileImage(){
	
	//creates the image
	assign_image(img, array2d<rgb_pixel> (HEIGHT, (WIDTH-xSpacing)));
	
	rgb_pixel* pixel;
	for (int row=0; row<HEIGHT; row++){
		for (int column=0; column<(WIDTH-xSpacing); column++){
			pixel = &img[row][column];
			//sets the intensity and sateration to levels to make the colour change visable and sets the hue based on the data collected/interpolated
			if (sensorData(row,column)){
				pixel->red=int(getRed(int(sensorData(row,column)*360/MAXVAL)));
				pixel->green=int (getGreen(int(sensorData(row,column)*360/MAXVAL)));
				pixel->blue= int (getBlue(int(sensorData(row,column)*360/MAXVAL)));
			}
		}
	}
	return img;
}

/**returns a reference to the rasterized image*/
array2d<rgb_pixel>& ImageProcessor:: getImage(){
	return img;
}

/**returns a refernece to the array containing the numerical data
*from the scan and the interpolation*/

//int& ImageProcessor::getData(){
//	return *sensorData;
//}

float getRed(int hue){
	if ((hue>=60 && hue<120) || (hue>=240 && hue<300)){
			return CHROMA*(1-abs(((hue/60)%2)-1))*255;
	}
	else if((hue<60)||(hue>=300)){
			return CHROMA*255;
		}
		else return 0;
}

float getGreen(int hue){
	if ((hue<60)|| (hue >= 180 && hue < 240)){
		return CHROMA*(1-abs(((hue/60)%2)-1))*255;
	}
	else if (hue>=60&&hue<180){
			return CHROMA*255;
		}
		else return 0;
}

float getBlue(int hue){
	if((hue>=120 && hue<180) || (hue<300)){
			return CHROMA*(1-abs(((hue/60)%2)-1))*255;
		}
		else if ((hue>=180 && hue<300)){
			return CHROMA*255;
		}
		else {
			return 0;
		}
}

