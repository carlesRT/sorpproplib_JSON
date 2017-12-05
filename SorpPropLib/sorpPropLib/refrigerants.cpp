#include "refrigerants.h"

double refrigerants::calcPsat(const parms prms, double tK) {
	para_refrigerants myPara(prms);
	double a = myPara.a0;
	double b = myPara.b0;
	double c = myPara.c;
	double p = pow(10, a-(b / (tK + c)));//in bar
	return p*100;//return kPa
}

double refrigerants::getDensity(const parms prms) {
	para_refrigerants myPara(prms);
	return myPara.rho;
}

double refrigerants::getMolVol(const parms prms) {
	para_refrigerants myPara(prms);
	return myPara.vm;

}

double refrigerants::getPcrit(const parms prms) {
	para_refrigerants myPara(prms);
	return myPara.p_crit;

}

double refrigerants::getTcrit(const parms prms) {
	para_refrigerants myPara(prms);
	return myPara.t_crit;

}

double refrigerants::getAccenticFactor(const parms prms) {
	para_refrigerants myPara(prms);
	return myPara.w;

}