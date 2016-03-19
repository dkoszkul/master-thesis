#ifndef OBJECT_H
#define OBJECT_H


class Object{
private:
    int positionX;
    int positionY;
    double angle;

public:
    int getPositionX();
    int getPositionY();
    double getAngle();

    Object();
    Object(int positionX,int positionY,double angle);
};

#endif // OBJECT_H
