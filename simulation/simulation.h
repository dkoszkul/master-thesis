#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <list>
#include <map>
#include <vector>
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

#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

struct SimulationTime {
    double from;
    double to;
    double step;
};

using namespace std;

class Simulation : public QObject
{
    Q_OBJECT
private:
    Object *emitter;
    std::vector<Receiver *> receivers;
    std::vector<Obstacle *> obstacles;

    Algorithm *algorithm;

    QwtPlot *plot;
    QwtPlot *resultPlot;

    std::map<int,std::vector<double>> deltaTByReceiverNumber;
    std::map<int,std::vector<double>> timeByReceiverNumber;
    std::vector<Point *> algorithmResultsToPlot;

    double epsilon;
    SimulationTime time;

    bool allReceiversHaveSignals(bool* signalTable, int size);
    void detectZeroCrossings();
    void plotPhaseShift();

    void runTheAlgorithm(int referenceReceiverZeroCrossTime, double signalFrequency);

public:
    explicit Simulation(QObject *parent = 0);
    ~Simulation();

    void setEmitter(Object* emitter);
    void setReceivers(const std::vector<Receiver*> &receivers);
    void setObstacles(const std::vector<Obstacle*> &obstacles);
    std::map<int, std::vector<double> > getDeltaTByReceiverNumber() const;
    std::map<int, std::vector<double> > getTimeByReceiverNumber() const;
    void setPlot(QwtPlot *value);
    std::vector<Obstacle *> &getObstacles();
    std::vector<Receiver *> &getReceivers();
    Object *getEmitter() const;

    void setResultPlot(QwtPlot *value);
    void setEpsilon(double value);
    void setTime(const SimulationTime &value);

    void simulate();
    std::vector<Point *> getAlgorithmResultsToPlot() const;
};

#endif // SIMULATION_H
