#ifndef PARMS_H
#define PARMS_H

#include <map>

#include <wh/nosjob/nosjob.hpp>
#include <wh/nosjob/nosjob_convert.hpp>

#include "IPair_rs.h"

/**
	Implementation of equation pair data.
		Three strings are used to identify each pair:
			referigerant name, adsorbent name and subtype.
		The equation pair data is a set (map) of equation
			parameters indexed by the equation name;
*/
class parms
{
public:
	parms(std::string e = "", std::string c = "");
	parms(nosjob::Object const & o);
	// Properties
	//   Equation Name - not restricted to lower case
	std::string getEquation() const { return eqn; }
	void setEquation(std::string s) { eqn = s; }
	//   Comment (Literature, references, etc).
	std::string getComment() const { return comment; }
	void setComment(std::string s) { comment = s; }

	nosjob::Object nativeToAtom();

	bool add(std::string name, double val);
	bool has(std::string name);
	std::map<std::string, double> getAll();
	double get(std::string name);
	bool erase(std::string name);
	int size();
	void clear();

protected:
	std::string eqn;
	std::string comment;
	std::map<std::string, double> ps;
};

namespace nosjob {
template <> struct NativeToAtom< parms >
{
	typedef Object AtomType;
	inline AtomType operator()(parms const & arg) const
	{
		return const_cast<parms&>(arg).nativeToAtom();
	}
};

template <> struct AtomToNative< parms >
{
    typedef parms ReturnType;

	inline parms operator()(Object const & o) const
	{
		return parms(o);
	}
	inline parms operator()(Atom const & o) const
	{
		return this->operator()(Object::cast(o));
	}
};
}

#endif // PARMS_H
