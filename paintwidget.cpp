#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{

}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
    drawReceivers(&painter);
    drawEmitter(&painter);
    drawObstacles(&painter);

}


void PaintWidget::addEmitter(Object* emitter){
    this->emitter = emitter;
}

void PaintWidget::addReceiver(Object* receiver){
    this->sensors.push_back(receiver);
}

void PaintWidget::addObstacle(Object* obstacle){
    this->obstacles.push_back(obstacle);
}

void PaintWidget::drawReceivers(QPainter *p){
    std::list<Object*>::iterator it;
    for(it=sensors.begin();it!=sensors.end();it++){
        p->drawEllipse((*it)->getPositionX(), (*it)->getPositionY(), 1, 1);
    }

}

void PaintWidget::drawEmitter(QPainter *p){
    if(emitter != NULL){
        p->setBrush(QBrush(Qt::black));
        p->drawEllipse(emitter->getPositionX(), emitter->getPositionY(), 1, 1);
    }

}

void PaintWidget::drawObstacles(QPainter *p){
    std::list<Object*>::iterator it;
    for(it=obstacles.begin();it!=obstacles.end();it++){
        p->setBrush(QBrush(Qt::red));
        p->drawEllipse((*it)->getPositionX(), (*it)->getPositionY(), 1, 1);
    }

}
