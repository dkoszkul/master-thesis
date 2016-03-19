#include "object.h"

Object::Object()
{
    this->positionX=0;
    this->positionY=0;
    this->angle=0;
}

Object::Object(int positionX, int positionY, double angle){
    this->positionX=positionX;
    this->positionY=positionY;
    this->angle=angle;
}

int Object::getPositionX(){
    return this->positionX;
}

int Object::getPositionY(){
    return this->positionY;
}

double Object::getAngle(){
    return this->angle;
}
