#include "ImageProcessor Class.h"
#include "stdlib.h"
#include "time.h"

//int main(int argc, char** argv) {
int main (){
int val;
ImageProcessor test (0);

srand(time(NULL));
for (int i=0; i<(HEIGHT/20);i++){
	for (int n=0; n<MAJORXS; n++){
		val=(std::rand()*255)/RAND_MAX;
		test.addData(val*n,(i*20));
	}
}

//test.addData(10,0);
//test.addData(20,01);
//test.addData(35,02);
//test.addData(40,03);
//test.addData(50,04);
//
//test.addData(20,121);
//test.addData(75,122);
//test.addData(45,123);
//test.addData(50,124);
//test.addData(60,125);
//
//test.addData(30,241);
//test.addData(40,242);
//test.addData(100,243);
//test.addData(60,244);
//test.addData(70,245);
//
//test.addData(40,361);
//test.addData(50,362);
//test.addData(60,363);
//test.addData(25,364);
//test.addData(80,365);
//
//test.addData(50,481);
//test.addData(60,482);
//test.addData(70,483);
//test.addData(85,484);
//test.addData(90,485);

//test.displayData();

save_bmp(test.compileImage(),"C:\\Users\\sparr_000\\Documents\\4th year project\\image.bmp");

return 0;

}
