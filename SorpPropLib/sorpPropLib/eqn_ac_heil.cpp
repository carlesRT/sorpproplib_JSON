#include "eqn_ac_heil.h"

double eqn_ac_heil::calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::string ref)
{
	para_heil mpara(prms);

    double x1 = xMass, x2 = 1-xMass;

	double R = 8.314;

    double tau12 = mpara.dLambda12/(R*tK);
    double tau21 = mpara.dLambda21/(R*tK);

    double lambda12 = mpara.vm2/mpara.vm1*exp(-tau12);
    double lambda21 = mpara.vm1/mpara.vm2*exp(-tau21);

    double t1 = lambda21/(x1+x2*lambda21);
    double t2 = lambda12/(x1*lambda12+x2);
    double f1 = -log(x1+x2*lambda21);
    double f2 = x2*(t1-t2);
    double f3 = x2*x2*(tau12*pow(t1,2)+tau21/lambda12*pow(t2,2));

    return exp(f1 + f2 + f3);
}
