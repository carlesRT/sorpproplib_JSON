#include "pair_rs.h"

/**
	default ctor
*/
pair_rs::pair_rs()
{
}

/**
	construct an equation constants pair
*/
pair_rs::pair_rs(std::string r, std::string t, std::string s)
{
	setKey(r, t, s);
}

/**
	add parameters for an equation
*/
int pair_rs::addData(std::string eqn, std::vector<std::string> col_names, std::vector<std::string> data)
{
	int count = 0;
	bool have_first = false;
	std::string e = eqn;
	std::string comment = "";
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	for (int i = 0; i < data.size(); i++) {
		if (i > 2)
		{
			if (i < (col_names.size() - 1)) {
				char * e;
				errno = 0;
				double val = std::strtod(data[i].c_str(), &e);
				if ((*e != '\0') || (errno != 0))
				{
					throw std::invalid_argument("Unable to convert data items");
				}
				add(eqn, col_names[count + 3], val);
			}
			else if (i < (data.size() - 1)) {
				if (have_first) {
					comment += ',';
				}
				have_first = true;
				comment += data[i];
			}
			else {
				if (have_first) {
					comment += ',';
				}
				comment += data[i];
				eqn_parms[e].setComment(comment);
			}
			count++;
		}
		else {
			switch (i) {
			case 0: { ref_name = data[i]; } break;
			case 1: { sorp_type = data[i]; } break;
			case 2: { subtype = data[i]; } break;
			}
		}
	}
	return count;
}

std::string pair_rs::getComment(const std::string eqn)
{
	std::string e = eqn;
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	if (has(e)) {
		return eqn_parms[e].getComment();
	}
}

void pair_rs::setComment(std::string eqn, std::string c)
{
	std::string e = eqn;
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	if (has(e)) {
		eqn_parms[e].setComment(c);
	}
	else {
		parms p;
		p.setEquation(eqn);
		p.setComment(c);
		eqn_parms[e] = p;
	}
}

/**
	set identification keys for an equation constants pair
*/
void pair_rs::setKey(std::string r, std::string s, std::string t)
{
	ref_name = r;
	sorp_type = s;
	subtype = t;
}

/**
	add a named equation parameter to this pair
*/
bool pair_rs::add(std::string eqn, std::string name, double val)
{
	parms ps;
	std::string e = eqn;
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	auto f = eqn_parms.find(e);
	if (f != eqn_parms.end()) {
		ps = eqn_parms[f->first];
	}
	else {
		ps.setEquation(eqn);
	}
	ps.add(name, val);
	eqn_parms[e] = ps;
}

/**
	does a set of equation parameters exist for this pair
*/
bool pair_rs::has(std::string e)
{
	auto f = eqn_parms.find(e);
	return (f != eqn_parms.end());
}

/**
	get a set of equation parameter for this pair
*/
std::map<std::string, double> *pair_rs::get(std::string e)
{
	auto f = eqn_parms.find(e);
	if (f != eqn_parms.end()) {
		return new std::map<std::string, double> (eqn_parms[f->first].getAll());
	}
	return nullptr;
}

/**
	remove a set of equation parameters from this pair
*/
bool pair_rs::erase(std::string e)
{
	eqn_parms.erase(e);
}

/**
	count the sets of named equation parameters for this pair
*/
int pair_rs::size() {
	return (int) eqn_parms.size();
}

/**
	remove all named equation parameter from this pair
*/
void pair_rs::clear() {
	eqn_parms.clear();
}
