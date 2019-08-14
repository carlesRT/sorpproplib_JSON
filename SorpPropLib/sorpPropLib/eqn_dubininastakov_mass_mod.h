#ifndef EQN_DUBININASTAKOV_MASS_MOD_H
#define EQN_DUBININASTAKOV_MASS_MOD_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"


class para_dubininastakov_mass_mod {
public:
	double Y0;
	double k;
	double n;

	para_dubininastakov_mass_mod(parms prms) {
		Y0 = prms.get("Y0");
		k = prms.get("k");
		n = prms.get("n");
	}
};

class eqn_dubininastakov_mass_mod :public eqn_template
{
public:
	double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo);
	double calcY(DATAMAP& pairs, const para_dubininastakov_mass_mod& para, double tK, double tsat, std::vector<double> refInfo);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("Y0")) { s << "Y0" << ","; isOk = false; }
		if (!prms.has("k")) { s << "k" << ","; isOk = false; }
		if (!prms.has("n")) { s << "n" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_DUBININASTAKOV_MASS_MOD_H
