#ifndef IPAIR_RS
#define IPAIR_RS

#include <map>
#include <tuple>

typedef std::tuple<std::string, std::string, std::string> PK;

/**
	Interface for accessing equation pair data.
*/
class IPair_rs
{
public:
	// Properties
	//   Refrigerant - must be lower case
	virtual std::string getRefName() const = 0;
	virtual void setRefName(std::string s) = 0;
	//   Adsorbate Type - must be lower case
	virtual std::string getSorpType() const = 0;
	virtual void setSorpType(std::string s) = 0;
	//   Adsorbate SubType - must be lower case
	virtual std::string getSubType() const = 0;
	virtual void setSubType(std::string s) = 0;
	//   Comment (literature / reference)
	virtual std::string getComment(const std::string eqn) = 0;
	virtual void setComment(std::string eqn, std::string c) = 0;
	//   Key
	virtual PK getKey() const = 0;
	virtual void setKey(std::string r, std::string s, std::string t) = 0;

	// Equation Parameters
	virtual bool add(std::string e, std::string nane, double val) = 0;
	virtual bool has(std::string e) = 0;
	virtual std::map<std::string, double> *get(std::string e) = 0;
	virtual bool erase(std::string e) = 0;
	virtual int size() = 0;
	virtual void clear() = 0;
//	virtual void destroy() = 0;
};

typedef std::map<PK, IPair_rs *> DATAMAP;

#endif // IPAIR_RS
