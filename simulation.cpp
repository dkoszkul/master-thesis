#include "simulation.h"

Simulation::Simulation(QObject *parent) : QObject(parent)
{

}

void Simulation::setEmitter(Object *emitter)
{
    this->emitter=emitter;
}

void Simulation::setReceivers(std::list<Object *> receivers)
{
    this->receivers=receivers;
}

void Simulation::setObstacles(std::list<Object *> obstacles)
{
    this->obstacles=obstacles;
}

void Simulation::simulate()
{
    std::cout<<"simulate"<<std::endl;
}


