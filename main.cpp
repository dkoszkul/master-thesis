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
    obstacles.push_back(new Obstacle(100,120,0));
    obstacles.push_back(new Obstacle(90,110,0));
    obstacles.push_back(new Obstacle(190,190,0));

    list<Receiver*> sensors;
    sensors.push_back(new Receiver(20,100,0));
    sensors.push_back(new Receiver(20,130,0));
    sensors.push_back(new Receiver(20,160,0));

    Emitter* emitter = new Emitter(0,125,0);

    for(list<Receiver*>::iterator it=sensors.begin();it!=sensors.end();it++){
        widget->addReceiver((*it));
    }

    widget->addEmitter(emitter);

    for(list<Obstacle*>::iterator it=obstacles.begin();it!=obstacles.end();it++){
        widget->addObstacle((*it));
    }


    grid->addWidget(widget, 1, 1);
    w.setLayout(grid);
    w.show();

    return a.exec();
}
