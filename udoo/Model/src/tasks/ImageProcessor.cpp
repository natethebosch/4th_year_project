/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "ImageProcessor.h"


using namespace std;

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
}

/**
 * (Source: http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c)
 */
bool ImageProcessor::file_exists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

std::string ImageProcessor::getCurrentDateTime(){
    char datebuff[32];
    
    // initialize datebuff
    time(&timer);
    timeinfo=localtime(&timer);
    
    strftime (datebuff, 32,"%F-%T", timeinfo);
    
    // replace [:\] with -
    int i = 0;
    while(datebuff[i] != '\0'){
        if(datebuff[i] == ':' || datebuff[i] == '\\'){
            datebuff[i] = '-';
        }
        i++;
    }
    
    return std::string(datebuff);
}

void ImageProcessor::run(void* cookie){
    
    // image processor main logic goes here
    SensorDataPoint dp;
    float lasty=0.0;
    
    std::string date = getCurrentDateTime();
    
    // open default csv file
    std::string csvCannonicalName = _outputDirectory + "/current.csv";
    
    // if file already exists, move it to a timestamp formatted location
    if(file_exists(csvCannonicalName)){
        
        if(rename(csvCannonicalName.c_str(), (_outputDirectory + "/" + date + ".csv").c_str()) != 0){
            Debug::output("Couldn't move file current.csv");
        }
        
        updateManifest();
    }
    
    updateCSVFile();

    // infinite loop
      for(;;){
          
          // check if we need to stop
          if(hasTerminateSignal()){
              return;
          }
     
           try{
               
               // loop until the queue gives us an item or hasTerminateSignal causes exit
               for(;;){
                   try{
                       if(hasTerminateSignal()){
                           return;
                       }
                       
                       // fetch from the buffer
                       dp = input->take(10);
                       
                       // break cause we got something
                       break;
                   }catch(BlockingQueueStatus err){
                       if(err != BQ_TIMEOUT){
                           Debug::output("Unknown error in ImageProcessor::run input->take");
                           Debug::output(std::to_string(err).c_str());
                       }else{
                       		//Debug::output("no input wait");
                           sleep_millis(100);
                       }
                   }
               }
               // y value divided by 2 since height is 500 and the scanner is 1000mm long
               addData(dp.value, int(dp.y/2),dp.x);
               
               ofstream csvFile(csvCannonicalName.c_str(), std::fstream::app);
               csvFile << "\"" << date << "\"," << dp.x << "," << dp.y << ",\"" << dp.value << "\"\n";
               csvFile.close();


               
              //finishs this image and moves on to the next one
			  if (lasty>(dp.y)){
                  
                    // update for the csv
                    date = getCurrentDateTime();

                    fileName = date;
                    fileName += ".jpg";

                    Debug::output(fileName.c_str());

                    //saves image as image in as the file specified by the time stamp
                    compileImage(_outputDirectory + "/" + fileName);
                  
                    // clean up filesystem
                    updateCSVFile();
                    cleanOldFiles();
                    updateManifest();
                  

                    //creates a new ImageProcessor object to hold the next scan
                    empty();
			  }
               
			  lasty=dp.y;
           }catch(BlockingQueueStatus s){
               if(s == BQ_TIMEOUT){
               	Debug::output("queue timout");
                  continue;
              }else{
                 Debug::output("Irrecoverable error. Exiting...");
                   exit(-1);
               }
           }
      }
}

void ImageProcessor::updateCSVFile(){
    
    // get file size
    std::ifstream in((_outputDirectory + "/current.csv").c_str(), std::ifstream::ate | std::ifstream::binary);
    size_t len = in.tellg();
    
    // if file is above allowed, move to cached file
    if(len > MAX_CSV_SIZE){
        int result = rename((_outputDirectory + "/current.csv").c_str(), (_outputDirectory + "/" + getCurrentDateTime() + ".csv").c_str());
        if(result != 0){
            Debug::output("Couldn't move current.csv");
        }
    }
    
    // if file doesn't exist, create it and add csv header
    if(!file_exists(_outputDirectory + "/current.csv")){
        std::ofstream csvFile;
        csvFile.open((_outputDirectory + "/current.csv").c_str(), std::ofstream::out);
        
        csvFile << "\"Date of Capture Set\",x,y\n";
        
        csvFile.close();
    }
}

/**
 * Source: http://stackoverflow.com/questions/20446201/how-to-check-if-string-ends-with-txt
 */
