#ifndef RECEIVER_H
#define RECEIVER_H

#include "object.h"

class Receiver : public Object{
public:
    Receiver();
    Receiver(int positionX,int positionY,double angle);
};

#endif // RECEIVER_H
