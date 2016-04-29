#include "simulation.h"

Simulation::Simulation(QObject *parent) : QObject(parent)
{
    algorithm = new Algorithm;
}

Simulation::~Simulation()
{
    delete algorithm;
}

void Simulation::setEmitter(Object *emitter)
{
    this->emitter=emitter;
}

void Simulation::setReceivers(const std::list<Receiver*> &receivers)
{
    this->receivers=receivers;
}

void Simulation::setObstacles(const std::list<Obstacle*> &obstacles)
{
    this->obstacles=obstacles;
}

void Simulation::simulate()
{
    deltaTByReceiverNumber.clear();
    timeByReceiverNumber.clear();

    std::cout<<"simulate"<<std::endl;
    std::cout<<"In the simulation we have "<<this->obstacles.size()<<" obstacles and "<<this->receivers.size()<<" receivers"<<std::endl;

    for(auto r=receivers.begin();r!=receivers.end();r++){
        (*r)->resetReceiver();
        for(auto o=obstacles.begin();o!=obstacles.end();o++){

            double emitterToObstacleDistance = emitter->getDistance((*o));
            double distanceInMilimeters = emitterToObstacleDistance + (*r)->getDistance((*o));
            double distanceinMeters = distanceInMilimeters/1000;
            double timeInMicroseconds = (distanceinMeters/SPEED_OF_SOUND)*pow(10,6);
            (*r)->addTime(timeInMicroseconds);
            std::cout<<"Distance between emitter to receiver ["<<(*r)->getPositionX()<<","<<(*r)->getPositionY()<<"] through obstacle ["<<(*o)->getPositionX()<<","<<(*o)->getPositionY();
            std::cout<<"] is equal to "<<distanceInMilimeters<<"[mm], TOF = "<<timeInMicroseconds<<"[us]"<<std::endl;
        }
        std::cout<<"-------------------"<<std::endl;
    }

    Receiver * referenceReceiver = receivers.front();
    list<double> times = referenceReceiver->getTimes();
    for(auto t=times.begin();t!=times.end();t++){
        double time = (*t);
        for(auto r=receivers.begin();r!=receivers.end();r++){
            list<double> rTimes = (*r)->getTimes();
            double maxDistanceBetweenSensorsInUs = ((referenceReceiver->getDistance((*r))/1000)/SPEED_OF_SOUND)*pow(10,6);
            for(auto tt=rTimes.begin();tt!=rTimes.end();tt++){
                double timeDelay =(*tt) - time;
                if(abs(timeDelay) <= maxDistanceBetweenSensorsInUs){
                    (*r)->addDelay(timeDelay);
                    tt = rTimes.erase(tt);
                    (*r)->setTimes(rTimes);
                    break;
                }
            }
        }
        t = times.erase(t);
    }

    for(auto r=receivers.begin();r!=receivers.end();r++){
        list<double> times = (*r)->getCopyTimes();
        std::cout<<"Receiver "<<(*r)->getReceiverNumber()<<" ";
        for(auto t=times.begin();t!=times.end();t++){
            std::cout<<(*t)<<" ";
        }
        std::cout<<std::endl;
    }

    for(auto r=receivers.begin();r!=receivers.end();r++){
        (*r)->setTimes((*r)->getCopyTimes()); //this step is important after computing the delays to recover the TOF
        cout<<(*r)->getTimes().size();
        list<double> delays = (*r)->getDelays();
        for(auto t=delays.begin();t!=delays.end();t++){
            double v =(*t);
            std::cout<<" "<<v;
        }
        cout<<endl;
    }

    /*************************************************/
    /* DRAWING PLOTS BASED ON TOF FROM EACH RECEIVER */
    /*************************************************/

    double minTime = 999999;
    for(auto r=receivers.begin();r!=receivers.end();r++){
        list<double> rTimes = (*r)->getTimes();
        for(auto tt=rTimes.begin();tt!=rTimes.end();tt++){
            if((*tt)<minTime){
                minTime = (*tt);
            }
        }
    }
    for(auto r=receivers.begin();r!=receivers.end();r++){
        list<double> rTimes = (*r)->getTimes();
        for(auto tt=rTimes.begin();tt!=rTimes.end();tt++){
            (*r)->getSignal()->addTime((*tt)- minTime);
        }
    }
    detectZeroCrossings();
}

