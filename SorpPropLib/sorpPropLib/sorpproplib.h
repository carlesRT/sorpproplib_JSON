#ifndef SORPPROPLIB_H
#define SORPPROPLIB_H

#include "sorpproplib_global.h"

#include <iostream>
#include <memory>
#include <map>
#include "IPair_rs.h"

class SORPPROPLIBSHARED_EXPORT SorpPropLib
{

public:
    SorpPropLib();

	// Calculations
	std::string calc(DATAMAP& pairs, std::string ref, std::string sorb, double tK, double xMass);

	// IO Functions
	void addExtra(DATAMAP& pairs);
	bool readCsv(DATAMAP& pairs, std::string equation, std::istream& input);
	bool readJson(DATAMAP& pairs, std::istream& input, bool check = false);
	bool writeJson(DATAMAP& pairs, std::ostream& output, bool check = false);

	// Utility functions
	//   String functions
	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string> SorpPropLib::tokenize(const std::string line, const std::vector<std::string> tokens);
	//   Memory Functions
	void destroy(DATAMAP& pairs);
};

#endif // SORPPROPLIB_H
