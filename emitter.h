#ifndef EMITTER_H
#define EMITTER_H

#include <iostream>
#include "object.h"

class Emitter : public Object{
public:
    Emitter();
    Emitter(int positionX,int positionY,double angle);
    void emitSignal();
};

#endif // EMITTER_H
