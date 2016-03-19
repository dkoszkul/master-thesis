#include "sensor.h"

Sensor::Sensor()
{
    this->positionX=0;
    this->positionY=0;
    this->angle=0;
}

Sensor::Sensor(int positionX, int positionY, double angle){
    this->positionX=positionX;
    this->positionY=positionY;
    this->angle=angle;
}

int Sensor::getPositionX(){
    return this->positionX;
}

int Sensor::getPositionY(){
    return this->positionY;
}

double Sensor::getAngle(){
    return this->angle;
}
