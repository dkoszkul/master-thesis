#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <list>

#include <QObject>

#include "constants.h"
#include "object.h"

class Simulation : public QObject
{
    Q_OBJECT
private:
    Object* emitter;
    std::list<Object*> receivers;
    std::list<Object*> obstacles;

public:
    explicit Simulation(QObject *parent = 0);
    void setEmitter(Object* emitter);
    void setReceivers(std::list<Object*> receivers);
    void setObstacles(std::list<Object*> obstacles);

public slots:
    void simulate();
};

#endif // SIMULATION_H
