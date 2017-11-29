#ifndef EQN_AC_FLORYHUGGINS_H
#define EQN_AC_FLORYHUGGINS_H

#include "pair_template.h"
#include "eqn_template.h"

class para_floryhuggins {
public:
    double w0;
    double w1;
    double r;

	para_floryhuggins(parms prms) {
		w0 = prms.get("w0");
		w1 = prms.get("w1");
		r = prms.get("r");
	}
};

class eqn_ac_floryhuggins:public eqn_template
{
public:
    double calc(const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("w0")) { s << "w0" << ","; isOk = false; }
		if (!prms.has("w1")) { s << "w1" << ","; isOk = false; }
		if (!prms.has("r")) { s << "r" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_AC_FLORYHUGGINS_H
