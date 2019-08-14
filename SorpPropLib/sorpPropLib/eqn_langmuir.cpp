#include "eqn_langmuir.h"

double eqn_langmuir::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_langmuir myPara(prms);

	double tolerance = 1e-4;

	double p_guess = 10;
	double Y_guess = 10;
	double Y_guess_d = 0;
	double dYdp = 0;

	int counter = 0;

	for (Y_guess = calcY(myPara, tK, p_guess, refInfo); fabs(Y_guess - xMass)>tolerance&&counter<50; counter++) {

		//use Newton-Raphson to solve for pressure
		//calculate the functional value
		Y_guess = calcY(myPara, tK, p_guess, refInfo);

		//calculate the first derivative
		Y_guess_d = calcY(myPara, tK, p_guess + 0.001, refInfo);
		dYdp = (Y_guess_d - Y_guess) / 0.001;

		//update guess value
		p_guess -= (Y_guess - xMass) / dYdp;
		//std::cout <<"xMass\t"<<xMass<< "\tYguess\t" << Y_guess <<"\tYgussd\t"<<Y_guess_d<< "\tdYdp\t" << dYdp << "\tp_guess\t" << p_guess << "\t";

		// Since the calcY function cannot be evaluated with negative p_guess 
		//(and the solution wouldn't be a negative pressure), p_guess is kept above 0.01
		if (p_guess < 0) {
			p_guess = 0.01;
		}
	}

	//std::cout << "iter\t" << counter << "\t";

	return (counter == 50 ? -1 : p_guess);
}

double eqn_langmuir::calcY(const para_langmuir& para, double tK, double pKpa, std::vector<double> refInfo)
{
    double K = para.K0/1000*exp(para.dH/(8.314*tK));

    //double Y = para.Y0*K*pKpa*pow((1-Y),para.alpha);
	double Y = para.Y0*K*pKpa/(1+ K*pKpa);

	//std::cout <<"K0 "<<para.K0<<" dH "<<para.dH<<" Y0 "<<para.Y0<<" K "<<K<< " y " << Y << " p " << pKpa << '\n';

    return Y;
}
