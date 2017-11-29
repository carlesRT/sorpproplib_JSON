#ifndef EQN_DUHRING_H
#define EQN_DUHRING_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_duhring {
public:
	double a0;
    double a1;
    double a2;
    double a3;
    double b0;
    double b1;
    double b2;
    double b3;
    double n;
    double m;
    double r;
    double q;
    double C;
    double D;
    double E;

	para_duhring(parms prms) {
		a0 = prms.get("a0");
		a1 = prms.get("a1");
		a2 = prms.get("a2");
		a3 = prms.get("a3");
		b0 = prms.get("b0");
		b1 = prms.get("b1");
		b2 = prms.get("b2");
		b3 = prms.get("b3");
		n = prms.get("n");
		m = prms.get("m");
		r = prms.get("r");
		q = prms.get("q");
		C = prms.get("C");
		D = prms.get("D");
		E = prms.get("E");
	}
};

class eqn_duhring : public eqn_template
{
public:
    double calc(const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("a0")) { s << "a0" << ","; isOk = false; }
		if (!prms.has("a1")) { s << "a1" << ","; isOk = false; }
		if (!prms.has("a2")) { s << "a2" << ","; isOk = false; }
		if (!prms.has("a3")) { s << "a3" << ","; isOk = false; }
		if (!prms.has("b0")) { s << "b0" << ","; isOk = false; }
		if (!prms.has("b1")) { s << "b1" << ","; isOk = false; }
		if (!prms.has("b2")) { s << "b2" << ","; isOk = false; }
		if (!prms.has("b3")) { s << "b3" << ","; isOk = false; }
		if (!prms.has("n")) { s << "n" << ","; isOk = false; }
		if (!prms.has("m")) { s << "m" << ","; isOk = false; }
		if (!prms.has("r")) { s << "r" << ","; isOk = false; }
		if (!prms.has("q")) { s << "q" << ","; isOk = false; }
		if (!prms.has("C")) { s << "C" << ","; isOk = false; }
		if (!prms.has("D")) { s << "D" << ","; isOk = false; }
		if (!prms.has("E")) { s << "E" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_DUHRING_H
