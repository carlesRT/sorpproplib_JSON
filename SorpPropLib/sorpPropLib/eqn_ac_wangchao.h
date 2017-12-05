#ifndef EQN_AC_WANGCHAO_H
#define EQN_AC_WANGCHAO_H

#include "pair_template.h"
#include "eqn_template.h"

class para_wangchao {
public:
	double vm1;
    double vm2;
    double dLambda12;
    double dLambda21;

	para_wangchao(parms prms) {
		vm1 = prms.get("vm1");
		vm2 = prms.get("vm2");
		dLambda12 = prms.get("dLambda12");
		dLambda21 = prms.get("dLambda21");
	}
};

class eqn_ac_wangchao:public eqn_template
{
public:
    double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("dLambda12")) { s << "dLambda12" << ","; isOk = false; }
		if (!prms.has("dLambda21")) { s << "dLambda21" << ","; isOk = false; }
		if (!prms.has("vm1")) { s << "vm1" << ","; isOk = false; }
		if (!prms.has("vm2")) { s << "vm2" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_AC_WANGCHAO_H
