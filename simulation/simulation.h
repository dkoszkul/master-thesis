#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <list>
#include <map>
#include <QObject>
#include <QString>
#include <QFile>

#include "sensors/object.h"
#include "sensors/emitter.h"
#include "sensors/receiver.h"
#include "sensors/obstacle.h"
#include "simulation/signal.h"

#include "algorithm/algorithm.h"

#include "simulation/point.h"

using namespace std;

class Simulation : public QObject
{
    Q_OBJECT
private:
    Object *emitter;
    std::list<Receiver *> receivers;
    std::list<Obstacle *> obstacles;

    Algorithm *algorithm;

    QwtPlot *plot;
    std::map<int,std::vector<double>> deltaTByReceiverNumber;
    std::map<int,std::vector<double>> timeByReceiverNumber;
    std::list<Point *> algorithmResultsToPlot;

    bool allReceiversHaveSignals(bool* signalTable, int size);
    void detectZeroCrossings();
    bool arePatternTheSame(int* pattern, int* previousPattern, int size);
    void plotPhaseShift();

public:
    explicit Simulation(QObject *parent = 0);
    ~Simulation();

    void setEmitter(Object* emitter);
    void setReceivers(const std::list<Receiver*> &receivers);
    void setObstacles(const std::list<Obstacle*> &obstacles);
    std::map<int, std::vector<double> > getDeltaTByReceiverNumber() const;
    std::map<int, std::vector<double> > getTimeByReceiverNumber() const;
    void setPlot(QwtPlot *value);
    std::list<Obstacle *> &getObstacles();
    std::list<Receiver *> &getReceivers();
    Object *getEmitter() const;

public slots:
    void simulate();
};

#endif // SIMULATION_H
