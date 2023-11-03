
#ifndef UltrasonicDistance_h
#define UltrasonicDistance_h

#include <Arduino.h>
#include <ESP32Servo.h>


class UltrasonicDistance
{
    public:
        UltrasonicDistance();
        UltrasonicDistance(int angle_,int avglen_,int partition_size_);
        bool setDistance(float distance);
        
        float getDistance();

        void reset();
        bool filled();
        bool inRange(int angle_);
        int getAngle();

    private:
        int angle;
        int avglen;
        int partition_size;

        float* array = nullptr;
        int currentindex;

        bool readed=false;
};


#endif