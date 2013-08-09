////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Mortality.cpp
////
//// 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "mortality.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"


void Mortality::init(string& path)
{
	
  ifstream mort_data;
  mort_data.open(path.c_str() );

  if (!mort_data) {   // Check that the file was opened
    cerr << "Unable to open file " << path << endl;
    exit(1);   // call system to stop
  }

  MortalityTable.loadData(mort_data);

  mort_data.close();

}

double Mortality::getMortalityRate(int age, bool male, bool white, bool lowrisk, bool hcv)
{
	int colidx = male*8 + (white==false)*4 + (lowrisk==false)*2 + hcv*1;
	
	double base_rate = MortalityTable.getValue(age, 0);
	double hazard_ratio = MortalityTable.getValue(age, colidx+1);

	return base_rate*hazard_ratio;
}

bool Mortality::died(RandomBin& rb, int age, bool male, bool white, bool lowrisk, bool hcv, double excess_rate_hcv, double cure_reduction)
{
	// Someone has survived to 120 years so they die
	if (age >= MortalityTable.numRows() || age >= 120*52) {
		return true;
	}

	double rate = 0.0;

	double nocure_rate = getMortalityRate(age, male, white, lowrisk, hcv);
	double cure_rate = getMortalityRate(age, male, white, lowrisk, false);
	
	rate = cure_reduction*nocure_rate + (1-cure_reduction)*cure_rate;
	rate += excess_rate_hcv;

	double prob = 1-exp(-1*rate);
	bool happens = rb.rand(RandomBin::Alive) <= prob;
	return happens;
}