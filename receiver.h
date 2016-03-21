#ifndef RECEIVER_H
#define RECEIVER_H

#include "object.h"

using namespace std;

class Receiver : public Object{
private:
    list<double> times;
    list<double> delays;

public:
    Receiver();
    Receiver(int positionX,int positionY,double angle);

    void resetReceiver();
    void addTime(int time);
    void countDelays();
    list<double> getDelays() const;
};

#endif // RECEIVER_H
