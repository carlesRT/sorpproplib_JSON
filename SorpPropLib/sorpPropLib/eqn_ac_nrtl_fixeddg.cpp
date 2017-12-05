#include "eqn_ac_nrtl_fixeddg.h"

double eqn_ac_NRTL_fixdDg::calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::string ref)
{
	para_ac_NRTL_fixdDg mpara(prms);

    double x1 = xMass, x2 = 1-xMass;
    double dg12 = mpara.dg12;
    double dg21 = mpara.dg21;

    double tau12 = dg12/(8.314*tK);
    double tau21 = dg21/(8.314*tK);

    double f1 = tau21*exp(-2*mpara.alpha12*tau21)
            /pow((x1 + x2*exp(-2*mpara.alpha12*tau21)),2);
    double f2 = tau12*exp(-2*mpara.alpha12*tau12)
            /pow((x1 + x2*exp(-2*mpara.alpha12*tau12)),2);

    return exp(pow(1-x1,2)*(f1+f2));
}
