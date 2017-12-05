#ifndef EQN_AC_UNIQUAC_DU_T_H
#define EQN_AC_UNIQUAC_DU_T_H

#include "pair_template.h"
#include "eqn_template.h"

class para_UNIQUAC_Du_T {
public:
	double a12;
	double a21;
	double b12;
	double b21;
	double r1;
	double r2;
	double q1;
	double q2;
	double z;

	para_UNIQUAC_Du_T(parms prms) {
		a12 = prms.get("a12");
		a21 = prms.get("a21");
		b12 = prms.get("b12");
		b21 = prms.get("b21");
		r1 = prms.get("r1");
		r2 = prms.get("r2");
		q1 = prms.get("q1");
		q2 = prms.get("q2");
		z = prms.get("z");
	}
};

class eqn_ac_UNIQUAC_Du_T :public eqn_template
{
public:
	double calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("a12")) { s << "a12" << ","; isOk = false; }
		if (!prms.has("a21")) { s << "a21" << ","; isOk = false; }
		if (!prms.has("b12")) { s << "b12" << ","; isOk = false; }
		if (!prms.has("b21")) { s << "b21" << ","; isOk = false; }
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
