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
#include "eqn_nrtl.h"
#include "eqn_toth.h"
#include "eqn_mr_1pvdw.h"


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
std::string calcpair(pair_rs *p, double tK, double xMass)
{
	std::ostringstream s;
	for (auto pr : p->eqn_parms) {
		eqn_template *eqn = getEqnByName(pr.first);
		if (eqn != nullptr) {
			double pressure = eqn->calc(pr.second, tK, xMass, p->getRefName());
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
	
	PK rsKey(ref, srb[0], srb[1]);

	DATAMAP::iterator it = pairs.find(rsKey);
	if (it != pairs.end()) {
		pair_rs *pr = (pair_rs *)pairs[rsKey];

		if (pr->eqn_parms.size() < 1) {
			std::cout << "no equations found for: " << ref << ", " << sorb;
		}
		else {
			std::string rc = calcpair(pr, tK, xMass);
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
	pair_rs *pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "ch3cook");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "ch3ch(oh)coona");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "h2n(ch2)2oh");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "ho(ch2)3oh");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "lino3-lii-licl");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "lii-oh(ch2)3oh");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "libr", "lino3");
	pr->add("duhring", "a0", -2.00755);
	pr->add("duhring", "a1", 0.16976);
	pr->add("duhring", "a2", -3.133362e-3);
	pr->add("duhring", "a3", 1.97668e-5);
	pr->add("duhring", "b0", 321.128);
	pr->add("duhring", "b1", -19.322);
	pr->add("duhring", "b2", 0.374382);
	pr->add("duhring", "b3", -2.0637e-3);
	pr->add("duhring", "n", 1.8);
	pr->add("duhring", "m", 32);
	pr->add("duhring", "r", 6.89476);
	pr->add("duhring", "q", 459.72);
	pr->add("duhring", "c", 6.21147);
	pr->add("duhring", "d", -2886.373);
	pr->add("duhring", "e", -337269.46);
	pairs[pr->getKey()] = pr;

	/*
	pr->setComment("Duhring", "");
	pr->setKeys("water", "licl", "");
	pr->setKeys("water", "cacl2", "");
	pr->setComment("Duhring", "");
	pr->setKeys("water", "zeolite", "5a");
	pr->setKeys("water", "zeolite", "13x");
	pr->setComment("Duhring", "");
	pr->setKeys("water", "silicagel", "");
	*/

	pr = new pair_rs;
	pr->setComment("Duhring", "");
	pr->setKey("water", "naoh", "koh-csoh");
	pr->add("duhring", "a0", 6.164233723);
	pr->add("duhring", "a1", 2.746665026e-1);
	pr->add("duhring", "a2", 4.916023734e-3);
	pr->add("duhring", "a3", 2.859098259e-5);
	pr->add("duhring", "b0", 5.380343163e1);
	pr->add("duhring", "b1", 5.004848451);
	pr->add("duhring", "b2", 1.228273028e-1);
	pr->add("duhring", "b3", 1.096142341e-3);
	pr->add("duhring", "n", 1);
	pr->add("duhring", "m", 0);
	pr->add("duhring", "r", 1);
	pr->add("duhring", "q", 273.15);
	pr->add("duhring", "c", 6.427154896);
	pr->add("duhring", "d", 1208.919437);
	pr->add("duhring", "e", 166159.9630);
	pairs[pr->getKey()] = pr;

	const std::string toth_citing = "Da Silva, F. A., & Rodrigues, A. E. (1999). Adsorption equilibria and kinetics for propylene and propane over 13X and 4A zeolite pellets. Industrial & engineering chemistry research, 38(5), 2051-2057.";

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propylene", "zeolite", "4a");
	pr->add("toth", "q_s", 85.26);
	pr->add("toth", "b_0", 7.4e-6);
	pr->add("toth", "qstar_r", 3.6e3);
	pr->add("toth", "n_0", 0.666);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propylene", "zeolite", "13x");
	pr->add("toth", "q_s", 112.5);
	pr->add("toth", "b_0", 3.5e-7);
	pr->add("toth", "qstar_r", 5.1e3);
	pr->add("toth", "n_0", 0.608);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propylene", "zeolite", "5a-crystal");
	pr->add("toth", "q_s", 168.84);
	pr->add("toth", "b_0", 1.33e-2);
	pr->add("toth", "qstar_r", 1.684e3);
	pr->add("toth", "n_0", 0.4);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 2.5);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propylene", "zeolite", "5a-pellets");
	pr->add("toth", "q_s", 123.06);
	pr->add("toth", "b_0", 2.02e-2);
	pr->add("toth", "qstar_r", 1.612e3);
	pr->add("toth", "n_0", 0.36);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 2.78);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("butene", "zeolite", "13x");
	pr->add("toth", "q_s", 121.8);
	pr->add("toth", "b_0", 2.5e-7);
	pr->add("toth", "qstar_r", 6.543e3);
	pr->add("toth", "n_0", 0.452);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 2.21);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("co2", "carbon", "acf-a-20");
	pr->add("toth", "q_s", 1.56e3);
	pr->add("toth", "b_0", 2.55e-7);
	pr->add("toth", "qstar_r", 2.313e3);
	pr->add("toth", "n_0", 0.696);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("co2", "carbon", "ac-maxsorbiii");
	pr->add("toth", "q_s", 3.06e3);
	pr->add("toth", "b_0", 1.17e-7);
	pr->add("toth", "qstar_r", 2.45e3);
	pr->add("toth", "n_0", 0.664);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("co2", "silicagel", "");
	pr->add("toth", "q_s", 655.6);
	pr->add("toth", "b_0", 5.164e-7);
	pr->add("toth", "qstar_r", 2.330e3);
	pr->add("toth", "n_0", -3.053e-1);
	pr->add("toth", "c", 2.386e2);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("co2", "zeolite", "5a");
	pr->add("toth", "q_s", 642.4);
	pr->add("toth", "b_0", 6.761e-8);
	pr->add("toth", "qstar_r", 5.625e3);
	pr->add("toth", "n_0", 2.7e-1);
	pr->add("toth", "c", -2.002e1);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("co2", "zeolite", "13x");
	pr->add("toth", "q_s", 585.2);
	pr->add("toth", "b_0", 4.884e-4);
	pr->add("toth", "qstar_r", 2.991e3);
	pr->add("toth", "n_0", 7.487e-2);
	pr->add("toth", "c", 3.805e1);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("hfc134a", "carbon", "ac-maxsorbiii");
	pr->add("toth", "q_s", 4.32e3);
	pr->add("toth", "b_0", 3.51e-6);
	pr->add("toth", "qstar_r", 3.27e3);
	pr->add("toth", "n_0", 0.321);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("hfo1234ze", "carbon", "ac-maxsorbiii");
	pr->add("toth", "q_s", 3.74e3);
	pr->add("toth", "b_0", 1.3e-6);
	pr->add("toth", "qstar_r", 3.685e3);
	pr->add("toth", "n_0", 0.295);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propane", "carbon", "molecularsieve");
	pr->add("toth", "q_s", 77.308);
	pr->add("toth", "b_0", 1.81e-2);
	pr->add("toth", "qstar_r", 1.378e3);
	pr->add("toth", "n_0", 0.356);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 2.81);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propane", "zeolite", "4a");
	pr->add("toth", "q_s", 89.32);
	pr->add("toth", "b_0", 6e-4);
	pr->add("toth", "qstar_r", 0);
	pr->add("toth", "n_0", 1);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propane", "zeolite", "13x");
	pr->add("toth", "q_s", 117.92);
	pr->add("toth", "b_0", 3.5e-7);
	pr->add("toth", "qstar_r", 4.3e3);
	pr->add("toth", "n_0", 0.58);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1);
	pr->add("toth", "r", -1);       //if p = n
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propane", "zeolite", "5a-crystal");
	pr->add("toth", "q_s", 160.16);
	pr->add("toth", "b_0", 4.3e-3);
	pr->add("toth", "qstar_r", 1.828e3);
	pr->add("toth", "n_0", 0.46);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 2.17);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propane", "zeolite", "5a-pellets");
	pr->add("toth", "q_s", 114.84);
	pr->add("toth", "b_0", 4.94e-4);
	pr->add("toth", "qstar_r", 2.393e3);
	pr->add("toth", "n_0", 0.58);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 1.72);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;

	pr = new pair_rs;
	pr->setComment("Toth", toth_citing);
	pr->setKey("propylene", "carbon", "molecularsieve");
	pr->add("toth", "q_s", 80.934);
	pr->add("toth", "b_0", 1.32e-2);
	pr->add("toth", "qstar_r", 1.726e3);
	pr->add("toth", "n_0", 0.325);
	pr->add("toth", "c", 0);
	pr->add("toth", "m", 3.08);
	pr->add("toth", "r", 1);
	pairs[pr->getKey()] = pr;
}

