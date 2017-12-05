#include "eqn_dubininastakov_volume.h"

double eqn_dubininastakov_volume::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref)
{
	para_dubininastakov_volume myPara(prms);
    double p_guess = 10;
    double Y_guess = 0;
    double tolerance = 1e-4;
    double scaler = 0.005;
    int counter = 0;

    for(Y_guess = calcY(myPara,tK,p_guess);fabs(Y_guess - xMass)>tolerance&&counter<100;counter++){

        Y_guess = calcY(myPara,tK,p_guess);

        p_guess += (xMass-Y_guess) * scaler;
        p_guess = p_guess<0?0:p_guess;
    }

    return (counter==1000?-1:p_guess);
}

double eqn_dubininastakov_volume::calcY(const para_dubininastakov_volume& para, double tK, double pKpa)
{
    double rho = 0.5;
    double Ps = 1;
    double R = 8.314;
	double E = para.E;
	double n = para.n;

    double A = R * tK* log(Ps/pKpa);
    double W = para.W0 * exp(-pow(A/E,n));
    double Y = W * rho;

    return Y;
}
