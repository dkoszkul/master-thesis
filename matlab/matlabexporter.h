#ifndef MATLABEXPORTER_H
#define MATLABEXPORTER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <vector>

#include "sensors/receiver.h"
#include "sensors/emitter.h"
#include "sensors/obstacle.h"
#include "simulation/simulation.h"

class MatlabExporter : public QObject
{
    Q_OBJECT
public:
    explicit MatlabExporter(QObject *parent = 0);

    void setObstacles(const std::list<Obstacle *> &value);
    void setSimulation(Simulation *value);
    void exportResults(QString *filename);

private:
    Simulation* simulation;

};

#endif // MATLABEXPORTER_H
