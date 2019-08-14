#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "sorpproplib.h"
#include "pair_rs.h";

namespace fs = ::std::experimental::filesystem;

bool ReadCsvFile(SorpPropLib *sLib, std::string fullname, DATAMAP& pairs);
int ReadCsvFolder(SorpPropLib *sLib, std::string pathspec, DATAMAP& pairs);
int ProcessFile(SorpPropLib *sLib, DATAMAP pairs, std::ifstream& iFile, std::string outputFilename);
int ProcessSinglePoint(SorpPropLib *sLib, DATAMAP pairs);

int main(int argc, char *argv[])
{
	DATAMAP pairs;
	SorpPropLib *sLib = new SorpPropLib;
	if (!sLib)
	{
		std::cerr << "Cannot open the SorpPropLib." << std::endl;
		return -2;
	}

	std::string fullname = (argc > 1) ? argv[1] : "input.txt";
	std::string outname = (argc > 2) ? argv[2] : "output.txt";
	std::string dataname = (argc > 3) ? argv[2] : "sorpproplib.json";

	
	/**
		If we're given a folder as the first argument, 
			then read the csv file(s) and write the Json data file.
		Otherwise we'll process the input text file.
	*/
	fs::path arg1(fullname);
	if (fs::is_directory(arg1)) {

		sLib->addExtra(pairs);
		ReadCsvFolder(sLib, fullname, pairs);
		std::ofstream oFile(dataname);
		sLib->writeJson(pairs, oFile, true);
		oFile.flush();
		oFile.close();
	}
	else {

		std::ifstream iData(dataname);
		sLib->readJson(pairs, iData);
		iData.close();

		std::ifstream iFile(fullname);
		if (!iFile) {
			std::cout << "single point";

			ProcessSinglePoint(sLib, pairs);
		}
		else {
			std::cout << "process file";

			ProcessFile(sLib, pairs, iFile, outname);
		}
	}


	std::cout << "Press enter to exit.";
	std::getchar();

	sLib->destroy(pairs);
	delete sLib;

	return 0;
}

int ProcessFile(SorpPropLib *sLib, DATAMAP pairs, std::ifstream& iFile, std::string outputFilename) {

	int count = 0;
	bool doWrite;
	char *e;
	double val = 0;
	std::string line, ref, sorb, str;
	double tK, xMass;
	std::vector<std::string> strs;

	std::ofstream oFile(outputFilename, std::ofstream::out | std::ofstream::trunc);
	if (!oFile) {
		oFile.open(outputFilename, std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
	}
	doWrite = (bool)oFile;

	while (!iFile.eof()) {

		std::getline(iFile, line);

		if (line.find_first_of("ref/sorb") == 0) {

			if (doWrite) {

				oFile << line << '\n';
				oFile << "t[K]\tmass ratio\tp[kPa]\tequation form\treference\n";
			}

			strs = sLib->split(line, '\t');

			ref = strs[1];
			sorb = strs[2];

			std::cout << "beginning!" << std::endl;
			std::cout << "ref: " << ref << " sorb: " << sorb << "\n";
			std::cout << "t[K]\tmass ratio\tpressure[kPa]\tequation form\treference\n";
		}
		else if (isdigit(line.at(0))) {

			strs = sLib->split(line, '\t');

			errno = 0;
			val = std::strtod(strs[0].c_str(), &e);
			if ((*e != '\0') || (errno != 0))
			{
				std::cout << "Could not convert tK: " << strs[0];
			}
			tK = val;

			errno = 0;
			val = std::strtod(strs[1].c_str(), &e);
			if ((*e != '\0') || (errno != 0))
			{
				std::cout << "Could not convert xMass: " << strs[1];
			}
			xMass = val;

			str = sLib->calc(pairs, ref, sorb, tK, xMass);
			if (doWrite) {
				oFile << str;
			}
		}
	}
	if (doWrite) {
		oFile.flush();
		oFile.close();
	}
	return count;
}

int ProcessSinglePoint(SorpPropLib *sLib, DATAMAP pairs) {

	int count = 0;
	std::string ref, sorb;
	double tK, xMass;
	for (char exit = 'n'; exit != 'y';) {

		std::cout << "input file not found,\nstart single point inquiry:\n";

		std::cout << "\n\nPlease enter the refrigerant, sorbate, temperature[k], and mass fraction of sorbate[0.01].\n";

		std::cin >> ref >> sorb >> tK >> xMass;

		std::cout << "temperature[K]\tmass ratio\tpressure[kPa]\tequation form\treference\n";

		sLib->calc(pairs, ref, sorb, tK, xMass);

		std::cout << "\nexit?[y/n]\n";
		std::cin >> exit;
	}
	return count;
}

//
// CSV file data entry (convert to JSON)
//

/**
	Load equation constants from a utf-encoded csv file
*/
bool ReadCsvFile(SorpPropLib *sLib, std::string fullname, DATAMAP& pairs)
{
	std::string filename = fullname;
	size_t path_posn = fullname.find_last_of("\\");
	if (path_posn >= 0)
	{
		filename = fullname.substr(path_posn + 1);
	}
	std::string equation = filename;
	size_t extn_posn = filename.find_last_of(".");
	if (extn_posn >= 0) {
		equation = filename.substr(0, extn_posn);
	}
	std::ifstream iFile(fullname);
	if ((iFile) && (iFile.is_open())) {
		bool isOk = sLib->readCsv(pairs, equation, iFile);
		iFile.close();
		return isOk;
	}
	return false;
}

/**
	 return the filenames of all files that have the specified extension
	 in the specified directory and all subdirectories
*/

void get_all_files_with_extn(const std::string& pathspec, const std::string& ext, std::vector<std::string>& file_list)
{
	for (const auto& p : fs::directory_iterator(pathspec))
	{
		if (p.path().extension() == ext)
		{
			file_list.push_back(p.path().u8string());
		}
	}
}

/**
	Load all equation constants from utf-encoded csv files in a folder
*/
int ReadCsvFolder(SorpPropLib *sLib, std::string pathspec, DATAMAP& pairs)
{
	int count = 0;
	std::vector<std::string> file_list;
	get_all_files_with_extn(pathspec, ".csv", file_list);
	if (file_list.size() > 0) {
		for (std::string fullname : file_list) {
			std::cout << fullname;
			if (ReadCsvFile(sLib, fullname, pairs)) {
				std::cout << " processed\n";
				count++;
			}
			else {
				std::cout << " ERROR\n";
			}
		}
		std::cout << count << " files processed\n";
	}
	else {
		std::cout << "no files found\n";
	}
	return count;
}
