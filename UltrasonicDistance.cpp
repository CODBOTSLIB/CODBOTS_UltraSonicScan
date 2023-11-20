#include <UltrasonicDistance.h>

UltrasonicDistance::UltrasonicDistance()
{
}

UltrasonicDistance::UltrasonicDistance(int angle_, int avglen_, int partition_size_)
{
    angle = angle_;
    avglen = avglen_;
    if (array != nullptr)
    {
        delete[] array;
    }
    array = new float[avglen];
    for (int i = 0; i < avglen; i++)
    {
        array[i] = 0;
    }
    partition_size = partition_size_;
}

bool UltrasonicDistance::setDistance(float distance)
{
    // Serial.println("CINDEX:"+String(currentindex)+"AVHLEN:"+String(avglen));
    array[currentindex] = distance;
    currentindex++;
    if (currentindex >= avglen)
    {
        currentindex = 0;
        readed = true;
    }
    return readed;
}

void UltrasonicDistance::reset()
{
    readed = false;
    currentindex = 0;
}

bool UltrasonicDistance::filled()
{
    return readed;
}

bool UltrasonicDistance::inRange(int angle_)
{
    return (angle - (partition_size / 2)) < angle_ && (angle + (partition_size / 2)) > angle_;
}

float UltrasonicDistance::getDistance()
{
    float total = 0;
    for (int i = 0; i < avglen; i++)
    {
        total += array[i];
    }
    if (total < avglen)
    {
        return 0;
    }
    return total / (float)avglen;
}

int UltrasonicDistance::getAngle()
{
    return angle;
}