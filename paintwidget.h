#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H
#include <QtGui>
#include <QWidget>

#include "object.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    void addEmitter(Object& emitter);
    void addReceiver(Object& receiver);
    void drawElipsis(QPainter *p);
    std::list<Object*> sensors;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // PAINTWIDGET_H
