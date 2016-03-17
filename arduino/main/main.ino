//Pins
#define DIPIN 8
#define IRPIN 9
#define MUXA0 1
#define MUXA1 2
#define MUXA2 3
#define MUXA3 4
#define MUXA4 5
#define MUXIN 6
#define START 7

//Constants
#define BEAMOPEN    1
#define BEAMCLOSED  0
#define HOLEDIST    7.558 //mm
#define BRIDGEDIST  2.925 //mm
#define WAIT        1

//array to hold sensor locations
//basically 24 constants but can't itterate #defines
int sensorLocations[24]={0};

//variables
int sensorValue=0;
int xVal =0;
int yVal =0;
int holes=0;
int bridges=0;
bool broken=0;

void setup() {
  Serial.begin(115200);
  Serial.write("Initializing...");
  
  // Set input pins to input mode
  pinMode (DIPIN, INPUT);
  for (int i=0; i<24; i++){
    pinMode (sensorLocations[i],INPUT);
  }
  pinMode ((MUXA0), OUTPUT);
  pinMode ((MUXA1), OUTPUT);
  pinMode ((MUXA2), OUTPUT);
  pinMode ((MUXA3), OUTPUT);
  pinMode ((MUXA4), OUTPUT);
  broken=(digitalRead (IRPIN)==BEAMOPEN);

  Serial.println("  done!");
}

void loop() {
  if (digitalRead (START)==1){
    holes=0;
    bridges=0;
  }
  // put your main code here, to run repeatedly:
  //check beam to know y value
  checkBeam();
  if (digitalRead (DIPIN)==1){
    for (int i=0; i<24; i++){
      //sets MUX to get desired input
      if ((i& 16)==16){
        digitalWrite((MUXA4), HIGH);
        } else {
          digitalWrite ((MUXA4), LOW);
        }
      if ((i& 8)==8){
        digitalWrite((MUXA3), HIGH);
      }else {
        digitalWrite ((MUXA3), LOW);
      }
      if ((i& 4)==4){
        digitalWrite((MUXA2), HIGH);
      }else {
        digitalWrite ((MUXA2), LOW);
      }
      if ((i&2)==2){
        digitalWrite((MUXA1), HIGH);
      }else {
        digitalWrite ((MUXA1), LOW);
      }
      if ((i&1)==1){
        digitalWrite ((MUXA0), HIGH);
      }else {
        digitalWrite ((MUXA0), LOW);
      }
      delay (WAIT);
      //check beam to know y value
      checkBeam();
      //collect data from the sensors
      xVal=i*20;
      yVal=int(float(holes*HOLEDIST) + float (bridges*BRIDGEDIST));
      sensorValue= analogRead(MUXIN);
      
      //send data to udoo
      Serial.print(xVal);
      Serial.print(',');
      Serial.print(yVal);
      Serial.print(',');
      Serial.print(sensorValue);
      Serial.print('\n');
      
    }
  }
}

//checks if the infared beams state has changed to determine y position
void checkBeam(){
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

