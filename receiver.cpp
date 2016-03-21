#include "receiver.h"


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

void Receiver::addTime(int time)
{
    times.push_back(time);
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
