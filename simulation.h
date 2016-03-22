#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <list>

#include <QObject>

#include "constants.h"
#include "object.h"
#include "emitter.h"
#include "receiver.h"
#include "obstacle.h"

using namespace std;

class Simulation : public QObject
{
    Q_OBJECT
private:
    Object* emitter;
    std::list<Receiver*> receivers;
    std::list<Obstacle*> obstacles;

public:
    explicit Simulation(QObject *parent = 0);
    ~Simulation();
    void setEmitter(Object* emitter);
    void setReceivers(const std::list<Receiver*> &receivers);
    void setObstacles(const std::list<Obstacle*> &obstacles);

public slots:
    void simulate();
};

#endif // SIMULATION_H