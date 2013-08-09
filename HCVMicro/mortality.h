////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Mortality.h
////
//// Description: A class that tracks mortality rates/risks (by age in weeks)
//// stratified by sex, risk status, and HCV status
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef MORTALITY_H
#define MORTALITY_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"

using namespace std;

class Mortality
{

 public:
	void init(string& path);

	double getMortalityRate(int age, bool male, bool white, bool lowrisk, bool hcv);
	bool died(RandomBin& rb, int age, bool male, bool white, bool lowrisk, bool hcv, double excess_rate_hcv, double cure_reduction);

 private:

	ProbData MortalityTable;

};


#endif
