#include <CODBOTS_UltraSonicScan.h>

CODBOTS_UltraSonicScan::CODBOTS_UltraSonicScan(int pin_trig_,int pin_echo_){
    pin_trig = pin_trig_;
    pin_echo = pin_echo_;
}

void CODBOTS_UltraSonicScan::begin(int avg_samples){
    begin(0, 1, 1,avg_samples);
}

void CODBOTS_UltraSonicScan::begin(int start_,int end_,int partitions_count_,int avg_samples_){
    pinMode(pin_trig, OUTPUT);
    pinMode(pin_echo, INPUT);

    start = start_;
    end = end_;

    if (partitions != nullptr) {
        delete [] partitions;
    }
    partitions_count = partitions_count_;

    partitions = new UltrasonicDistance[partitions_count];
    Serial.println("PCOUNT:"+String(partitions_count));
    
    if(partitions_count<=1){
        partition_size = 1; 
        partitions[0] = UltrasonicDistance(90,avg_samples_,partition_size);
    }else{
        partition_size = abs(end_ - start_) / partitions_count;
         Serial.println("PSIZE:"+String(partition_size));
        int count = 0;
        for (int a = start_; a < end_;a+=partition_size){
             Serial.println("PARTITION:"+String(count)+"\t"+String(a)+"\t"+String(avg_samples_)+"\t"+String(partition_size));
            partitions[count] = UltrasonicDistance(a,avg_samples_,partition_size);
            count++;
        }
    }
}

float CODBOTS_UltraSonicScan::readSensor(){
    digitalWrite(pin_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_trig, LOW);

    duration_us = pulseIn(pin_echo, HIGH,9000);
  if (duration_us == 0) {
    duration_us = 6200;
  }
    distance_cm = 0.017 * duration_us;

    return distance_cm;
}

void CODBOTS_UltraSonicScan::read(){
    partitions[0].setDistance(readSensor());
}

void CODBOTS_UltraSonicScan::readIndex(int index){
    if(!isInIndexRange(index)){
        return ;
    }
    partitions[index].setDistance(readSensor());
}

void CODBOTS_UltraSonicScan::read(int angle){
    if(!isInAngleRange(angle)){
        return ;
    }
    for (int n = 0; n < partitions_count;n++){
        if(partitions[n].inRange(angle)){
            partitions[n].setDistance(readSensor());
            return ;
        }
    }
}

float CODBOTS_UltraSonicScan::getDistance(){
    return partitions[0].getDistance();
}

float CODBOTS_UltraSonicScan::getDistanceIndex(int index){
    if(!isInIndexRange(index)){
        return -1;
    }
    return partitions[index].getDistance();
}

float CODBOTS_UltraSonicScan::getDistance(int angle){
    if(!isInAngleRange(angle)){
        return -1;
    }
    for (int n = 0; n < partition_size;n++){
        if(partitions[n].inRange(angle)){
           return partitions[n].getDistance();
        }
    }
    return -1;
}

void CODBOTS_UltraSonicScan::reset(){
    for (int n = 0; n < partitions_count;n++){
        partitions[n].reset();
    }
}

bool CODBOTS_UltraSonicScan::scan(bool shift){
   // Serial.println(scanindex);
 
        partitions[scanindex].setDistance(readSensor());
        if(partitions[scanindex].filled() && shift){
            Serial.println("Angle:" + String(partitions[scanindex].getAngle())+"\tDistace:"+ partitions[scanindex].getDistance());
            nextScanIndex();
           if(scanindex==-1){
             delay(2000);
           }
        }
        return true;
}

int CODBOTS_UltraSonicScan::nextScanIndex(){
    if(scandir){
        for (int n = 0; n < partitions_count;n++){
            if(!partitions[n].filled()){
                scanindex = n;
                return scanindex;
            }
        }
    }else{
        for (int n = partitions_count-1; n >-1;n--){
            if(!partitions[n].filled()){
                scanindex = n;
                return scanindex;
            }
        }
    }
    scandir = !scandir;
    reset();
    return scanindex;
}

bool CODBOTS_UltraSonicScan::isInIndexRange(int index){
    return index > -1 && index < partitions_count;
}

bool CODBOTS_UltraSonicScan::isInAngleRange(int angle){
     return angle >= start && angle <= end;
}

int CODBOTS_UltraSonicScan::getAngle(){
    if(scanindex==-1){
        return -1;
    }
     return partitions[scanindex].getAngle()+(partition_size/2);
}