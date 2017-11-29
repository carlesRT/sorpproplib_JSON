#ifndef PAIR_TEMPLATE_H
#define PAIR_TEMPLATE_H

#include <iostream>
#include <vector>

#include "pair_rs.h"

class pair_template
{
public:    
	pair_template(pair_rs rs) { pairRS = rs; }

    //initiate pair with equation/reference list and equation paramters
    //main location for pair data
    virtual void init() = 0;

    //invoke corresponding equation to calculate the pressure
    //passing tK, xMass, as well as the equation-pair parameter object to equation class
    virtual double calc(std::string eqn, double tK, double xMass) =0;

    pair_rs pairRS;
};

#endif // PAIR_TEMPLATE_H
