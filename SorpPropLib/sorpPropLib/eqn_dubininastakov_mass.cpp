#include "eqn_dubininastakov_mass.h"
#include "refrigerants.h"

double eqn_dubininastakov_mass::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref)
{
	para_dubininastakov_mass myPara(prms);
	double p_guess = 10;
	double Y_guess = 0;
	double tolerance = 1e-4;
	double scaler = 0.005;
	int counter = 0;

	for (Y_guess = calcY(pairs, myPara, tK, p_guess); fabs(Y_guess - xMass)>tolerance&&counter<100; counter++) {

		Y_guess = calcY(pairs, myPara, tK, p_guess);

		p_guess += (xMass - Y_guess) * scaler;
		p_guess = p_guess<0 ? 0 : p_guess;
	}

	return (counter == 1000 ? -1 : p_guess);
}

double eqn_dubininastakov_mass::calcY(DATAMAP& pairs, const para_dubininastakov_mass& para, double tK, double pKpa, std::string ref)
{
	PK rsKey(ref, "dum_sorb", "dum_subtype");

	DATAMAP::iterator it = pairs.find(rsKey);
	if (it != pairs.end()) {
		pair_rs *pr = (pair_rs *)pairs[rsKey];

		if (pr->eqn_parms.size() < 1) {
			std::cout << "no equations found for: " << ref;
		}
		else {
			for (auto prm : pr->eqn_parms) {
				
			}
		}
	}
	else {
		std::cout << ref << " not found\n";
	}
	double rho = 0.5;
	double Ps = 1;
	double R = 8.314;
	double E = para.E;
	double n = para.n;

	double A = R * tK* log(Ps / pKpa);
	double W = para.Y0 * exp(-pow(A / E, n));
	double Y = W * rho;

	return Y;
}