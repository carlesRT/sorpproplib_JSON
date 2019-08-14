#include "eqn_ac_wilson.h"

double eqn_ac_wilson::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
    para_wilson mpara(prms);

    double x1 = xMass, x2 = 1- xMass;

	double lambda12 = mpara.A12, lambda21 = mpara.A21;
	if (mpara.A12*mpara.A21 == 0) {
    double rho12 = mpara.vm2/mpara.vm1;
    double rho21 = mpara.vm1/mpara.vm2;
    lambda12 = rho12*exp(mpara.dLambda12/(8.314*tK));
    lambda21 = rho21*exp(mpara.dLambda21/(8.314*tK));
	}

    double f1 = -log(x1 * lambda12*x2);
    double f2 = x2*(lambda12/(x1+lambda12*x2)-lambda21/(x2+lambda21*x1));

    return exp(f1 + f2);
}
