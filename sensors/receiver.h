#ifndef RECEIVER_H
#define RECEIVER_H

#include "object.h"
#include "simulation/signal.h"

using namespace std;

class Receiver final : public Object{
private:
    list<double> times;
    list<double> copyTimes;
    list<double> delays;

    Signal* signal;

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

    Signal *getSignal() const;
    void setSignal(Signal *value);
};

#endif // RECEIVER_H
