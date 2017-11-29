#ifndef REFRIGERANT_H
#define REFRIGERANT_H


#include "ref_template.h"
#include "pair_template.h"


#include <cmath>

class refrigerant : public ref_template
{
public:
	refrigerant(std::string sorbate, double temperatureK, double massRatio);

	double calc();

	std::string getResult() const { return result; }

	void calcEquations(pair_template*p);

	static double psat(double tK);

protected:
	std::string result;
	double myTemperatureK;
	double myMassRatio;

	refPara myRefPara;
};

#endif // REFRIGERANT_H
