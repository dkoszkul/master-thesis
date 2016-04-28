#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>

struct AlgorithmResult {
    int status;
    double angle;
    double distance;
};

class algorithm : public QObject
{
    Q_OBJECT
public:
    explicit algorithm(QObject *parent = 0);

    AlgorithmResult compute(double timeDelay1, double timeDelay2);


signals:

public slots:

};

#endif // ALGORITHM_H
