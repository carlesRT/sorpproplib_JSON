#include "eqn_mr_1pvdw.h"

double eqn_mr_1pvdw::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_1pvdw myPara(prms);

	double t_crit_r = 0;//ref.t_crit
	double p_crit_r = 0;//ref.p_crit
	double w_r = 0;//ref.w
	double vm = 0;//ref.vm

	double t_crit_s = myPara.t_crit_s;
	double p_crit_s = myPara.p_crit_s;
	double w_s = myPara.w_s;

	double tr = tK / t_crit_r;
	double R = 8.314;//gas constant

	double m = 0, n = 0, ac = 0, bc = 0, c0 = 0, c1 = 0, c2 = 0, c3 = 0;

	if(myPara.eos > 5) {//PR
		m = 2;
		n = -1;
		ac = 0.457240;
		bc = 0.07780;
		c0 = 0.37464;
		c1 = -1.54226;
		c2 = -0.26992;
		c3 = 0;
	}
	else if (myPara.eos < -5) {//SRK
		m = 1;
		n = 0;
		ac = 0.427480;
		bc = 0.08664;
		c0 = 0.378893;
		c1 = 1.1897153;
		c2 = -0.17131848;
		c3 = 0.0196554;
	}
	else {//PRSV
		m = 2;
		n = -1;
		ac = 0.457240;
		bc = 0.07780;
		c0 = 0.378893;
		c1 = 1.1897153;
		c2 = -0.17131848;
		c3 = 0.0196554;		
	}

	double k0_r = c1*w_r + c2*pow(w_r, 2) + c3*pow(w_r, 3);
	double k0_s = c1*w_s + c2*pow(w_s, 2) + c3*pow(w_s, 3);
	double kk_r = 0, kk_s = 0;
	if (abs(myPara.eos) > 5) {//PR or SRK
		kk_r = k0_r;
		kk_s = k0_s;
	}
	else {//PRSV
		kk_r = k0_r + myPara.k1_r*(1 + pow(tr, 0.5))*(0.7 - tr);
		kk_s = k0_s + myPara.k1_s*(1 + pow(tr, 0.5))*(0.7 - tr);
	}

	double alpha_r = pow(1 + kk_r*(1 - pow(tr, 0.5)), 2);
	double alpha_s = pow(1 + kk_s*(1 - pow(tr, 0.5)), 2);

	double a_r = ac*(pow(R, 2)*pow(t_crit_r, 2)) / pow(p_crit_r, 2)*alpha_r;
	double a_s = ac*(pow(R, 2)*pow(t_crit_s, 2)) / pow(p_crit_s, 2)*alpha_s;

	double b_r = bc*R*t_crit_r / p_crit_r;
	double b_s = bc*R*t_crit_s / p_crit_s;

	double aa = xMass*(1 - xMass)*sqrt(a_r*a_s)*(1 - myPara.kij);
	double bb = xMass*b_r + (1 - xMass)*b_s;

	double P = R*tK / (vm - bb) - aa / (pow(vm, 2) + m*bb*vm + n*pow(bb, 2));

	return P;
}
