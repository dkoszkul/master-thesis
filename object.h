#ifndef OBJECT_H
#define OBJECT_H

#include <cmath>
#include <list>

class Object{
private:
    int positionX;
    int positionY;
    double angle;

public:
    int getPositionX();
    int getPositionY();
    double getAngle();

    double getDistance(Object* object);

    Object();
    Object(int positionX,int positionY,double angle);
};

#endif // OBJECT_H
