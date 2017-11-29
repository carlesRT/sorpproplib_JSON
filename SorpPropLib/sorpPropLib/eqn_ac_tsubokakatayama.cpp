#include "eqn_ac_tsubokakatayama.h"

double eqn_ac_tsubokakatayama::calc(const parms prms, double tK, double xMass, std::string ref)
{
	para_tsubokakatayama mpara(prms);

    double x1 = xMass, x2 = 1-xMass;
    double rho12 = mpara.vm2/mpara.vm1;
    double rho21 = mpara.vm1/mpara.vm2;
    double lambda12 = rho12*exp(mpara.dLambda12/(8.314*tK));
    double lambda21 = rho21*exp(mpara.dLambda21/(8.314*tK));

    double f1 = -log(x1+x2*lambda21);
    double f2 = x2*(lambda21/(x1+x2*lambda21)
                    -lambda12/(x1*lambda12+x2));
    double f3 = log(x1+x2*rho21);
    double f4 = -x2*(rho21/(x1+x2*rho21)-rho12/(x1*rho12+x2));

    return exp(f1 + f2 + f3 + f4);

}
