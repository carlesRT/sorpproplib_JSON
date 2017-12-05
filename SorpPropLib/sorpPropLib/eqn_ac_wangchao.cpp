#include "eqn_ac_wangchao.h"

double eqn_ac_wangchao::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref)
{
    para_wangchao mpara(prms);

    double x1 = xMass, x2 = 1-xMass;

    double lambda12 = mpara.vm2/mpara.vm1*exp(-mpara.dLambda12/(8.314*tK));
    double lambda21 = mpara.vm1/mpara.vm2*exp(-mpara.dLambda21/(8.314*tK));
	/** Zhiyao 
		no lambda11 or lambda 12
	
	double A = (x2*exp(-lambda12/(8.314*tK)))/(x1*exp(-lambda11/(8.314*tK)));
    double B = (x2*exp(-lambda12/(8.314*tK)))/(x1*exp(-lambda22/(8.314*tK)));
	*/

    //figure out the calculation of Wang and Chao!!
}

