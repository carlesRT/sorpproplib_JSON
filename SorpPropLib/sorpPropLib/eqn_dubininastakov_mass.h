#ifndef EQN_DUBININASTAKOV_MASS_H
#define EQN_DUBININASTAKOV_MASS_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"


class para_dubininastakov_mass {
public:
	double Y0;
	double E;
	double n;

	para_dubininastakov_mass(parms prms) {
		Y0 = prms.get("Y0");
		E = prms.get("E");
		n = prms.get("n");
	}
};

class eqn_dubininastakov_mass :public eqn_template
{
public:
	double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref = "");
	double calcY(DATAMAP& pairs, const para_dubininastakov_mass& para, double tK, double pKpa, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("Y0")) { s << "Y0" << ","; isOk = false; }
		if (!prms.has("E")) { s << "E" << ","; isOk = false; }
		if (!prms.has("n")) { s << "n" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_DUBININASTAKOV_MASS_H
