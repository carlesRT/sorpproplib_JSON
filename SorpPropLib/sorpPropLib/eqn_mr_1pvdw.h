#ifndef EQN_MR_1PVDW_H
#define EQN_MR_1PVDW_H

#include "pair_template.h"
#include "eqn_template.h"

class para_1pvdw {
public:
	double kij;//PR or PRSV
	double kij_2;//SRK
	double t_crit_s;
	double p_crit_s;
	double w_s;
	double eos;//10=PR, -10=SRK, 0=PRSV
	double k1_r;
	double k1_s;

	para_1pvdw(parms prms) {
		kij = prms.get("kij");
		kij_2 = prms.get("kij_2");
		t_crit_s = prms.get("t_crit_s");
		p_crit_s = prms.get("p_crit_s");
		w_s = prms.get("w_s");
		eos = prms.get("eos");
		k1_r = prms.get("k1_r");
		k1_s = prms.get("k1_s");
	}
};

class eqn_mr_1pvdw :public eqn_template
{
public:
	double calc(const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("kij")) { s << "kij" << ","; isOk = false; }
		if (!prms.has("kij_2")) { s << "kij_2" << ","; isOk = false; }
		if (!prms.has("t_crit_s")) { s << "t_crit_s" << ","; isOk = false; }
		if (!prms.has("p_crit_s")) { s << "p_crit_s" << ","; isOk = false; }
		if (!prms.has("w_s")) { s << "w_s" << ","; isOk = false; }
		if (!prms.has("eos")) { s << "eos" << ","; isOk = false; }
		if (!prms.has("k1_r")) { s << "k1_r" << ","; isOk = false; }
		if (!prms.has("k1_s")) { s << "k1_s" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_MR_1PVDW_H