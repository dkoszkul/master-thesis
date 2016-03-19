#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "sensor.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    list<Sensor*> sensors;
    sensors.push_back(new Sensor(0,0,0));
    sensors.push_back(new Sensor(0,10,0));
    sensors.push_back(new Sensor(0,20,0));

    list<Sensor*>::iterator it;
    for(it=sensors.begin();it!=sensors.end();it++){
        cout<<(*it)->getPositionY()<<endl;
    }

    for(it=sensors.begin();it!=sensors.end();it++){
        delete *it;
    }

    return a.exec();
}
