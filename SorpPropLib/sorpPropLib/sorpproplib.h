#ifndef SORPPROPLIB_H
#define SORPPROPLIB_H

#include <iostream>
#include <memory>
#include <map>
#include "IPair_rs.h"

class SorpPropLib
{

public:
	__declspec(dllexport)  SorpPropLib();

	// Calculations
	__declspec(dllexport)  std::string calc(DATAMAP& pairs, std::string ref, std::string sorb, double tK, double xMass);
	// IO Functions
	__declspec(dllexport)  void addExtra(DATAMAP& pairs);
	__declspec(dllexport)  bool readCsv(DATAMAP& pairs, std::string equation, std::istream& input);
	__declspec(dllexport)  bool readJson(DATAMAP& pairs, std::istream& input, bool check = false);
	__declspec(dllexport)  bool writeJson(DATAMAP& pairs, std::ostream& output, bool check = false);

	// Utility functions
	//   String functions
	template<typename Out>
	__declspec(dllexport)  void split(const std::string &s, char delim, Out result) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}
	__declspec(dllexport)  std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string> SorpPropLib::tokenize(const std::string line, const std::vector<std::string> tokens);
	//   Memory Functions
	__declspec(dllexport)  void destroy(DATAMAP& pairs);
};

#endif // SORPPROPLIB_H
