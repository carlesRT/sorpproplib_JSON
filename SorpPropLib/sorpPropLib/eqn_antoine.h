#ifndef EQN_ANTOINE_H
#define EQN_ANTOINE_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_antoine {
public:

    double a0;
    double a1;
    double a2;
    double a3;
    double a4;
    double b0;
    double b1;
    double b2;
    double b3;
    double b4;
    double c;

	para_antoine(parms prms) {
		a0 = prms.get("a0");
		a1 = prms.get("a1");
		a2 = prms.get("a2");
		a3 = prms.get("a3");
		a4 = prms.get("a4");
		b0 = prms.get("b0");
		b1 = prms.get("b1");
		b2 = prms.get("b2");
		b3 = prms.get("b3");
		b4 = prms.get("b4");
		c = prms.get("c");
		/** Zihyao

			no parms
		*/
		c = 43.15;
		a0 = a1 = a2 = a3 = a4 = b1 = b2 = b3 = b4 = 0;
	}
};

class eqn_antoine:public eqn_template
{
public:

    double calc(DATAMAP& pairs, const parms prms, double temperatureK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("a0")) { s << "a0" << ","; isOk = false; }
		if (!prms.has("a1")) { s << "a1" << ","; isOk = false; }
		if (!prms.has("a2")) { s << "a2" << ","; isOk = false; }
		if (!prms.has("a3")) { s << "a3" << ","; isOk = false; }
		if (!prms.has("a4")) { s << "a4" << ","; isOk = false; }
		if (!prms.has("b0")) { s << "b0" << ","; isOk = false; }
		if (!prms.has("b1")) { s << "b1" << ","; isOk = false; }
		if (!prms.has("b2")) { s << "b2" << ","; isOk = false; }
		if (!prms.has("b3")) { s << "b3" << ","; isOk = false; }
		if (!prms.has("b4")) { s << "b4" << ","; isOk = false; }
		if (!prms.has("c")) { s << "c" << ","; isOk = false; }
		if (!isOk) {
			s << std::endl;
			badparms = s.str();
		}
		return isOk;
	}
};

#endif // EQN_ANTOINE_H
