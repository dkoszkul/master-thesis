#include "signal.h"

Signal::Signal(QObject *parent) : QObject(parent)
{
    times.push_back(20);
}

Signal::~Signal()
{

}

std::vector<double> Signal::generateSignal()
{
    std::vector<double> result;

    for(double time : times){
        std::cout<<time<<std::endl;

        double t = signalMin;
        while(t<signalMax){
            double signalValue = AMP * pow(t,POW)* exp(-t/TAU)*sin(2*PI*t/25);
            std::cout<<"Time "<<t<<" value "<<signalValue<<std::endl;
            result.push_back(signalValue);
            t+=signalStep;
        }
    }

   // S(t)= t <= 0 ? 0 : Amp * t**Power * exp(-t/Tau) * sin(2*PI*t/25);

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
