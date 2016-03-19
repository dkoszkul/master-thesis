#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{

}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
    drawElipsis(&painter);
    /* painter.drawEllipse(50, 0, 10, 10);
     painter.drawEllipse(50, 50, 10, 10);
     painter.drawEllipse(50, 100, 10, 10);*/

    // painter.drawEllipse(30, 40, 10, 30);

}


void PaintWidget::addEmitter(Object &emitter){

}

void PaintWidget::addReceiver(Object &receiver){

}

void PaintWidget::drawElipsis(QPainter *p){

    std::list<Object*>::iterator it;
    for(it=sensors.begin();it!=sensors.end();it++){
        //cout<<(*it)->getPositionY()<<endl;
        p->drawEllipse((*it)->getPositionX(), (*it)->getPositionY(), 10, 10);
    }

}
