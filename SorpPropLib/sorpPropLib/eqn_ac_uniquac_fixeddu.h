#ifndef EQN_AC_UNIQUAC_FIXEDDU_H
#define EQN_AC_UNIQUAC_FIXEDDU_H

#include "pair_template.h"
#include "eqn_template.h"

class para_UNIQUAC_fixedDu {
public:
	double du12;
    double du21;
    double r1;
    double r2;
    double q1;
    double q2;
    double z;

	para_UNIQUAC_fixedDu(parms prms) {
		du12 = prms.get("du12");
		du21 = prms.get("du21");
		r1 = prms.get("r1");
		r2 = prms.get("r2");
		q1 = prms.get("q1");
		q2 = prms.get("q2");
		z = prms.get("z");
	}
};

class eqn_ac_UNIQUAC_fixedDu :public eqn_template
{
public:
    double calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("du12")) { s << "du12" << ","; isOk = false; }
		if (!prms.has("du21")) { s << "du21" << ","; isOk = false; }
		if (!prms.has("r1")) { s << "r1" << ","; isOk = false; }
		if (!prms.has("r2")) { s << "r2" << ","; isOk = false; }
		if (!prms.has("q1")) { s << "q1" << ","; isOk = false; }
		if (!prms.has("q2")) { s << "q2" << ","; isOk = false; }
		if (!prms.has("z")) { s << "z" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_AC_UNIQUAC_FIXEDDU_H
