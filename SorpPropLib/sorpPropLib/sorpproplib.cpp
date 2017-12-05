#include <algorithm> 
#include <cctype>
#include <cwctype>
#include <iterator>
#include <list>
#include <locale>
#include <memory>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
#include <wh/nosjob/nosjob.hpp>
#include <wh/nosjob/nosjob_convert.hpp>
#include <wh/nosjob/nosjob_s11n.hpp>

#include "sorpproplib.h"

#include "pair_rs.h"
#include "eqn_template.h"
#include "eqn_ac_floryhuggins.h"
#include "eqn_ac_heil.h"
#include "eqn_ac_nrtl_fixeddg.h"
#include "eqn_ac_nrtl_dg_t.h"
#include "eqn_ac_tsubokakatayama.h"
#include "eqn_ac_uniquac_du_t.h"
#include "eqn_ac_uniquac_fixeddu.h"
#include "eqn_ac_wangchao.h"
#include "eqn_ac_wilson.h"
#include "eqn_antoine.h"
#include "eqn_dss.h"
#include "eqn_dubininastakov_volume.h"
#include "eqn_dubininastakov_mass.h"
#include "eqn_duhring.h"
#include "eqn_langmuir.h"
#include "eqn_toth.h"
#include "eqn_mr_1pvdw.h"
#include "eqn_duhring.h"


SorpPropLib::SorpPropLib()
{
}

/**
	return the eqn with the given name -- lower case
*/
eqn_template *getEqnByName(std::string eqn_name)
{
	eqn_template * eqn = nullptr;

	if (eqn_name == "") {
	}
	else if (eqn_name == "dss") {
		eqn = new eqn_dss;
	}
	else if (eqn_name == "dubinin-astakov-mass") {
		eqn = new eqn_dubininastakov_mass;
	}
	else if (eqn_name == "dubinin-astakov-volume") {
		eqn = new eqn_dubininastakov_volume;
	}
	else if (eqn_name == "duhring") {
		eqn = new eqn_duhring;
	}
	else if (eqn_name == "flory-huggins") {
		eqn = new eqn_ac_floryhuggins;
	}
	else if (eqn_name == "heil") {
		eqn = new eqn_ac_heil;
	}
	else if (eqn_name == "langmuir") {
		eqn = new eqn_langmuir;
	}
	else if (eqn_name == "mixingrule-1pvdw") {
		eqn = new eqn_mr_1pvdw;
	}
	else if (eqn_name == "mixingrule-2pcmr") {
	}
	else if (eqn_name == "mixingrule-vdwb") {
	}
	else if (eqn_name == "nrtl-dg-t") {
		eqn = new eqn_ac_NRTL_Dg_T;
	}
	else if (eqn_name == "nrtl-fixeddg") {
		eqn = new eqn_ac_NRTL_fixdDg;
	}
	else if (eqn_name == "nrtl-multicomp") {
		eqn = new eqn_ac_NRTL_Dg_T;
	}
	else if (eqn_name == "tsuboka-katayama") {
		eqn = new eqn_ac_tsubokakatayama;
	}
	else if (eqn_name == "toth") {
		eqn = new eqn_toth;
	}
	else if (eqn_name == "uniquac-du-t") {
		eqn = new eqn_ac_UNIQUAC_Du_T;
	}
	else if (eqn_name == "uniquac-fixeddu") {
		eqn = new eqn_ac_UNIQUAC_fixedDu;
	}
	else if (eqn_name == "wang-chao") {
		eqn = new eqn_ac_wangchao;
	}
	else if (eqn_name == "wilson") {
		eqn = new eqn_ac_wilson;
	}
	return eqn;
}

/**
	Execute refrigerant-adsorption calculation
*/
std::string calcpair(DATAMAP& pairs, pair_rs *p, double tK, double xMass)
{
	std::ostringstream s;
	for (auto pr : p->eqn_parms) {
		eqn_template *eqn = getEqnByName(pr.first);
		if (eqn != nullptr) {
			double pressure = eqn->calc(pairs, pr.second, tK, xMass, p->getRefName());
			s.flush();
			s << tK << "\t" << xMass << "\t";
			if (pressure<0) {
				s << "N/A";
			}
			else {
				s << pressure;
			}
			s << "\t" << pr.second.getEquation() << "\t" << pr.second.getComment() << "\n";
		}
	}
	return s.str();
}

/**
	decode input, find data pair and execute
*/
std::string SorpPropLib::calc(DATAMAP& pairs, std::string ref, std::string sorb, double tK, double xMass)
{
	// note: all string data is lower case except comments (literature & references)
	std::transform(sorb.begin(), sorb.end(), sorb.begin(), ::tolower);
	std::vector<std::string> srb = split(sorb, ':');
	if (srb.size() < 2) {
		srb.push_back("");//for no-subtype pairs
	}
	
	PK rsKey(ref, srb[0], srb[1]);

	DATAMAP::iterator it = pairs.find(rsKey);
	if (it != pairs.end()) {
		pair_rs *pr = (pair_rs *)pairs[rsKey];

		if (pr->eqn_parms.size() < 1) {
			std::cout << "no equations found for: " << ref << ", " << sorb;
		}
		else {
			std::string rc = calcpair(pairs, pr, tK, xMass);
			std::cout << rc;
			return rc;
		}
	}
	else {
		std::cout << ref << ", " << sorb << " not found\n";
	}
	return "";
}

/**
	check the equation parameters for a pair
*/
bool checkpair(pair_rs *p, std::string& badparms)
{
	bool isOk = true;
	std::ostringstream s;
	s << "\"" << p->getRefName() << "\",\"" << p->getSorpType() << "\",\"" << p->getSubType() << "\":";
	for (auto pr : p->eqn_parms) {
		eqn_template *eqn = getEqnByName(pr.first);
		std::string str;
		if (eqn != nullptr) {
			if (!eqn->check(pr.second, str)) {
				s << "\t" << str;
				isOk = false;
			}
		}
	}
	if (!isOk) {
		s << std::endl;
		badparms = s.str();
	}
	return isOk;
}

