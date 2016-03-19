#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"

class Obstacle : public Object{
public:
    Obstacle();
    Obstacle(int positionX,int positionY,double angle);
};

#endif // OBSTACLE_H
