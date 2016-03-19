#include "mainwindow.h"
#include <QApplication>

#include <iostream>

#include "object.h"
#include "emitter.h"
#include "receiver.h"
#include "obstacle.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    list<Obstacle*> obstacles;
    obstacles.push_back(new Obstacle(10,20,0));
    obstacles.push_back(new Obstacle(10,10,0));
    obstacles.push_back(new Obstacle(5,7,0));

    list<Receiver*> sensors;
    sensors.push_back(new Receiver(0,0,0));
    sensors.push_back(new Receiver(0,10,0));
    sensors.push_back(new Receiver(0,20,0));

    Emitter* emitter = new Emitter(-5,10,0);


    cout<<"emitter X "<<emitter->getPositionX()<<endl;
    emitter->emitSignal();

    list<Receiver*>::iterator it;
    for(it=sensors.begin();it!=sensors.end();it++){
        cout<<(*it)->getPositionY()<<endl;
    }

    for(it=sensors.begin();it!=sensors.end();it++){
        delete *it;
    }

    return a.exec();
}
