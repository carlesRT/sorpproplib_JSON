#include "eqn_antoine.h"

double eqn_antoine::calc(DATAMAP& pairs, parms prms, double temperatureK, double xMass, std::string ref)
{
	para_antoine myPara(prms);

    double p0 = (myPara.a0 + (1000*myPara.b0 / (temperatureK - myPara.c)))*pow(100*xMass,0);
    double p1 = (myPara.a1 + (1000*myPara.b1 / (temperatureK - myPara.c)))*pow(100*xMass,1);
    double p2 = (myPara.a2 + (1000*myPara.b2 / (temperatureK - myPara.c)))*pow(100*xMass,2);
    double p3 = (myPara.a3 + (1000*myPara.b3 / (temperatureK - myPara.c)))*pow(100*xMass,3);
    double p4 = (myPara.a4 + (1000*myPara.b4 / (temperatureK - myPara.c)))*pow(100*xMass,4);

    double pKpa = exp(p0 + p1 + p2 + p3 + p4);

    return pKpa;
}
