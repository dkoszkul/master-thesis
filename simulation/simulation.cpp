#include "simulation.h"

Simulation::Simulation(QObject *parent) : QObject(parent)
{

}

Simulation::~Simulation()
{

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
    std::cout<<"simulate"<<std::endl;
    std::cout<<"In the simulation we have "<<this->obstacles.size()<<" obstacles and "<<this->receivers.size()<<" receivers"<<std::endl;

    for(auto r=receivers.begin();r!=receivers.end();r++){
        (*r)->resetReceiver();
        for(auto o=obstacles.begin();o!=obstacles.end();o++){

            double emitterToObstacleDistance = emitter->getDistance((*o));
            double distance = emitterToObstacleDistance + (*r)->getDistance((*o));
            double time = ((distance/100)/SPEED_OF_SOUND)*pow(10,6);
            (*r)->addTime(time);
            std::cout<<"Distance between emitter to receiver ["<<(*r)->getPositionX()<<","<<(*r)->getPositionY()<<"] through obstacle ["<<(*o)->getPositionX()<<","<<(*o)->getPositionY();
            std::cout<<"] is equal to "<<distance<<", TOF = "<<time<<"[us]"<<std::endl;
        }
        std::cout<<"-------------------"<<std::endl;
    }

    Receiver * referenceReceiver = receivers.front();
    list<double> times = referenceReceiver->getTimes();
    for(list<double>::iterator t=times.begin();t!=times.end();t++){
        double time = (*t);
        for(list<Receiver*>::iterator r=receivers.begin();r!=receivers.end();r++){
            list<double> rTimes = (*r)->getTimes();
            double maxDistanceBetweenSensorsInUs = ((referenceReceiver->getDistance((*r))/100)/SPEED_OF_SOUND)*pow(10,6);
            for(list<double>::iterator tt=rTimes.begin();tt!=rTimes.end();tt++){
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
        (*r)->setTimes((*r)->getCopyTimes()); //this step is important after computing the delays to recover the TOF
        cout<<(*r)->getTimes().size();
        list<double> delays = (*r)->getDelays();
        for(list<double>::iterator t=delays.begin();t!=delays.end();t++){
            double v =(*t);
            std::cout<<" "<<v;
        }
    cout<<endl;
    }

}


