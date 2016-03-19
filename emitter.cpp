#include "emitter.h"

Emitter::Emitter()
{

}

Emitter::Emitter(int positionX, int positionY, double angle) : Sensor(positionX,positionY,angle){

}

void Emitter::emitSignal(){
    std::cout<<"Emitting the signal"<<std::endl;
}
