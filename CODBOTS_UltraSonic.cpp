#include <CODBOTS_UltraSonic.h>

CODBOTS_UltraSonic::CODBOTS_UltraSonic(int pin_trig_, int pin_echo_)
{
    pin_trig = pin_trig_;
    pin_echo = pin_echo_;
}

void CODBOTS_UltraSonic::begin(int avg_samples)
{
    begin(0, 1, 1, avg_samples);
}

void CODBOTS_UltraSonic::begin(int start_, int end_, int partitions_count_, int avg_samples_)
{
    pinMode(pin_trig, OUTPUT);
    pinMode(pin_echo, INPUT);

    start = start_;
    end = end_;

    if (partitions != nullptr)
    {
        delete[] partitions;
    }
    partitions_count = partitions_count_;

    partitions = new UltrasonicDistance[partitions_count];
    Serial.println("PCOUNT:" + String(partitions_count));

    if (partitions_count <= 1)
    {
        partition_size = 1;
        partitions[0] = UltrasonicDistance(90, avg_samples_, partition_size);
    }
    else
    {
        partition_size = abs(end_ - start_) / partitions_count;
        Serial.println("PSIZE:" + String(partition_size));
        int count = 0;
        for (int a = start_; a < end_; a += partition_size)
        {
            Serial.println("PARTITION:" + String(count) + "\t" + String(a) + "\t" + String(avg_samples_) + "\t" + String(partition_size));
            partitions[count] = UltrasonicDistance(a, avg_samples_, partition_size);
            count++;
        }
    }
    scanindex = partitions_count / 2;
}

float CODBOTS_UltraSonic::readSensor()
{
    int delay_between_reads = millis() - lastreadtime;

    if (delay_between_reads < 5)
    {
        delay(5 - delay_between_reads);
    }
    digitalWrite(pin_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_trig, LOW);

    duration_us = pulseIn(pin_echo, HIGH, 9000);
    lastreadtime = millis();
    if (duration_us < 2)
    {
        duration_us = 6500;
    }
    distance_cm = 0.017 * duration_us;
    // Serial.println(String(duration_us) + "\t" + String(distance_cm));
    return distance_cm;
}

void CODBOTS_UltraSonic::read()
{
    int n = partitions_count / 2;
    partitions[n].setDistance(readSensor());
}

void CODBOTS_UltraSonic::readIndex(int index)
{
    if (!isInIndexRange(index))
    {
        return;
    }
    partitions[index].setDistance(readSensor());
}

void CODBOTS_UltraSonic::read(int angle)
{
    if (!isInAngleRange(angle))
    {
        return;
    }
    for (int n = 0; n < partitions_count; n++)
    {
        if (partitions[n].inRange(angle))
        {
            partitions[n].setDistance(readSensor());
            return;
        }
    }
}

float CODBOTS_UltraSonic::getDistance()
{
    int n = partitions_count / 2;
    return partitions[n].getDistance();
}

float CODBOTS_UltraSonic::getDistanceIndex(int index)
{
    if (!isInIndexRange(index))
    {
        return -1;
    }
    return partitions[index].getDistance();
}

float CODBOTS_UltraSonic::getDistance(int angle)
{
    if (!isInAngleRange(angle))
    {
        return -1;
    }
    for (int n = 0; n < partition_size; n++)
    {
        if (partitions[n].inRange(angle))
        {
            return partitions[n].getDistance();
        }
    }
    return -1;
}

float *CODBOTS_UltraSonic::getDistanceArray()
{
    float *distances = new float[partitions_count];

    for (int i = 0; i < partitions_count; ++i)
    {
        distances[i] = partitions[i].getDistance();
    }

    return distances;
}

int *CODBOTS_UltraSonic::getAngleArray()
{
    int *angles = new int[partitions_count];

    for (int i = 0; i < partitions_count; ++i)
    {
        angles[i] = partitions[i].getAngle();
    }

    return angles;
}

void CODBOTS_UltraSonic::reset()
{
    Serial.println("SERVO ARRAY RESET");
    for (int n = 0; n < partitions_count; n++)
    {
        partitions[n].reset();
    }
}

bool CODBOTS_UltraSonic::scan(bool shift)
{
    // Serial.println(scanindex);

    partitions[scanindex].setDistance(readSensor());
    while (!partitions[scanindex].filled())
    {
        partitions[scanindex].setDistance(readSensor());
    }

    if (partitions[scanindex].filled() && shift)
    {
        nextScanIndex();
    }
    return true;
}

bool CODBOTS_UltraSonic::isIndexFilled()
{
    return partitions[scanindex].filled();
}

bool CODBOTS_UltraSonic::isFilled()
{
    for (int n = 0; n < partitions_count; n++)
    {
        if (!partitions[n].filled())
        {
            return false;
        }
    }
    return true;
}

int CODBOTS_UltraSonic::nextScanIndex()
{
    if (scandir)
    {
        for (int n = 0; n < partitions_count; n++)
        {
            if (!partitions[n].filled())
            {
                scanindex = n;
                return scanindex;
            }
        }
    }
    else
    {
        for (int n = partitions_count - 1; n > -1; n--)
        {
            if (!partitions[n].filled())
            {
                scanindex = n;
                return scanindex;
            }
        }
    }
    scandir = !scandir;
    return scanindex;
}

bool CODBOTS_UltraSonic::isInIndexRange(int index)
{
    return index > -1 && index < partitions_count;
}

bool CODBOTS_UltraSonic::isInAngleRange(int angle)
{
    return angle >= start && angle <= end;
}

int CODBOTS_UltraSonic::getScanIndex()
{
    return scanindex;
}

bool CODBOTS_UltraSonic::getScanDir()
{
    return scandir;
}

int CODBOTS_UltraSonic::getAngle()
{
    if (scanindex == -1)
    {
        return -1;
    }
    return getAngle(scanindex);
}

int CODBOTS_UltraSonic::getAngle(int index)
{
    return partitions[index].getAngle() + (partition_size / 2);
}