#ifndef EQN_AC_NRTL_DG_T_H
#define EQN_AC_NRTL_DG_T_H

#include "pair_template.h"
#include "eqn_template.h"

class para_ac_NRTL_Dg_T {
public:
	double a12;
	double a21;
	double b12;
	double b21;
	double alpha12;

	para_ac_NRTL_Dg_T(parms prms) {
		a12 = prms.get("a12");
		a21 = prms.get("a21");
		b12 = prms.get("b12");
		b21 = prms.get("b21");
		alpha12 = prms.get("alpha12");
	}
};

class eqn_ac_NRTL_Dg_T :public eqn_template
{
public:
	double calc(const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("a12")) { s << "a12" << ","; isOk = false; }
		if (!prms.has("a21")) { s << "a21" << ","; isOk = false; }
		if (!prms.has("b12")) { s << "b12" << ","; isOk = false; }
		if (!prms.has("b21")) { s << "b21" << ","; isOk = false; }
		if (!prms.has("alpha12")) { s << "alpha12" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_NRTL_DG_T_H
