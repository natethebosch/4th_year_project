#define MAXVALUE 255
#define DIPIN 0


int sensorLocations[24]={0};
double sensorValue=0;
int movedY = 0;
int xVal =0;
int yVal =0;

void setup() {
  // put your setup code here, to run once:
  pinMode (DIPIN, INPUT);
  for (int i=0; i<24; i++){
    pinMode (sensorLocations[i],INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead (DIPIN)==1){
    for (int i=0; i<24; i++){
      sensorValue= analogRead(sensorLocations[i])*255/MAXVALUE;
      xVal=i*20;
      yVal=0;
      
      
    }
  }
}
