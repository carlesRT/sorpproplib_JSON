#include "eqn_dubininastakov_mass_mod.h"
#include "refrigerants.h"
#include "sorpproplib.h"

double eqn_dubininastakov_mass_mod::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_dubininastakov_mass_mod myPara(prms);

	double Ps = refInfo.at(0);//kPa

	double tolerance = 1e-4;
	double t_guess = tK /1.1;
	double Y_guess = 0;
	double Y_guess_d = 0;
	double dYdt = 0;
	int counter = 0;
	double step = 0;

	for (Y_guess = calcY(pairs, myPara, tK, t_guess, refInfo); fabs(Y_guess - xMass)>tolerance&&counter<50; counter++) {


		//use Newton-Raphson to solve for pressure
		//calculate the functional value
		Y_guess = calcY(pairs, myPara, tK, t_guess, refInfo);

		//calculate the first derivative
		Y_guess_d = calcY(pairs, myPara, tK, t_guess + 0.001, refInfo);
		dYdt = (Y_guess_d - Y_guess) / 0.001;

		//update guess value
		step = (Y_guess - xMass) / dYdt;
		t_guess -= step / abs(step)*fmin(abs(step), t_guess / 1.1);
		//std::cout <<"xMass\t"<<xMass<< "\tYguess\t" << Y_guess <<"\tYgussd\t"<<Y_guess_d<< "\tdYdp\t" << dYdp << "\tp_guess\t" << p_guess << "\t";

		// Since the calcY function cannot be evaluated with negative p_guess, p_guess is kept above 0.01
		if (t_guess < 0) {
			t_guess = 1;
		}

	}

	double pp = pow(10, refInfo.at(2) - refInfo.at(3) / (t_guess + refInfo.at(4))) * 100;
	return (counter == 50 ? -1 : pp);

}

double eqn_dubininastakov_mass_mod::calcY(DATAMAP& pairs, const para_dubininastakov_mass_mod& para, double tK, double tsat, std::vector<double> refInfo)
{

	double k = para.k;
	double n = para.n;
	
	double Y = para.Y0 * exp(-k*pow(tK / tsat-1, n));

	//std::cout << "tsat\t" << tsat << "t\Y\t" << Y << '\n';
	return Y;
}