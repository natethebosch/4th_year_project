#define HEIGHT 		500
#define WIDTH 		480
#define MAJORXS 	24
#define MAXREADING 	(255*24)
#define xSpacing	(WIDTH/MAJORXS)

#include <vector>
#include <iostream>
#include <string>
#include <dlib\pixel.h>
#include <dlib\array2d.h>
#include <dlib\image_io.h>
#include <dlib\image_transforms.h>



using namespace std;
using namespace dlib;


class ImageProcessor {
	private:
		//int HEIGHT;
		//int WIDTH;
		//int xSpacing;
		int currentX;
		int rowLength;
		//int MAJORXS;
		int *lastY;
		int *currentY;
		bool first;
		int currentXCompile;
		array2d<hsi_pixel> img;
		//std::vector < std::vector <double> > sensorData;	
		double sensorData[HEIGHT][WIDTH];
		void yCompile();
		void xCompileTo(int botY);
	
		
		
	
	public:
		ImageProcessor(int i);
		void addData (int value, int y);
		void displayData ();
		array2d<hsi_pixel>& compileImage();
		array2d<hsi_pixel>& getImage();
		

};
