#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include<list>
#include<vector>
#include <QObject>
#include<cmath>
#include "constants.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class Signal : public QObject
{
    Q_OBJECT
public:
    explicit Signal(QObject *parent = 0);
    ~Signal();


    void addTime(double time);

    void setSignalPlot(QwtPlotCurve *value);
    void setPlot(QwtPlot *value);
    void clear();

    QwtPlot *getPlot() const;

public slots:
    void showSignals();

private:
    double signalMin = 0;
    double signalMax = 1500;
    double signalStep = 1;
    std::list<double> times;

    QwtPlot* plot;
    QwtPlotCurve *signalPlot;

    void generateSignal();
    double computeSignalValueInTime(double time);
};

#endif // SIGNAL_H
