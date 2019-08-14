#include "eqn_ac_uniquac_du_t.h"

double eqn_ac_UNIQUAC_Du_T::calc(DATAMAP& pairs,const parms prms, double tK, double xMass, std::vector<double> refInfo)
{
	para_UNIQUAC_Du_T mpara(prms);

	double x1 = xMass, x2 = 1 - xMass;
	double du12 = mpara.a12 + mpara.b12*tK;
	double du21 = mpara.a21 + mpara.b21*tK;

	double phi1 = (mpara.r1*x1) / (mpara.r1*x1 + mpara.r2*x2);
	double phi2 = (mpara.r2*x2) / (mpara.r1*x1 + mpara.r2*x2);
	double theta1 = (mpara.q1*x1) / (mpara.q1*x1 + mpara.q2*x2);
	double theta2 = (mpara.q2*x2) / (mpara.q1*x1 + mpara.q2*x2);

	double l1 = (mpara.z / 2)*(mpara.r1 - mpara.q1) - (mpara.r1 - 1);
	double l2 = (mpara.z / 2)*(mpara.r2 - mpara.q2) - (mpara.r2 - 1);

	double tau12 = exp(-du12 / (8.314*tK));
	double tau21 = exp(-du21 / (8.314*tK));

	double f0 = theta1*tau12 + theta2*tau21;
	double f1 = log(phi1 / x1) + (mpara.z / 2)*log(theta1 / phi1) + l1;
	double f2 = -phi1 / x1*(x1*l1 + x2*l2);
	double f3 = -mpara.q1*log(theta1*tau12 + theta2*tau21);
	double f4 = mpara.q1 - mpara.q1*(theta1*tau21 + theta2*tau12) / f0;

	return exp(f1 + f2 + f3 + f4);
}
