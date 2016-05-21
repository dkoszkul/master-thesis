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

void Simulation::setReceivers(const std::vector<Receiver*> &receivers)
{
    this->receivers=receivers;
}

void Simulation::setObstacles(const std::vector<Obstacle*> &obstacles)
{
    this->obstacles=obstacles;
}

void Simulation::simulate()
{
    deltaTByReceiverNumber.clear();
    timeByReceiverNumber.clear();
    algorithmResultsToPlot.clear();

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

void Simulation::detectZeroCrossings()
{
    // [1] method showSignals() computes signal values in every time tick
    for(auto r=receivers.begin();r!=receivers.end();r++){
        (*r)->getSignal()->showSignals(this->time.from,this->time.to,this->time.step);
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
double  previousZeroCrossingUs = 0;
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

                        //std::cout<<(*r)->getSignal()->getSignalX().at(uS) - previousZeroCrossingUs<<std::endl;
                        double actualFrequency = (*r)->getSignal()->getSignalX().at(uS) - previousZeroCrossingUs;
                        runTheAlgorithm(referenceReceiverZeroCrossTime, actualFrequency);
                        previousZeroCrossingUs = (*r)->getSignal()->getSignalX().at(uS);

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
                timeByReceiverNumber[receiverNumber].push_back((*r)->getSignal()->getSignalX().at(uS));
            }
            receiverNumber++;
        }
    }

    QwtPlotCurve* signalPlot = new QwtPlotCurve( "" );
    signalPlot->setRenderHint( QwtPlotItem::RenderAntialiased );
    signalPlot->setLegendAttribute( QwtPlotCurve::LegendShowLine, true );
    signalPlot->setPen( Qt::blue );
    signalPlot->attach( resultPlot );
    std::vector<double> x;
    std::vector<double> y;
    signalPlot->setSamples(x.data(),y.data(),x.size());
     plot->replot();
    for(auto r=algorithmResultsToPlot.begin();r!=algorithmResultsToPlot.end();r++){
        x.push_back((*r)->getX());
        y.push_back((*r)->getY());
    }
    signalPlot->setSamples(x.data(),y.data(),x.size());

    plotPhaseShift();
    plot->replot();

    /**********************************/
    /* CLEAR ALL POINTERS             */
    /**********************************/
    delete[] hasSignal;
    delete[] previousProbes;
}

void Simulation::runTheAlgorithm(int referenceReceiverZeroCrossTime, double signalFrequency){

    double Receiver1deltaT = algorithm->correctTime(deltaTByReceiverNumber[1].back(),signalFrequency); //This is unnecessary in sumulation but in future it may be useful
    double Receiver2deltaT = algorithm->correctTime(deltaTByReceiverNumber[2].back(),signalFrequency); //This is unnecessary in sumulation but in future it may be useful
    int r0R1DistanceInMm = receivers.at(1)->getPositionY() - receivers.at(0)->getPositionY();
    int r1R2DistanceInMm = receivers.at(2)->getPositionY() - receivers.at(1)->getPositionY();

//uncomment this line to get readings to doctor's code    //  std::cout<<"0 "<<referenceReceiverZeroCrossTime<<" "<<Receiver1deltaT<<" "<<Receiver2deltaT<<" 0 "<<signalFrequency<<std::endl;
    AlgorithmResult aResult = algorithm->findAngleByKValuesFor(r0R1DistanceInMm,r1R2DistanceInMm,Receiver1deltaT, Receiver2deltaT, signalFrequency, this->epsilon);
    Point *point = new Point(referenceReceiverZeroCrossTime,aResult.angle);
    algorithmResultsToPlot.push_back(point);

    QwtPlotMarker* m = new QwtPlotMarker();
    m->setSymbol(new QwtSymbol( QwtSymbol::Ellipse, Qt::red, Qt::NoPen, QSize( 10, 10 ) ) );
    m->setValue( QPointF( point->getX(), point->getY() ) );
    m->attach( resultPlot );
    resultPlot->replot();

   // std::cout<<aResult.status<<" angle: "<<aResult.angle<<std::endl;
   // std::cout<<"POINT: ("<<point->getX()<<","<<point->getY()<<")"<<std::endl;
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

std::vector<Obstacle *> &Simulation::getObstacles()
{
    return this->obstacles;
}

std::vector<Receiver *> &Simulation::getReceivers()
{
    return this->receivers;
}


Object *Simulation::getEmitter() const
{
    return emitter;
}

void Simulation::setResultPlot(QwtPlot *value)
{
    resultPlot = value;
}

void Simulation::setEpsilon(double value)
{
    epsilon = value;
}

void Simulation::setTime(const SimulationTime &value)
{
    time = value;
}
