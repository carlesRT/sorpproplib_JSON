#ifndef EQN_DSS_H
#define EQN_DSS_H

#include <iostream>
#include <cmath>

#include "pair_template.h"
#include "eqn_template.h"

class para_dss {
public:
	double b0A;
    double b0B;
    double etaA;
    double etaB;
    double YA;
    double YB;
    double dHA;
	double dHB;

	para_dss(parms prms) {
		b0A = prms.get("b0A");
		b0B = prms.get("b0B");
		etaA = prms.get("etaA");
		etaB = prms.get("etaB");
		YA = prms.get("YA");
		YB = prms.get("YB");
		dHA = prms.get("dHA");
		dHB = prms.get("dHB");
	}
	para_dss(const para_dss &) {}
	para_dss & operator=(const para_dss& a) { return *this; }
};

class eqn_dss :public eqn_template
{
public:
	double calcY(const para_dss& para, double tK, double pKpa);
	double calc(const parms prms, double tK, double xMass, std::string ref = "");
	bool check(parms prms, std::string& badparms) {
		bool isOk = true;
		std::ostringstream s;
		s << "\"" << prms.getEquation() << "\",";
		if (!prms.has("b0A")) { s << "b0A" << ","; isOk = false; }
		if (!prms.has("b0B")) { s << "b0B" << ","; isOk = false; }
		if (!prms.has("etaA")) { s << "etaA" << ","; isOk = false; }
		if (!prms.has("etaB")) { s << "etaB" << ","; isOk = false; }
		if (!prms.has("YA")) { s << "YA" << ","; isOk = false; }
		if (!prms.has("YB")) { s << "YB" << ","; isOk = false; }
		if (!prms.has("dHA")) {
			s << "dHA" << ","; isOk = false;
			if (!prms.has("dHB")) { s << "dHB" << ","; isOk = false; }
			if (!isOk) {
				s << std::endl;
				badparms = s.str();
			}
			return isOk;
		}
	}
};

#endif // EQN_DSS_H
