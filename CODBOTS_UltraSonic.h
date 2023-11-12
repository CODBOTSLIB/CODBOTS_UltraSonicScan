
#ifndef CODBOTS_UltraSonic_h
#define CODBOTS_UltraSonic_h

#include <Arduino.h>
#include <ESP32Servo.h>
#include <UltrasonicDistance.h>

class CODBOTS_UltraSonic
{
public:
    CODBOTS_UltraSonic(int pin_trig_, int pin_echo_);
    void begin(int avg_samples);
    void begin(int start, int end, int partitions_count, int avg_samples);
    float readSensor();

    void read();
    void readIndex(int index);
    void read(int angle);

    float getDistance();
    float getDistanceIndex(int index);
    float getDistance(int angle);
    float *getDistanceArray();
    int *getAngleArray();

    void reset();
    bool scan(bool shift);

    int nextScanIndex();

    bool isIndexFilled();
    bool isFilled();

    bool isInIndexRange(int index);
    bool isInAngleRange(int angle);

    int getAngle();
    int getAngle(int index);

    int getScanIndex();
    bool getScanDir();

    int partition_size;   // single partition length
    int partitions_count; // count is this
    int scanindex = 0;
    int scandir = true;

private:
    int pin_trig;
    int pin_echo;

    int start;
    int end;

    float duration_us, distance_cm;

    UltrasonicDistance *partitions;

    long lastreadtime;
};

#endif