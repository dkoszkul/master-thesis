#ifndef POINT_H
#define POINT_H


class Point
{
private:
    double x;
    double y;

public:
    Point();
    Point(double x, double y);

    double getY() const;
    void setY(double value);

    double getX() const;
    void setX(double value);
};

#endif // POINT_H
