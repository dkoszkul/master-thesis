#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include<list>
#include<vector>
#include <QObject>
#include<cmath>
#include "constants.h"

class Signal : public QObject
{
    Q_OBJECT
public:
    explicit Signal(QObject *parent = 0);
    ~Signal();

    std::vector<double> generateSignal();
    void addTime(double time);


public slots:
    void showSignals();

private:

    double signalMin = 0;
    double signalMax = 1500;
    double signalStep = 1;
    std::list<double> times;

};

#endif // SIGNAL_H
