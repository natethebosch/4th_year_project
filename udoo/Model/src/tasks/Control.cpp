/**
 * Project Realtime Solar Scanner Control
 * @author Nate Bosscher, Tyler d'Entremont
 * @version 1.0
 */


#include "Control.h"
#include "../sys/AudioRecorder.h"
#include "../type/SensorDataPoint.h"

/**
 * Control implementation
 */


using namespace std;

void Control::motorAway(){
    io.digitalWrite(MOTOR_CTRL_EN, true);
    io.digitalWrite(MOTOR_B, false);
    io.digitalWrite(MOTOR_A, true);
}

void Control::motorHome(){
    io.digitalWrite(MOTOR_CTRL_EN, true);
    io.digitalWrite(MOTOR_A, false);
    io.digitalWrite(MOTOR_B, true);
}

void Control::stopScanner() {
    io.digitalWrite(MOTOR_CTRL_EN, false);
    io.digitalWrite(MOTOR_A, false);
    io.digitalWrite(MOTOR_B, false);
}


bool Control::homeScanner() {
    motorHome();
    
    if(!io.digitalRead(HOME_SENSOR))
        return true;
    
	stopScanner();
    return false;
}

bool Control::awayScanner() {
    motorAway();
	
    if(!io.digitalRead(AWAY_SENSOR)){
        return true;
	}
    
	stopScanner();
    return false;
}

bool Control::scanHome() {
    motorHome();
	
	if (!io.digitalRead(HOME_SENSOR)){
        return true;
	}
    
	stopScanner();
    return false;
}

bool Control::scanAway() {
    motorAway();
	
    if(!io.digitalRead(AWAY_SENSOR)){
        return true;
    }
    
	stopScanner();
    return false;
}

bool Control::updateHoleIndex(){
    if(scanPosition == UNKNOWN){
        return true;
    }
    
    if(lastCheck - rt_timer_read() < HOLE_TIMER_NS)
        return true;
    
    lastCheck = rt_timer_read();
    
    
    bool sensorValue = io.digitalRead(BREAK_BEAM);
    if(sensorValue == holeIndexValue){
        if(debounceCountDown == 0){
            holeIndexValue = sensorValue;
            
            if(scanDirection == TOWARDS_AWAY){
                holeIndex++;
            }else if(scanDirection == TOWARDS_HOME){
                holeIndex--;
            }
            
            std::string s = "";
            s += "Control: hole updated (";
            s += std::to_string(holeIndex);
            s += ")";
            Debug::output(s.c_str());
            
            return false;
        }else{
            debounceCountDown--;
        }
    }else{
        debounceCountDown = DEBOUNCE_PARAM;
    }
    
    return true;
}

bool Control::isAtScanPosition(){
    if(holeIndex % SCAN_EACH_N_HOLES == 0){
        return true;
    }else{
        return false;
    }
}

bool Control::performLineScan(){
    SensorDataPoint dp;
    
    // update MUX address
    
    io.digitalWrite(MUX_A0,
                    ((lineScanPosition & (0x01 << 0)) != 0));
    io.digitalWrite(MUX_A1,
                    ((lineScanPosition & (0x01 << 1)) != 0));
    io.digitalWrite(MUX_A2,
                    ((lineScanPosition & (0x01 << 2)) != 0));
    io.digitalWrite(MUX_A3,
                    ((lineScanPosition & (0x01 << 3)) != 0));
    io.digitalWrite(MUX_A4,
                    ((lineScanPosition & (0x01 << 4)) != 0));
    
    int value;
    value = ar.getSampleValue();
    
    dp.value = (float)value;
    dp.x = lineScanPosition;
    dp.y = holeIndex;
    
    _queue->put(dp);
    
    lineScanPosition++;
    
    if(lineScanPosition < N_SENSORS){
        return true;
    }else{
        return false;
    }
}

void Control::resetLineScanner(){
    lineScanPosition = 0;
}