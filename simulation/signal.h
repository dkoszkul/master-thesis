#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <list>
#include <vector>
#include <QObject>
#include <QVBoxLayout>
#include <cmath>
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
    void clear();
    void showSignals(double from, double to, double step);

    void setSignalPlot(QwtPlotCurve *value);
    void setPlot(QwtPlot *value);

    std::vector<double> getSignalY() const;
    std::vector<double> getSignalX() const;

    QwtPlotCurve *getSignalPlot() const;
    QwtPlot *getPlot(double from, double to);
    double getSignalStep() const;

private:
    double signalMin;
    double signalMax;
    double signalStep;
    std::list<double> times;
    std::vector<double> signalY;
    std::vector<double> signalX;

    QwtPlot* plot;
    QwtPlotCurve *signalPlot;

    void generateSignal();
    double computeSignalValueInTime(double time);
};

#endif // SIGNAL_H
