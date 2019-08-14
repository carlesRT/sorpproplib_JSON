#ifndef EQN_TOTH_H
#define EQN_TOTH_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_toth {
public:
    double Y0;
    double b_0;
    double qstar_R;
    double n_0;
    double C;
    double m;
    double r;

	para_toth(parms prms) {
		Y0 = prms.get("Y0");
		b_0 = prms.get("b_0");
		qstar_R = prms.get("qstar_R");
		n_0 = prms.get("n_0");
		C = prms.get("C");
		m = prms.get("m");
		r = prms.get("r");
	}
	para_toth(const para_toth &) {}
	para_toth & operator=(const para_toth& a) { return *this; }
};

class eqn_toth:public eqn_template
{
public:
    double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo);
    double calcY(const para_toth& para, double tK, double pKpa, std::vector<double> refInfo);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("Y0")) { s << "Y0" << ","; isOk = false; }
		if (!prms.has("b_0")) { s << "b_0" << ","; isOk = false; }
		if (!prms.has("qstar_R")) { s << "qstar_R" << ","; isOk = false; }
		if (!prms.has("n_0")) { s << "n_0" << ","; isOk = false; }
		if (!prms.has("C")) { s << "C" << ","; isOk = false; }
		if (!prms.has("m")) { s << "m" << ","; isOk = false; }
		if (!prms.has("r")) { s << "r" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_TOTH_H
