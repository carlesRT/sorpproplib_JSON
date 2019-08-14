#include "eqn_ac_floryhuggins.h"

double eqn_ac_floryhuggins::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_floryhuggins mpara(prms);
    double chi = mpara.w0/(8.314*tK)*(1+mpara.w1/tK);
    double phi2 = mpara.r*(1-xMass)/(xMass + mpara.r*xMass);

    double f = log(1-(1-1/mpara.r)*phi2)+(1-1/mpara.r)*phi2+chi+phi2;

    return exp(f);
}
