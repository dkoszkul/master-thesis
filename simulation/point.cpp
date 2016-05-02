#include "point.h"

Point::Point()
{
    this->x=0.0;
    this->y=0.0;
}

Point::Point(double x, double y)
{
    this->x=x;
    this->y=y;
}

double Point::getY() const
{
    return y;
}

void Point::setY(double value)
{
    y = value;
}

double Point::getX() const
{
    return x;
}

void Point::setX(double value)
{
    x = value;
}

