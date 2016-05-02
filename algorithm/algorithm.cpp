#include "algorithm.h"

/*std::ostream& operator << (std::ostream&  Strm, Type4KSearchResult Val){
    const char* Name[] = { "TK_good", "TK_ambiguous", "TK_none" };
    return Strm << Name[Val];
  }*/

Algorithm::Algorithm(QObject *parent) : QObject(parent)
{

}

AlgorithmResult Algorithm::findAngleByKValuesFor(double timeDelay1, double timeDelay2, double epsilon)
{
    std::cout<<std::endl<<"I have "<<timeDelay1<<" and "<<timeDelay2<<std::endl;
    AlgorithmResult result;

    int  K1_Idx, K2_Idx;
    int  K1_res = K_EMPTY_INDEX, K2_res = K_EMPTY_INDEX;
    double  AngleSin1, AngleSin2;

    for (K1_Idx = K_MIN; K1_Idx <= K_MAX; ++K1_Idx) {
        if (!CheckK_4_RecData(GAP_R0_R1_mm,timeDelay1,K1_Idx,AngleSin1)) continue;
        for  (K2_Idx = K_MIN; K2_Idx <= K_MAX; ++K2_Idx) {
          if (!CheckK_4_RecData(GAP_R0_R1_mm+GAP_R1_R2_mm,timeDelay2,K2_Idx,AngleSin2)) continue;
          if (fabs(AngleSin1-AngleSin2) >= epsilon) continue;
          if (K1_res != K_EMPTY_INDEX) {

            cout << ":)  Ambiguous solution found." << endl
             << "       1.  k1 = " << K1_res << "  k2 = " << K2_res << endl
             << "       2.  k1 = " << K1_Idx << "  k2 = " << K2_Idx << endl
             << endl;

            return result;
          }

          K1_res = K1_Idx;  K2_res = K2_Idx;
          //      cout << " ***   K1: " << K1_res << "    K2: " << K2_res << endl;
          result.angle = RAD2DEG(asin((AngleSin1+AngleSin2)/2));
        }
    }
    if(K1_res != K_EMPTY_INDEX){
        result.status = TK_good;
    }else{
        result.status = TK_none;
    }

    return result;
}

/*!
 * A value for angle determination is computed
 */
bool Algorithm::CheckK_4_RecData(double Gap_R_mm, double DTime_us,int Ki,double &AngleSin)
{
    double  Part_Upper =
            URS__ULTRASONIC_WAVE_SPEED_mmUS*DTime_us
            + Ki*URS__ULTRASONIC_WAVELENGTH__mm;
    double  Base = Gap_R_mm;  // It can be < 0.

    if (fabs(Part_Upper) > fabs(Base)) return false;
    AngleSin = Part_Upper/Base;
    return true;
}

double Algorithm::correctTime( double Delta_T_us, double T_period_us)
{
  if (Delta_T_us < 0) {
    while ((Delta_T_us += T_period_us) < 0) {}
    return Delta_T_us;
  }
  if (Delta_T_us > T_period_us) {
    while ((Delta_T_us -= T_period_us) > T_period_us) {}
  }
  return Delta_T_us;
}

