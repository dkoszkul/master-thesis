#include "signal.h"

#include <QString>
#include <QFile>

Signal::Signal(QObject *parent) : QObject(parent)
{
    plot = new QwtPlot();
    plot->setAxisScale( plot->xBottom, 0.0, 1500.0 );
    plot->setAxisScale( plot->yLeft, -1.8, 1.8 );

    signalPlot = new QwtPlotCurve( "signal" );
    signalPlot->setRenderHint( QwtPlotItem::RenderAntialiased );
    signalPlot->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    signalPlot->setPen( Qt::red );
    signalPlot->attach( plot );
}

Signal::~Signal()
{

}

void Signal::generateSignal()
{
    signalProbes.clear();
    std::vector<double> timePlot;
    double t = signalMin;
    while(t<signalMax){
        double signalValue = computeSignalValueInTime(t);
        signalProbes.push_back(signalValue);
        timePlot.push_back(t);
        t+=signalStep;
    }

    signalPlot->setSamples(timePlot.data(),signalProbes.data(),signalProbes.size());
    plot->replot();

    QString filename = "Plot";
    filename.append(".txt");
    QFile file(filename);
    if (file.open(QIODevice::Append)) {
        QTextStream stream(&file);
        stream <<endl<< "time=[";
        for(auto it = timePlot.begin(); it != timePlot.end(); ++it) {
           stream<<" "<<(*it);
        }
        stream<<"];"<<endl;
        stream << "values=[";
        for(auto it = signalProbes.begin(); it != signalProbes.end(); ++it) {
           stream<<" "<<(*it);
        }
        stream<<"];"<<endl;
    }
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

std::vector<double> Signal::getSignalProbes() const
{
    return signalProbes;
}

QwtPlot *Signal::getPlot() const
{
    return plot;
}

QwtPlotCurve *Signal::getSignalPlot() const
{
    return signalPlot;
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
