#include "eqn_duhring.h"


double eqn_duhring::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
    para_duhring myPara(prms);

	double tt = tK - 273.15;
    double xx = xMass * 100;

    double A = myPara.a0 + myPara.a1 * xx + myPara.a2 * pow(xx,2) + myPara.a3 * pow(xx,3);
    double B = myPara.b0 + myPara.b1 * xx + myPara.b2 * pow(xx,2) + myPara.b3 * pow(xx,3);
    double tsat = (myPara.n * tt + myPara.m - B)/A;
    double tt1 = tsat + myPara.q;
    double pp =  exp(myPara.C + myPara.D/tt1 + myPara.E/pow(tt1,2))/ myPara.r;
	
    return pp;
}
