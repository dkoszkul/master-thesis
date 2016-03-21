#ifndef RECEIVER_H
#define RECEIVER_H

#include "object.h"

using namespace std;

class Receiver : public Object{
private:
    list<double> times;
    list<double> copyTimes;

    list<double> delays;

public:
    Receiver();
    Receiver(int positionX,int positionY,double angle);
    ~Receiver();

    void resetReceiver();
    void addTime(double time);
    void addDelay(double delay);
    void countDelays();
    list<double> getDelays() const;
    list<double> getTimes() const;
    void setTimes(const list<double> &value);
    list<double> getCopyTimes() const;
};

#endif // RECEIVER_H
