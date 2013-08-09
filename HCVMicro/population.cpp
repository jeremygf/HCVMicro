///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Population.cpp
////
//// 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "population.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;

void Population::init(HCVCEA* phcvcea, string& nbaselineprev_path, string& nle_path, string& nmarkovtrace_path, string& ncea_path)
{
	hcvcea = phcvcea;
	
	baselineprev_path = nbaselineprev_path;
	le_path = nle_path;
	markovtrace_path = nmarkovtrace_path;
	cea_path = ncea_path;

	vector<int> row;
	row.resize(static_cast<int>(HCVStatus::FDEAD) + 2, 0);
	status_table.resize(120*52+1, row);
}

void Population::register_person(int id, Person& person)
{
	baselineprev_stream << id << "\t" << person.sex << "\t" << person.race <<
				"\t" << person.HCVRisk.il28b << "\t" << person.HCVRisk.risk <<
				"\t" << person.HCV.infected << "\t" << person.HCV.genotype <<
				"\t" << person.HCV.fibrosis << "\t" << person.HCV.progressor <<	
				"\t" << person.HCV.aware <<
				endl;
}

void Population::record_le(int id, int le) {
	le_stream << id << "\t" << le << endl;

	for (int tmp_le = le+1; tmp_le < status_table.size(); tmp_le++) {
		status_table[tmp_le][static_cast<int>(HCVStatus::FDEAD)+1]++;
	}
}

void Population::record_status(int id, Person& person) {
	status_table[person.age][static_cast<int>(person.HCV.fibrosis)+1]++;
}

void Population::open_outputfile(string& path, ofstream& thestream)
{
  thestream.open( path.c_str() ); 
  if (!thestream) {   // Check that the file was opened
    cerr << "Unable to open file " << path;
    exit(1);   // call system to stop
  }
}

void Population::open_outputfiles()
{
	open_outputfile(baselineprev_path, baselineprev_stream);
	open_outputfile(le_path, le_stream);
	open_outputfile(markovtrace_path, markovtrace_stream);
	open_outputfile(cea_path, cea_stream);
}

void Population::close_outputfiles()
{
	for (int row = 0; row < status_table.size(); row++) {
		markovtrace_stream << row << "\t";
		for (int col = 0; col < status_table[row].size(); col++) {
			markovtrace_stream << status_table[row][col] << "\t";
		}
		markovtrace_stream << endl;
	}

	cea_stream << hcvcea->get_discounted_costs() << "\t"
				<< hcvcea->get_discounted_qalys() << "\t" 
				<< hcvcea->get_undiscounted_costs() << "\t"
				<< hcvcea->get_undiscounted_qalys() << endl;

	baselineprev_stream.close();
	le_stream.close();
	markovtrace_stream.close();
	cea_stream.close();
}
