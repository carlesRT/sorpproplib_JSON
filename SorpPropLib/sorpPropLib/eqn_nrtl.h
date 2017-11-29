#ifndef EQN_NRTL_H
#define EQN_NRTL_H

#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_NRTL {
public:
	double q;
    double r;

	para_NRTL(parms prms) {
		q = prms.get("q");
		r = prms.get("r");
	}
};

class eqn_NRTL
{
public:
    eqn_NRTL(para_NRTL para);

    double calc(double tK, double xMass);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("q")) { s << "q" << ","; isOk = false; }
		if (!prms.has("r")) { s << "r" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_NRTL_H