bool ImageProcessor::has_suffix(const std::string &str, const std::string &suffix){
    return str.size() >= suffix.size() &&
    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * returns a list of file names within a directory that have
 * the extension given
 */
std::vector<std::string> ImageProcessor::listFilesInDirWithExtension(std::string dir, std::string ext){
    DIR *d;
    struct dirent *ent;
    std::vector<std::string> list;
    std::string name;
    
    // try to open the directory
    if((d = opendir(_outputDirectory.c_str())) != NULL){
        
        // loop over the items
        while ((ent = readdir (d)) != NULL) {
            name = std::string(ent->d_name);
            
            // check for extension
            if(has_suffix(name, ext)){
                list.push_back(name);
            }
        }
        
        closedir (d);
    }else{
        Debug::output("Couldn't open directory for listing");
    }
       
   return list;
}

/**
 * Removes oldest photos when outputDirectory contains more than
 * MAX_IMG_COUNT images
 */
void ImageProcessor::cleanOldFiles(){
    std::vector<std::string> list = listFilesInDirWithExtension(_outputDirectory, ".jpg");
    std::string cannonicalPath;
    
    if(list.size() > MAX_IMG_COUNT){
        // sort
        std::sort (list.rbegin(), list.rend());
        
        while(list.size() > MAX_IMG_COUNT){
            
            // skip current.jpg
            if(list.back() == "current.jpg"){
                list.pop_back();
                continue;
            }
            
            cannonicalPath = _outputDirectory + "/" + list.back();
            list.pop_back();
            
            if(remove(cannonicalPath.c_str()) != 0){
                Debug::output("Couldn't delete file");
                Debug::output(cannonicalPath.c_str());
            }
        }
    }
    
    // do the same for csvs
    list = listFilesInDirWithExtension(_outputDirectory, ".csv");
    
    if(list.size() > MAX_CSV_COUNT){
        // sort
        std::sort (list.rbegin(), list.rend());
        
        while(list.size() > MAX_CSV_COUNT){
            
            // skip current.csv
            if(list.back() == "current.csv"){
                list.pop_back();
                continue;
            }
            
            cannonicalPath = _outputDirectory + "/" + list.back();
            list.pop_back();
            
            if(remove(cannonicalPath.c_str()) != 0){
                Debug::output("Couldn't delete file");
                Debug::output(cannonicalPath.c_str());
            }
        }
    }
}
       
void ImageProcessor::updateManifest(){
    std::vector<std::string> list;
    
    // get output stream
    std::string cannonicalName = _outputDirectory + "/manifest.json";
    std::ofstream manifest(cannonicalName.c_str(), std::ofstream::out);
    
    // get date (http://stackoverflow.com/questions/9527960/how-do-i-construct-an-iso-8601-datetime-in-c)
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    std::string date = std::string(buf);
    
    // write to manifest
    manifest << "{\n";
    manifest << "\t\"updatedAt\":\"" << date << "\",\n";
    manifest << "\t\"images\":[";
    
    // get images from dir
    list = listFilesInDirWithExtension(_outputDirectory, ".jpg");
    
    // loop over all images
    for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it){
        
        // add comma only if one line has already been printed
        if(it != list.begin()){
            manifest << ",\n";
        }else{
            manifest << "\n";
        }
        
        manifest << "\t\t\"" << *it << "\"";
    }
    
    if(list.size() != 0){
        manifest << "\n";
    }
    
    // end of images property
    manifest << "\t],\n";
    
    
    manifest << "\t\"csvs\":[";
    
    // get images from dir
    list = listFilesInDirWithExtension(_outputDirectory, ".csv");
    
    // loop over all csvs
    for (std::vector<std::string>::iterator it = list.begin() ; it != list.end(); ++it){
        
        // add comma only if one line has already been printed
        if(it != list.begin()){
            manifest << ",\n";
        }else{
            manifest << "\n";
        }
        
        manifest << "\t\t\"" << *it << "\"";
    }
    
    if(list.size() != 0){
        manifest << "\n";
    }
    
    // end of csv property
    manifest << "\t]\n";
    manifest << "}";
    
    manifest.close();
}

/**
 * adds data point collected by sensors
 */
void ImageProcessor::addData(float value, int y, int x){
	
	//puts the y value into the array of current y values
	currentY[currentX]=y;
	
	//assigns the value to the coresponding spot in the data array
	sensorData[y][x]=value;
	//cout<< "added data point " << x<<","<<y<<":"<<sensorData[y][x]<<"\n";
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
	
	//goes through all major columns and interpolates the data for the major rows
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
				sensorData[row][column]= (
                                        (float)( xSpacing-shift)/xSpacing) *
                                        (float)(sensorData[row][leftMajorColumn] +
                                             (float)(shift/xSpacing)*
                                             (float)(sensorData[row][(leftMajorColumn+xSpacing)])
                                        );
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
				 Debug::output((to_string(sensorData[a][b])).c_str());
				 Debug::output("	");
			}
			Debug::output("\n\n");
		}
		Debug::output ("\n\n");
}

/**
 * Creates and image from the data in sensorData and saves it
 * as the filename specified
 */
void ImageProcessor::compileImage(std::string filename){
    
    std::cout << filename << "\n";
	
    try{
        //creates the image
        Magick::Image image(Magick::Geometry(HEIGHT,WIDTH - xSpacing), "white");
        
        // Set the image type to TrueColor DirectClass representation.
        image.type(Magick::TrueColorType);
        
        // Ensure that there is only one reference to underlying image
        // If this is not done, then image pixels will not be modified.
        image.modifyImage();
        
        // Allocate pixel view
        Magick::Pixels view(image);
        
        // get pixel iterator
        Magick::PixelPacket *pixels = view.get(0,0,HEIGHT,WIDTH - xSpacing);
        
        for(ssize_t y = 0; y < HEIGHT; y++){
            for(ssize_t x = 0; x < (WIDTH - xSpacing); x++){
                
                    *pixels++ = Magick::ColorHSL(float(sensorData[y][x])/float(MAXVAL)*float(360.0), 1, .5);
                    cout<<sensorData[y][x]<<" ";
                
            }
            cout<<"\n";
        }
        
        // Save changes to image.
        view.sync();
        
        // save image
        image.write(filename.c_str());
        
        // copy file to current.jpg
        // (Source: http://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way)
        std::ifstream  src(filename.c_str(), std::ios::binary);
        std::ofstream  dst((_outputDirectory + "/current.jpg").c_str(),   std::ios::binary);
        dst << src.rdbuf();
    
    }catch( std::exception &error_ ){
        cout << "Caught exception: " << error_.what() << endl;
    }
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

