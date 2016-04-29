#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>

#include <cmath>
#include <iostream>

#define K_EMPTY_INDEX   200
#define URS__SIG_FREQ_Hz    40000
#define URS__ULTRASONIC_WAVE_SPEED__mmS    343000
#define URS__ULTRASONIC_WAVE_SPEED_mmUS    0.343
#define URS__ULTRASONIC_WAVELENGTH__mm  (static_cast<double>(URS__ULTRASONIC_WAVE_SPEED__mmS)/URS__SIG_FREQ_Hz)
#define DEG2RAD( x )  ((x)*M_PI/180)
#define RAD2DEG( x )  ((x)*180/M_PI)
#define K_MIN   -4
#define K_MAX    4
/*!
 * \brief A gap between a receiver R0 and R1
 *
 * A gap between a receiver R0 and R1. It is expressed in millimeters.
 */
#define GAP_R0_R1_mm    11
/*!
 * \brief A gap between a receiver R1 and R2
 *
 * A gap between a receiver R1 and R2. It is expressed in millimeters.
 */
#define GAP_R1_R2_mm    4

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

    AlgorithmResult findAngleByKValuesFor(double timeDelay1, double timeDelay2, double epsilon=0.7);

private:
    int *distanceBetweenReceiversInMm; //not used yet

    bool CheckK_4_RecData(double Gap_R_mm, double DTime_us,int Ki,double &AngleSin);


signals:

public slots:

};

#endif // ALGORITHM_H
