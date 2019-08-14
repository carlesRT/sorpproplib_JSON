#include "eqn_refrigerants.h"

double eqn_refrigerants::calc(DATAMAP& pairs, const parms prms, double tK, double xMass, std::vector<double> refInfo) 
{
	para_refrigerants myPara(prms);
	
	//from NIST Webbook, originally P(bar)=f(T(K))
	
	return pow(10,myPara.a0 - myPara.b0 / (tK + myPara.c)) * 100;
}