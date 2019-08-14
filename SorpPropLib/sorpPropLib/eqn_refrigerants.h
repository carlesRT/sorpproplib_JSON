#ifndef EQN_REFRIGERANTS_H
#define EQN_REFRIGERANTS_H

#include <iostream>
#include <cmath>

#include "eqn_template.h"
#include "pair_template.h"

class para_refrigerants{
public:
	double a0;
	double b0;
	double c;
	double rho;
	double vm;
	double p_crit;
	double t_crit;
	double w;

	para_refrigerants(parms prms) {
		a0 = prms.get("a0");
		b0 = prms.get("b0");
		c = prms.get("c");
		rho = prms.get("rho");
		vm = prms.get("vm");
		p_crit = prms.get("p_crit");
		t_crit = prms.get("t_crit");
		w = prms.get("w");
	}
	para_refrigerants(const para_refrigerants &) {}
	para_refrigerants & operator=(const para_refrigerants& a) { return *this; }
};

class eqn_refrigerants :
	public eqn_template
{
public:
	double calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo);
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		if (!prms.has("a0")) {
			s << "a0" << ","; isOk = false;
			if (!prms.has("b0")) { s << "b0" << ","; isOk = false; }
			if (!prms.has("c")) { s << "c" << ","; isOk = false; }
			if (!prms.has("vm")) { s << "vm" << ","; isOk = false; }
			if (!prms.has("rho")) { s << "rho" << ","; isOk = false; }
			if (!prms.has("p_crit")) { s << "p_crit" << ","; isOk = false; }
			if (!prms.has("t_crit")) { s << "t_crit" << ","; isOk = false; }
			if (!prms.has("w")) { s << "w" << ","; isOk = false; }
			if (!isOk) {
				s << std::endl;
				badparms = s.str();
			}
			return isOk;
		}
	}
};

#endif //EQN_REFRIGERANTS_H