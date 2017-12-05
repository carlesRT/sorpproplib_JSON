#ifndef EQN_LANGMUIR_H
#define EQN_LANGMUIR_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_langmuir {
public:
	double Y0;
    double K0;
    double dH;
    double alpha;

	para_langmuir(parms prms) {
		Y0 = prms.get("Y0");
		K0 = prms.get("K0");
		dH = prms.get("dH");
		alpha = prms.get("alpha");
	}
};

class eqn_langmuir:public eqn_template
{
public:
    double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref = "");
    double calcY(const para_langmuir& para,double tK, double pKpa);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("Y0")) { s << "Y0" << ","; isOk = false; }
		if (!prms.has("K0")) { s << "K0" << ","; isOk = false; }
		if (!prms.has("dH")) { s << "dH" << ","; isOk = false; }
		if (!prms.has("alpha")) { s << "alpha" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_LANGMUIR_H
