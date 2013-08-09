////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: hcvprogression.h
////
//// Description: A class that handles rules for natural history of HCV
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef HCVPROGRESSION_H
#define HCVPROGRESSION_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"
#include "hcvstatus.h"
#include "hcvcea.h"

using namespace std;

class HCVProgression
{

 public:
	struct HCVProgressionData {
		double rate_remission;
		double rate_F4_DC;
		double rate_F4DC_HCC;
		double rate_F4TX_HCC;
		double rate_DC_LT;
		double rate_HCC_LT;
		double rate_mort_LT;
		double rate_mort_postLT;
		double rate_mort_DC;
		double rate_mort_HCCfirstyear;
		double rate_mort_HCC2plusyears;
		double rate_mort_Tx;
		double factor_redux_Cure;
		double prob_mort_liverbiopsy;
		double limit_maxLTage_yrs;
	};

	void init(string& path, HCVProgressionData& nprogdata, HCVCEA* hcvcea);
	bool progress(RandomBin& rb, int age, bool male, bool white, bool lowrisk, HCVStatus& hcvstatus);
	double additional_mortality(HCVStatus& hcvstatus);
	double cure_reduction(HCVStatus& hcvstatus);
	double treatment_mortality(void); 

 private:


	HCVCEA*				hcvcea;
	ProbData			FibrosisProgressionTable;
	HCVProgressionData	ProgressionData;
};


#endif
