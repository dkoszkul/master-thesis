#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PI 3.14159
#define AMP_NOM 1.0
#define AMP_DEN 9000.0
#define AMP AMP_NOM/AMP_DEN
#define TAU 130
#define POW 2

#define SPEED_OF_SOUND 343.2  // [m/s]

#define K_EMPTY_INDEX   200
#define URS__SIG_FREQ_Hz    40000
#define URS__ULTRASONIC_WAVE_SPEED__mmS    343000
#define URS__ULTRASONIC_WAVE_SPEED_mmUS    0.343
#define URS__ULTRASONIC_WAVELENGTH__mm  (static_cast<double>(URS__ULTRASONIC_WAVE_SPEED__mmS)/URS__SIG_FREQ_Hz)
#define DEG2RAD( x )  ((x)*M_PI/180)
#define RAD2DEG( x )  ((x)*180/M_PI)
#define K_MIN   -4
#define K_MAX    4

#endif // CONSTANTS_H
