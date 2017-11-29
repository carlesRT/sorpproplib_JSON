#include "refrigerant.h"

refrigerant::refrigerant(std::string sorbate, double temperatureK, double massRatio)
	:ref_template(sorbate, temperatureK, massRatio)
{
}

double refrigerant::calc()
{
}

double refrigerant::psat(double tK)
{
}

void ref_template::calcEquations(pair_template *p)
{
	p->init();
	if (!p->myEqns.empty()) {

		std::ostringstream s;
		result.clear();

		for (int i = 0; i < p->myEqns.size(); i++) {
			double pressure = p->calc(p->myEqns[i].eqn_type, myTemperatureK, myMassRatio);
			s.flush();
			s << myTemperatureK << "\t" << myMassRatio << "\t";
			if (pressure<0) {
				s << "N/A";
			}
			else {
				s << pressure;
			}
			s << "\t" << p->myEqns[i].eqn_type << "\t" << p->myEqns[i].eqn_reference << "\n";
			result.append(s.str());
		}

	}
	else {
		//report "no equation"
	}
}
