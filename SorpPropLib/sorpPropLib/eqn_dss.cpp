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

double eqn_dss::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_dss myPara(prms);

	double tolerance = 1e-4;

	double p_guess = 10;
	double Y_guess = 10;
	double Y_guess_d = 0;
	double dYdp = 0;

	int counter = 0;

	for (Y_guess = calcY(myPara, tK, p_guess); fabs(Y_guess - xMass)>tolerance&&counter<50; counter++) {


		//use Newton-Raphson to solve for pressure
		//calculate the functional value
		Y_guess = calcY(myPara, tK, p_guess);

		//calculate the first derivative
		Y_guess_d = calcY(myPara, tK, p_guess + 0.001);
		dYdp = (Y_guess_d - Y_guess) / 0.001;

		//update guess value
		p_guess -= (Y_guess - xMass) / dYdp;
		//std::cout <<"xMass\t"<<xMass<< "\tYguess\t" << Y_guess <<"\tYgussd\t"<<Y_guess_d<< "\tdYdp\t" << dYdp << "\tp_guess\t" << p_guess << "\t";

		// Since the calcY function cannot be evaluated with negative p_guess, p_guess is kept above 0.01
		if (p_guess < 0) {
			p_guess = 0.01;
		}

	}

	//std::cout << "iter\t" << counter << "\t";

	return (counter == 50 ? -1 : p_guess);
}
