//Pins
#define DIPIN 0
#define IRPIN 0

//Constants
#define MAXVALUE 255
#define BEAMOPEN    1
#define BEAMCLOSED  0
#define HOLEDIST    2.415 //mm
#define BRIDGEDIST  1.067 //mm


int sensorLocations[24]={0};

double sensorValue=0;
int movedY = 0;
int xVal =0;
int yVal =0;
int holes=0;
int bridges=0;
int broken=0;

void setup() {
  // Set input pins to input mode
  pinMode (DIPIN, INPUT);
  for (int i=0; i<24; i++){
    pinMode (sensorLocations[i],INPUT);
  }

  broken=(digitalRead (IRPIN)==BEAMOPEN);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead (DIPIN)==1){
    for (int i=0; i<24; i++){
      //collect data from the sensors
      sensorValue= analogRead(sensorLocations[i])*255/MAXVALUE;
      xVal=i*20;
      yVal=holes*HOLEDIST + bridges*BRIDGEDIST;
      
      
    }
  }
}

//checks if the infared beams state has changed to determine y position
void checkBeam{
  if (broken){
    if (digitalRead(IRPIN)==BEAMCLOSED){
      broken=false;
      holes++;
    }
  }
  else {
    if (digitalRead(IRPIN)==BEAMOPEN){
      broken=true;
      bridges++;
    }
  }
}