bool Simulation::allReceiversHaveSignals(bool *signalTable, int size)
{
    for(int i=0;i<size;i++){
        if(signalTable[i] == false){
            return false;
        }
    }

    return true;
}

void Simulation::plotPhaseShift()
{
    for(unsigned int i=0;i<receivers.size();i++){
        QwtPlotCurve* signalPlot = new QwtPlotCurve( "delta T" );
        signalPlot->setRenderHint( QwtPlotItem::RenderAntialiased );
        signalPlot->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
        signalPlot->setPen( Qt::red );
        signalPlot->attach( plot );
        signalPlot->setSamples(timeByReceiverNumber[i].data(),deltaTByReceiverNumber[i].data(),deltaTByReceiverNumber[i].size());
    }
}

void Simulation::detectZeroCrossings()
{
    // [1] method showSignals() computes signal values in every time tick
    for(auto r=receivers.begin();r!=receivers.end();r++){
        (*r)->getSignal()->showSignals();
    }

    // [2] initialize necessary variables
    bool* hasSignal = new bool[receivers.size()];
    double* previousProbes = new double[receivers.size()];
    for(unsigned int i=0;i<receivers.size();i++){
        hasSignal[i]=false;
        previousProbes[i]=0;
    }
    int receiverNumber;
    int signalProbesSize = receivers.front()->getSignal()->getSignalY().size();
    bool areReceiversHaveSignals = false;

    int referenceReceiver = 0;
    int referenceReceiverZeroCrossTime = 0;
    bool doMeasurement = false;

    // [3] compute zero crossings
    for(int uS=0;uS<signalProbesSize;uS++){
        // iteration over all signals and find phase delays
        receiverNumber=0;
        for(auto r=receivers.begin();r!=receivers.end();r++){
            double actualProbeValue = (*r)->getSignal()->getSignalY().at(uS);
            if(!areReceiversHaveSignals && actualProbeValue!=0){ // [A]
                hasSignal[receiverNumber]=true;
            }

            if(allReceiversHaveSignals(hasSignal,receivers.size())){
                // [.] all receivers receive signal
                if(!areReceiversHaveSignals){ //dezactivate [A]
                    areReceiversHaveSignals = true;
                }
                // [.] always detect zero-crossing in signal
                if(previousProbes[receiverNumber] < 0 && actualProbeValue > 0){
                    // [.] raise doMeasurement flag if zero-crossing belongs to the reference signal
                    if((*r)->getReceiverNumber() == referenceReceiver){
                        doMeasurement = true;
                        referenceReceiverZeroCrossTime = (*r)->getSignal()->getSignalX().at(uS);
                    }
                    // [.] measure the time if doMeasurement flag is raised
                    if(doMeasurement){
                        deltaTByReceiverNumber[receiverNumber].push_back((*r)->getSignal()->getSignalX().at(uS)-referenceReceiverZeroCrossTime);
                        timeByReceiverNumber[receiverNumber].push_back((*r)->getSignal()->getSignalX().at(uS));
                    }
                }

                previousProbes[receiverNumber] = actualProbeValue;


            }else{
                deltaTByReceiverNumber[receiverNumber].push_back(0);
                timeByReceiverNumber[receiverNumber].push_back(uS);
            }
            receiverNumber++;
        }
    }

    plotPhaseShift();
    plot->replot();

    /**********************************/
    /* CLEAR ALL POINTERS             */
    /**********************************/
    delete[] hasSignal;
    delete[] previousProbes;
}

bool Simulation::arePatternTheSame(int *pattern, int *previousPattern, int size)
{
    for(int index=0;index<size;index++){
        if(pattern[index] != previousPattern[index]){
            return false;
        }
    }
    return true;
}

std::map<int, std::vector<double> > Simulation::getDeltaTByReceiverNumber() const
{
    return deltaTByReceiverNumber;
}

std::map<int, std::vector<double> > Simulation::getTimeByReceiverNumber() const
{
    return timeByReceiverNumber;
}

void Simulation::setPlot(QwtPlot *value)
{
    plot = value;
}

std::list<Obstacle *> &Simulation::getObstacles()
{
    return this->obstacles;
}

std::list<Receiver *> &Simulation::getReceivers()
{
    return this->receivers;
}


Object *Simulation::getEmitter() const
{
    return emitter;
}
