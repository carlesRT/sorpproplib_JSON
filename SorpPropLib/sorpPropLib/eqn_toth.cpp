#include "eqn_toth.h"


double eqn_toth::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref)
{
    para_toth myPara(prms);

    double tolerance = 1e-4;
    double scaler = 0.005;

    double p_guess = 10;
    double Y_guess = 0;

    int counter = 0;

    for(Y_guess = calcY(myPara,tK,p_guess);fabs(Y_guess - xMass)>tolerance&&counter<100;counter++){

        Y_guess = calcY(myPara,tK,p_guess);

        p_guess += (xMass-Y_guess) * scaler;
        p_guess = p_guess<0?0:p_guess;

    }

    return (counter==1000?-1:p_guess);
}

double eqn_toth::calcY(const para_toth& para, double tK, double pKpa)
{
    double n = para.n_0 + para.C/tK;
    double r = para.r<0?n:para.r;
	double b = para.b_0 / 1e6 * exp(para.qstar_R / tK);//b0 input is scaled by 1e6
    double Y = para.Y0 * pow(b,para.m) * pKpa / pow(1+pow(b,r)*pow(pKpa,n),1/n);

    return Y;
}
