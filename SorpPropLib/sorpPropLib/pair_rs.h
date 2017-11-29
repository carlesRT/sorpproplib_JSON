#ifndef PAIR_RS_H
#define PAIR_RS_H

#include <list>
#include <map>
#include <tuple>
#include <cstdarg>
#include <vector>

#include "parms.h"

/**
	Implementation of equation parameters.
	Identfied by equation name.  This class contains a set (map)
		of numeric parameters identified by parameter name.
*/
class pair_rs : public IPair_rs
{
public:
	pair_rs();
	pair_rs(std::string r, std::string t = "", std::string s = "");

	// Properties
	//   Refrigerant
	std::string getRefName() const { return ref_name; }
	void setRefName(std::string s) { ref_name = s; }
	//   Adsorbate Type
	std::string getSorpType() const { return sorp_type; }
	void setSorpType(std::string s) { sorp_type = s; }
	//   Adsorbate SubType
	std::string getSubType() const { return subtype; }
	void setSubType(std::string s) { subtype = s; }
	//   Comment (literature / reference)
	std::string getComment(const std::string eqn);
	void setComment(std::string eqn, std::string c);
	//   Key
	PK getKey() const { return PK(ref_name, sorp_type, subtype); }
	void setKey(std::string r, std::string s, std::string t);

	std::map<std::string, parms> eqn_parms;

	// Pair API
	//   Initialization
	int addData(std::string eqn, std::vector<std::string> col_names, std::vector<std::string> data);
	//   Data Functions
	bool add(std::string e, std::string nane, double val);
	bool has(std::string e);
	std::map<std::string, double> *get(std::string e);
	bool erase(std::string e);
	int size();
	void clear();

protected:
	std::string ref_name;
	std::string sorp_type;
	std::string subtype;
};

//
// (de)serialization support
//
namespace nosjob {
	/**
		Convert PK to serializable object
	*/
	template <> struct NativeToAtom< PK >
	{
		typedef Object AtomType;
		inline AtomType operator()(PK const & arg) const
		{
			static const StringType r("_rf_");
			static const StringType s("_as_");
			static const StringType t("_st_");

			Object obj;
			obj.set(r, nativeToAtom(std::get<0>(arg)));
			obj.set(s, nativeToAtom(std::get<1>(arg)));
			obj.set(t, nativeToAtom(std::get<2>(arg)));
			return obj;
		}
	};

	/**
		Convert serializable object to PK
	*/
	template <> struct AtomToNative< PK >
	{
		typedef PK ReturnType;

		inline PK operator()(Object const & o) const
		{
			static const StringType r("_rf_");
			static const StringType s("_as_");
			static const StringType t("_st_");

			nosjob::Atom a;

			std::string rf = nosjob::atomToNative<std::string>(o.get(r));
			std::string as = nosjob::atomToNative<std::string>(o.get(s));
			std::string st = nosjob::atomToNative<std::string>(o.get(t));
			return PK(rf, as, st);
		}
		inline PK operator()(Atom const & ar) const {
			return this->operator()(Object::cast(ar));
		}
	};

	/**
		Convert pair_rs to serializable object
	*/
	template <> struct NativeToAtom< pair_rs >
	{
		typedef Object AtomType;
		inline AtomType operator()(pair_rs const & arg) const
		{
			static const StringType r("_r_");
			static const StringType s("_s_");
			static const StringType t("_t_");
			static const StringType p("_ep_");

			Object obj;
			obj.set(r, nativeToAtom(arg.getRefName()));
			obj.set(s, nativeToAtom(arg.getSorpType()));
			obj.set(t, nativeToAtom(arg.getSubType()));
			obj.set(p, nativeToAtom(arg.eqn_parms));
			return obj;
		}
	};

	/**
		Convert serializable object to pair_rs
	*/
	template <> struct AtomToNative< pair_rs >
	{
		typedef pair_rs ReturnType;

		inline pair_rs operator()(Object const & o) const
		{
			static const StringType r("_r_");
			static const StringType s("_s_");
			static const StringType t("_t_");
			static const StringType e("_ep_");

			nosjob::Atom a;

			pair_rs rs;
			rs.setRefName(nosjob::atomToNative<std::string>(o.get(r)));
			rs.setSorpType(nosjob::atomToNative<std::string>(o.get(s)));
			rs.setSubType(nosjob::atomToNative<std::string>(o.get(t)));
			rs.eqn_parms = atomToNative<std::map<std::string, parms>>(nosjob::Object::cast(o.get(e)));
			return rs;
		}
		inline pair_rs operator()(Atom const & ar) const {
			return this->operator()(Object::cast(ar));
		}
	};
}
#endif // PAIR_RS_H