/**
	Read a UTF8-encoded csv-formatted equation constants data file 
		containing column headers in the first line.

		The CSV format is as follows:

		Encoding:  The CSV file must be UTF8 encoded.  This can be done using Notepad 
						by opening the file and saving it as UTF8.

		Header row:		first three columns are skipped,
						the last column is the literature citing
						the remaining columns are the parameter names.

		NOTE:  These parameter names must match the parameter names
				  (lower case) hardcoded in the equation header files.
*/
bool SorpPropLib::readCsv(DATAMAP& pairs, std::string equation, std::istream& input)
{
	bool haveFirst = false;
	pair_rs *pr = nullptr;
	std::string line;
	std::vector<std::string> col_names;
	while (std::getline(input, line))
	{
		std::vector<std::string> tokens = tokenize(line, tokens);
		if (!haveFirst)
		{
			for (std::string token : tokens) {
				// note: all string data is lower case except comments (literature & references)
				std::transform(token.begin(), token.end(), token.begin(), ::tolower);
				col_names.push_back(token);
			}
			haveFirst = true;
		}
		else
		{
			//add conversion of ref/sorb to lower case
			if (tokens.size()>2) {
			for (int i : {0, 1, 2}) {
				std::transform(tokens[i].begin(), tokens[i].end(), tokens[i].begin(), ::tolower);
			}

			}
			
			PK pk = PK(tokens[0], tokens[1], tokens[2]);
			DATAMAP::iterator it = pairs.find(pk);
			if (it != pairs.end()) {
				pr = (pair_rs *) pairs[pk];
			}
			else {
				pr = new pair_rs();
				pr->setKey(col_names[0], col_names[1], col_names[2]);
			}
			pr->addData(equation, col_names, tokens);
			pairs[pk] = pr;
		}
	}
	return haveFirst;
}

/**
	load equation constats data from JSON

	The JSON format is an internal object format and will be written on a single line for compactness.
	You can use JSONLint to check and format the JSON: https://jsonlint.com/
*/
bool SorpPropLib::readJson(DATAMAP& pairs, std::istream& input, bool check)
{
	std::map<PK, pair_rs> data;
	typedef std::istream_iterator<char> IT;
	if (nosjob::s11n::load(data, IT(input), IT())) {
		for (auto pair : data) {
			PK pk = pair.first;
			pair_rs *pr = new pair_rs(pair.second);
			if (check) {
				std::string bad;
				if (!checkpair(pr, bad)) {
					std::cout << bad;
				}
			}
			pairs[pk] = pr;
		}
	}
	return false;
}

/**
	save equation constants data to JSON

	The JSON format is an internal object format and will be written on a single line for compactness.
	You can use JSONLint to check and format the JSON: https://jsonlint.com/
*/
bool SorpPropLib::writeJson(DATAMAP& pairs, std::ostream& output, bool check)
{
	std::map<PK, pair_rs> data;
	for (auto pair : pairs) {
		PK pk = pair.first;
		pair_rs *pr = (pair_rs *)pair.second;
		if (pr != nullptr) {
			if (check) {
				std::string bad;
				if (!checkpair(pr, bad)) {
					std::cout << bad;
				}
			}
			data[pr->getKey()] = *pr;
		}
	}
	return nosjob::s11n::save(data, output);
}

// String functions
/**
trim spaces in string from start (in place)
*/
static inline std::string ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !(std::iswspace(ch));
	}));
	return s;
}

/**
trim double quotes in string from start (in place)
*/
static inline std::string ldq(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return (ch != '\"');
	}));
	return s;
}

/**
trim spaces in string from end (in place)
*/
static inline std::string rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !(std::iswspace(ch));
	}).base(), s.end());
	return s;
}

/**
trim double quotes in string from end (in place)
*/
static inline std::string rdq(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return (ch != '\"');
	}).base(), s.end());
	return s;
}

/**
trim string from both ends (in place)
*/
static inline std::string trim(std::string &s) {
	ltrim(s);
	rtrim(s);
	ldq(s);
	rdq(s);
	return s;
}

/**
	split a comma-separated string into tokens
*/
std::vector<std::string> SorpPropLib::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

/**
tokenize an excel csv line which may have comma's inside of double quotes
*/
std::vector<std::string> SorpPropLib::tokenize(const std::string line, const std::vector<std::string> tokens)
{
	bool in_token = false;
	std::vector<std::string> tin = split(line, ',');
	std::vector<std::string> temp;
	std::vector<std::string> tout;
	for (std::string token : tin) {
		if (in_token) {
			temp.push_back(token);
			if (token.find("\"") == (token.length() - 1)) {
				std::string new_token = accumulate(temp.begin(), temp.end(), std::string(""));
				tout.push_back(trim(new_token));
				in_token = false;
			}
		}
		else if (token.find("\"") == 0) {
			temp.push_back(token);
			in_token = true;
		}
		else {
			trim(token);
			tout.push_back(token);
		}
	}
	return tout;
}

/**
	deallocate equation constants data pairs in memory
*/
void SorpPropLib::destroy(DATAMAP& pairs) {
	for (auto pair : pairs) {
		pair_rs *pr = (pair_rs *)pair.second;
		if (pr != nullptr) {
			delete pr;
			pr = nullptr;
		}
	}
}

/**
	add extra data not covered by csv files,
		note: all string data is lower case except comments (literature & references)
*/
void SorpPropLib::addExtra(DATAMAP& pairs)
{
}

