#ifndef EMITTER_H
#define EMITTER_H

#include <iostream>
#include "sensor.h"

class Emitter : public Sensor{
public:
    Emitter();
    Emitter(int positionX,int positionY,double angle);
    void emitSignal();
};

#endif // EMITTER_H
