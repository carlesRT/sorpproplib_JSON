#ifndef EQN_AC_NRTL_FIXEDDG_H
#define EQN_AC_NRTL_FIXEDDG_H

#include "pair_template.h"
#include "eqn_template.h"

class para_ac_NRTL_fixdDg {
public:
    double dg12;
    double dg21;
    double alpha12;

	para_ac_NRTL_fixdDg(parms prms) {
		dg12 = prms.get("dg12");
		dg21 = prms.get("dg21");
		alpha12 = prms.get("alpha12");
	}
};

class eqn_ac_NRTL_fixdDg :public eqn_template
{
public:
    double calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::vector<double> refInfo);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("dg12")) { s << "dg12" << ","; isOk = false; }
		if (!prms.has("dg21")) { s << "dg21" << ","; isOk = false; }
		if (!prms.has("alpha12")) { s << "alpha12" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_NRTL_FIXEDDG_H
