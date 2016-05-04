#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QWidget>

#include <cmath>
#include <iostream>
#include <sstream>

#include "constants.h"

enum Type4KSearchResult {  TK_good, TK_ambiguous, TK_none };
//std::ostream& operator << (std::ostream&  Strm, Type4KSearchResult Val);

struct AlgorithmResult {
    Type4KSearchResult status;
    double angle;
    double distance;
};

using namespace std;

class Algorithm : public QObject
{
    Q_OBJECT
public:
    explicit Algorithm(QObject *parent = 0);

    AlgorithmResult findAngleByKValuesFor(int r0R1DistanceInMm, int r1R2DistanceInMm, double timeDelay1, double timeDelay2, double epsilon=0.3);
    double correctTime( double Delta_T_us, double T_period_us);

    void setMainWindow(QWidget *value);

private:
    QWidget *mainWindow;

    int *distanceBetweenReceiversInMm; //not used yet

    bool CheckK_4_RecData(double Gap_R_mm, double DTime_us,int Ki,double &AngleSin);


signals:

public slots:
    void handleRealResults();

};

#endif // ALGORITHM_H
