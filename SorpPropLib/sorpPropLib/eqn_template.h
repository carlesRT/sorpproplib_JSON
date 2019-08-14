#ifndef EQN_TEMPLATE_H
#define EQN_TEMPLATE_H

#include <iostream>
#include <cmath>
#include <sstream>

#include "parms.h"
#include "pair_template.h"

class eqn_template
{
public:
    virtual double calc(DATAMAP& pairs, parms prms, double tK, double xMass, std::vector<double> refInfo) = 0;
	virtual bool check(parms prms, std::string& badparms) = 0;

};

#endif // EQN_TEMPLATE_H
