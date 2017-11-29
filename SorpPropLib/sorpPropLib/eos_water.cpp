#include "eos_water.h"

eos_water::eos_water()
{
    //Saul, A., & Wagner, W. (1987). International equations for the saturation properties of ordinary water substance. Journal of Physical and Chemical Reference Data, 16(4), 893-901.

    //Tc = 647.14 K
    //pc = 22.064e6 Pa
}

double eos_water::psat(double tK)
{
    double a0 = -7.858230;
    double a1 = 1.839910;
    double a2 = -11.781100;
    double a3 = 22.670500;
    double a4 = -15.939300;
    double a5 = 1.775160;

    double tau = 1 - tK / 647;
    double P = 647 * exp((a0*tau + a1*pow(tau,1.5) + a2*pow(tau,3)
                          + a3 * pow(tau,3.5) + a4 * pow(tau,4)
                          + a5 * pow(tau,7.5))/(1-tau));
    return P;
}
