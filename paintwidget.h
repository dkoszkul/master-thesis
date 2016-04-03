#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H
#include <QtGui>
#include <QWidget>

#include "sensors/object.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    void addEmitter(Object* emitter);
    void addReceiver(Object* receiver);
    void addObstacle(Object* obstacle);

private:
    void drawReceivers(QPainter *p);
    void drawEmitter(QPainter *p);
    void drawObstacles(QPainter *p);
    std::list<Object*> sensors;
    std::list<Object*> obstacles;
    Object* emitter;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // PAINTWIDGET_H
