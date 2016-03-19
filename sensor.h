#ifndef SENSOR_H
#define SENSOR_H


class Sensor{
private:
    int positionX;
    int positionY;
    double angle;

public:
    int getPositionX();
    int getPositionY();
    double getAngle();

    Sensor();
    Sensor(int positionX,int positionY,double angle);
};

#endif // SENSOR_H
