#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <list>
#include <map>
#include <QObject>

#include "sensors/object.h"
#include "sensors/emitter.h"
#include "sensors/receiver.h"
#include "sensors/obstacle.h"
#include "simulation/signal.h"

using namespace std;

class Simulation : public QObject
{
    Q_OBJECT
private:
    Object* emitter;
    std::list<Receiver*> receivers;
    std::list<Obstacle*> obstacles;

    bool allReceiversHaveSignals(bool* signalTable, int size);
    void detectZeroCrossings();
    bool arePatternTheSame(int* pattern, int* previousPattern, int size);

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
