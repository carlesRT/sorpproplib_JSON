#include "eqn_langmuir.h"

double eqn_langmuir::calc(const parms prms, double tK, double xMass, std::string ref)
{
	para_langmuir myPara(prms);

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

double eqn_langmuir::calcY(const para_langmuir& para, double tK, double pKpa)
{
    double K = para.K0*exp(para.dH/(8.314*tK));

    //figure out how to solve MSL
	/** Zhiyao
		no Y
    double Y = para.Y0*K*pKpa*pow((1-Y),para.alpha);

    return Y;
	*/
}
