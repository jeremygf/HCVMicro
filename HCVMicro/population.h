////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Population.h
////
//// Description: A class that 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "person.h"
#include "hcvcea.h"

using namespace std;

class Population
{

public:

	void init(HCVCEA* phcvcea, string& nbaselineprev_path, string& nle_path, string& nmarkovtrace_path, string& ncea_path);
	
	void open_outputfiles();
	void close_outputfiles();

	void register_person(int id, Person& person);
	void record_status(int id, Person& person);
	void record_le(int id, int le);

private:

	void Population::open_outputfile(string& path, ofstream& thestream);

	vector<vector<int>> status_table;

	string baselineprev_path;
	string le_path;
	string markovtrace_path;
	string cea_path;

	ofstream baselineprev_stream;
	ofstream le_stream;
	ofstream markovtrace_stream;
	ofstream cea_stream;

	HCVCEA* hcvcea;
};


#endif
