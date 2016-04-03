#include "signal.h"

Signal::Signal(QObject *parent) : QObject(parent)
{
    times.push_back(0);
    times.push_back(205);
    times.push_back(319);
}

Signal::~Signal()
{

}

void Signal::generateSignal()
{
    std::vector<double> result;
    std::vector<double> timePlot;
    double t = signalMin;
    while(t<signalMax){

        double signalValue = computeSignalValueInTime(t);
        //std::cout<<"Time "<<t<<" value "<<signalValue<<std::endl;
        result.push_back(signalValue);
        timePlot.push_back(t);
        t+=signalStep;
    }


    signalPlot->setSamples(timePlot.data(),result.data(),result.size());
    plot->replot();
}

double Signal::computeSignalValueInTime(double actualTime){
    double result = 0.0;
    for(double t : times){
        double time = actualTime-t;
        result+= time<0 ? 0 : AMP * pow(time,POW)* exp(-time/TAU)*sin(2*PI*time/25);
    }
    return result;
}

void Signal::addTime(double time)
{
    times.push_back(time);
}

void Signal::showSignals()
{
    generateSignal();
}

void Signal::setPlot(QwtPlot *value)
{
    plot = value;
}

void Signal::clear()
{
    times.clear();
}

void Signal::setSignalPlot(QwtPlotCurve *value)
{
    signalPlot = value;
}
