#ifndef EOS_WATER_H
#define EOS_WATER_H

#include <cmath>

class eos_water
{
public:
    eos_water();

    static double psat(double tK);
};

#endif // EOS_WATER_H
