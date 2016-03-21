#include "receiver.h"


list<double> Receiver::getTimes() const
{
    return times;
}

void Receiver::setTimes(const list<double> &value)
{
    times = value;
}

list<double> Receiver::getCopyTimes() const
{
    return copyTimes;
}

Receiver::Receiver()
{
    
}

Receiver::Receiver(int positionX, int positionY, double angle) : Object(positionX,positionY,angle){

}

Receiver::~Receiver()
{

}

void Receiver::resetReceiver()
{
    times.clear();
    delays.clear();
}

void Receiver::addTime(double time)
{
    times.push_back(time);
    copyTimes.push_back(time);
}

void Receiver::addDelay(double delay)
{
    delays.push_back(delay);
}

void Receiver::countDelays()
{
    times.sort();
    double firstTime = times.front();
    for(list<double>::iterator t=times.begin();t!=times.end();t++){
        delays.push_back((*t)-firstTime);
    }
}

list<double> Receiver::getDelays() const
{
    return delays;
}
