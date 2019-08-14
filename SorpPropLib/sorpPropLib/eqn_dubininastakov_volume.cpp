#include "eqn_dubininastakov_volume.h"

double eqn_dubininastakov_volume::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_dubininastakov_volume myPara(prms);

	double Ps = refInfo.at(0);//kPa

	double tolerance = 1e-4;
	double p_guess = Ps / 10;
	double Y_guess = 0;
	double Y_guess_d = 0;
	double dYdp = 0;
	int counter = 0;
	double step = 0;

	for (Y_guess = calcY(pairs, myPara, tK, p_guess, refInfo); fabs(Y_guess - xMass)>tolerance&&counter<50; counter++) {


		//use Newton-Raphson to solve for pressure
		//calculate the functional value
		Y_guess = calcY(pairs, myPara, tK, p_guess, refInfo);

		//calculate the first derivative
		Y_guess_d = calcY(pairs, myPara, tK, p_guess + 0.001, refInfo);
		dYdp = (Y_guess_d - Y_guess) / 0.001;

		//update guess value
		step = (Y_guess - xMass) / dYdp;
		p_guess -= step / abs(step)*fmin(abs(step), p_guess / 1.1);
		//std::cout <<"xMass\t"<<xMass<< "\tYguess\t" << Y_guess <<"\tYgussd\t"<<Y_guess_d<< "\tdYdp\t" << dYdp << "\tp_guess\t" << p_guess << "\t";

		// Since the calcY function cannot be evaluated with negative p_guess, p_guess is kept above 0.01
		if (p_guess < 0) {
			p_guess = 0.0001;
		}

	}

	//std::cout << "p/ps\t" << p_guess<<'\t'<<Ps << "\t"<<p_guess/Ps<<'\n';

	return (counter == 50 ? -1 : p_guess);
}

double eqn_dubininastakov_volume::calcY(DATAMAP& pairs, const para_dubininastakov_volume& para, double tK, double pKpa, std::vector<double> refInfo)
{

	double rho = refInfo.at(1);//kg/m3
	double Ps = refInfo.at(0);//kPa

    double R = 8.314;
	double E = para.E;
	double n = para.n;

    double A = R * tK* log(Ps/pKpa);
    double W = para.W0 * exp(-pow(A/E,n));
    double Y = W * rho;

    return Y;
}
