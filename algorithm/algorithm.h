#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QWidget>
#include <QTabWidget>

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

#include "constants.h"
#include "simulation/point.h"

enum Type4KSearchResult {  TK_good, TK_ambiguous, TK_none };
//std::ostream& operator << (std::ostream&  Strm, Type4KSearchResult Val);

struct AlgorithmResult {
    Type4KSearchResult status;
    double angle;
    double distance;
};

struct RealMeasurement {
    int R0R1InMm;
    int R1R2InMm;
    std::vector<double> R0Times;
    std::vector<double> R1Times;
    std::vector<double> R2Times;
    std::vector<double> TWaves;
};

using namespace std;

class Algorithm : public QObject
{
    Q_OBJECT
public:
    explicit Algorithm(QObject *parent = 0);

    AlgorithmResult findAngleByKValuesFor(int r0R1DistanceInMm, int r1R2DistanceInMm, double timeDelay1, double timeDelay2,double frequency, double epsilon=0.3);
    double correctTime( double Delta_T_us, double T_period_us);

    void setMainWindow(QWidget *value);
    void setAlgorithmResultPlot(QwtPlot *value);
    void exportAlgorithmResultsToMatlabScript();

private:
    QWidget *mainWindow;
    RealMeasurement *realMeasurement;
    QwtPlot *algorithmResultPlot;

    std::vector<Point *> resultPoints;

    bool CheckK_4_RecData(double Gap_R_mm, double DTime_us,int Ki,double frequency,double &AngleSin);


signals:

public slots:
    void handleRealResults();

};

#endif // ALGORITHM_H
