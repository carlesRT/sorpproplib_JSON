#include "parms.h"

/**
	construct equation parameters
*/
parms::parms(std::string e, std::string c)
{
	eqn = e;
	comment = c;
}

/**
	ctor to construst equation parameters from a serializable object
*/
parms::parms(nosjob::Object const & o)
{
	static const nosjob::StringType e("_e_");
	static const nosjob::StringType c("_c_");
	static const nosjob::StringType p("_p_");
	eqn = nosjob::atomToNative<std::string>(o.get(e));
	comment = nosjob::atomToNative<std::string>(o.get(c));
	ps = nosjob::atomToNative<std::map<std::string, double>>(nosjob::Object::cast(o.get(p)));
}

/**
	convert equation parameters to a serializable object
*/
nosjob::Object parms::nativeToAtom()
{
	static const nosjob::StringType e("_e_");
	static const nosjob::StringType c("_c_");
	static const nosjob::StringType p("_p_");
	nosjob::Object obj;
	obj.set(e, nosjob::nativeToAtom(eqn));
	obj.set(c, nosjob::nativeToAtom(comment));
	obj.set(p, nosjob::nativeToAtom(ps));
	return obj;
};

inline std::string tol(const std::string str) {
	std::string st = str;
	std::transform(st.begin(), st.end(), st.begin(), ::tolower);
	return st;
}

/**
	add a named parameter
*/
bool parms::add(std::string name, double val)
{
	ps[tol(name)] = val;
}

/**
	does a named parameter exist
*/
bool parms::has(std::string name)
{
	auto f = ps.find(tol(name));
	return (f != ps.end());
}

/**
	get all named parameters
*/
std::map<std::string, double> parms::getAll()
{
	return ps;
}

/**
	get a named parameter
*/
double parms::get(std::string name)
{
	return ps[tol(name)];
}

/**
	remove a named parameter
*/
bool parms::erase(std::string name)
{
	ps.erase(tol(name));
}

/**
	count named parameters
*/
int parms::size() {
	return (int)ps.size();
}

/**
	remove all named parameters
*/
void parms::clear() {
	ps.clear();
}

