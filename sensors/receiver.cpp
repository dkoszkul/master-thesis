#include "receiver.h"

int Receiver::getReceiverNumber() const
{
    return receiverNumber;
}

void Receiver::setReceiverNumber(int value)
{
    receiverNumber = value;
}

Receiver::Receiver()
{
    signal = new Signal();
}

Receiver::Receiver(int receiverNumber, int positionX, int positionY, double angle) : Object(positionX,positionY,angle){
    this->receiverNumber = receiverNumber;
    signal = new Signal();
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


Signal *Receiver::getSignal() const
{
    return signal;
}

void Receiver::setSignal(Signal *value)
{
    signal = value;
}
