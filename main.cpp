#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QGridLayout>
#include <iostream>

#include "object.h"
#include "emitter.h"
#include "receiver.h"
#include "obstacle.h"

#include "paintwidget.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   /* QPainter painter(this);

    painter.setPen(Qt::darkGreen);
    painter.drawRect(1, 2, 6, 4);*/

    MainWindow w;
    QGridLayout *grid =  w.findChild<QGridLayout*>("gridLayout");
    Q_ASSERT(grid);
    PaintWidget* widget = new PaintWidget();


    list<Obstacle*> obstacles;
    obstacles.push_back(new Obstacle(10,20,0));
    obstacles.push_back(new Obstacle(10,10,0));
    obstacles.push_back(new Obstacle(5,7,0));

    list<Receiver*> sensors;
    sensors.push_back(new Receiver(0,0,0));
    sensors.push_back(new Receiver(0,30,0));
    sensors.push_back(new Receiver(0,60,0));

    Emitter* emitter = new Emitter(-5,10,0);


    cout<<"emitter X "<<emitter->getPositionX()<<endl;
    emitter->emitSignal();

    list<Receiver*>::iterator it;
    for(it=sensors.begin();it!=sensors.end();it++){
        cout<<(*it)->getPositionY()<<endl;
        widget->sensors.push_back((*it));
    }

    grid->addWidget(widget, 1, 1);
    w.setLayout(grid);
    w.show();

    return a.exec();
}
