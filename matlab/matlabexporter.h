#ifndef MATLABEXPORTER_H
#define MATLABEXPORTER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <iostream>
#include <sstream>
#include <list>
#include "sensors/receiver.h"
#include "sensors/emitter.h"
#include "sensors/obstacle.h"

class MatlabExporter : public QObject
{
    Q_OBJECT
public:
    explicit MatlabExporter(QObject *parent = 0);

    void setFilePath(const std::string &value);
    void setReceivers(const std::list<Receiver *> &value);
    void setEmitter(Emitter *value);
    void setObstacles(const std::list<Obstacle *> &value);

public slots:
    void exportResults();

private:
    std::string filePath;
    std::list<Receiver*> receivers;
    Emitter* emitter;
    std::list<Obstacle*> obstacles;

};

#endif // MATLABEXPORTER_H
