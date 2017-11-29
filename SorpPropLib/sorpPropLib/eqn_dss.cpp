#include "eqn_dss.h"


double eqn_dss::calcY(const para_dss& para, double tK, double pKpa)
{
    double T0 = 323;
	double fA = exp(para.dHA / (8.314*tK)*(1 - tK / T0));
	double fB = exp(para.dHB / (8.314*tK)*(1 - tK / T0));
    double bA = para.b0A*fA;
    double bB = para.b0B*fB;

	double YAA = para.YA*pow((bA*pKpa),1/para.etaA)/(1+pow((bA*pKpa),1/para.etaA));
	
	double YBB = para.YB*pow((bB*pKpa),1/para.etaB)/(1+pow((bB*pKpa),1/para.etaB));

    return (YAA+YBB);
}

double eqn_dss::calc(const parms prms, double tK, double xMass, std::string ref)
{
	para_dss myPara(prms);

    double p_guess = 10;
    double Y_guess = 0;
    double tolerance = 1e-4;
    double scaler = 0.005;
    int counter = 0;

    for(Y_guess = calcY(myPara,tK,p_guess);fabs(Y_guess - xMass)>tolerance&&counter<100;counter++){

        Y_guess = calcY(myPara,tK,p_guess);

        if(counter%10==0){
        }

        p_guess += (xMass-Y_guess) * scaler;
        p_guess = p_guess<0?0:p_guess;

        if(counter%10==0){
        }
    }

    return (counter==1000?-1:p_guess);
}
