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

#include "hcvprogression.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"
#include "hcvcea.h"


void HCVProgression::init(string& path, HCVProgressionData& nprogdata, HCVCEA* phcvcea)
{
	
  ifstream fibprog_data;
  fibprog_data.open(path.c_str() );

  if (!fibprog_data) {   // Check that the file was opened
    cerr << "Unable to open file " << path << endl;
    exit(1);   // call system to stop
  }

  FibrosisProgressionTable.loadData(fibprog_data);

  fibprog_data.close();

  ProgressionData = nprogdata;

  hcvcea = phcvcea;
}

bool HCVProgression::progress(RandomBin& rb, int age, bool male, bool white, bool lowrisk, HCVStatus& hcvstatus)
{
	hcvcea->record_state(age, hcvstatus);

	if (hcvstatus.infected != HCVStatus::INFECTED || hcvstatus.treatment == HCVStatus::SUCCESS) {
		return false;
	}

	if (hcvstatus.fibrosis == HCVStatus::F0) {
		if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*ProgressionData.rate_remission) ) {
			hcvstatus.fibrosis = HCVStatus::NOFIB;
			hcvstatus.infected = HCVStatus::PREVINFECTED;
			hcvstatus.genotype = HCVStatus::GNONE;
			hcvstatus.treatment = HCVStatus::NOTX;
			hcvstatus.progressor = HCVStatus::PNONE;
			return false;
		}
	}

	if (hcvstatus.progressor == HCVStatus::PROGRESSOR) {
		if (hcvstatus.fibrosis < HCVStatus::F4) {
			int colidx = (male==0)*1;
			double rate = FibrosisProgressionTable.getValue(age, colidx);
			double prob = 1-exp(-1*rate);
			if (rb.rand(RandomBin::Progression) <= prob) {
				if (hcvstatus.fibrosis == HCVStatus::F0) {
					hcvstatus.fibrosis = HCVStatus::F1;
				} else if (hcvstatus.fibrosis == HCVStatus::F1) {
					hcvstatus.fibrosis = HCVStatus::F2;
				} else if (hcvstatus.fibrosis == HCVStatus::F2) {
					hcvstatus.fibrosis = HCVStatus::F3;
				} else if (hcvstatus.fibrosis == HCVStatus::F3) {	
					hcvstatus.fibrosis = HCVStatus::F4;
				}
			}
			return false;
		} else if (hcvstatus.fibrosis == HCVStatus::F4) {
			if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*(ProgressionData.rate_F4_DC + ProgressionData.rate_F4DC_HCC)) ) {
				if ( rb.rand(RandomBin::Progression) < (ProgressionData.rate_F4_DC / (ProgressionData.rate_F4_DC + ProgressionData.rate_F4DC_HCC)) ) {
					hcvstatus.fibrosis = HCVStatus::DC;
				} else {
					hcvstatus.fibrosis = HCVStatus::HCC;
					hcvstatus.HCCtime = 0;
				}
			}
			return false;
		} else if (hcvstatus.fibrosis == HCVStatus::DC) {
			double rate_LT = 0.0;
			if ( age < (ProgressionData.limit_maxLTage_yrs*52) ) {
				rate_LT = ProgressionData.rate_DC_LT;
			}
			
			if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*(ProgressionData.rate_F4DC_HCC + rate_LT)) ) {
				if ( rb.rand(RandomBin::Progression) < (rate_LT / (rate_LT + ProgressionData.rate_F4DC_HCC)) ) {
					hcvstatus.fibrosis = HCVStatus::LIVER_TRANSPLANT;
					hcvstatus.LTtime = 0;
					if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*ProgressionData.rate_mort_LT) ) {
						return true;
					}
				} else {
					hcvstatus.fibrosis = HCVStatus::HCC;
					hcvstatus.HCCtime = 0;
				}
			}
			return false;
		} else if (hcvstatus.fibrosis == HCVStatus::HCC) {
			hcvstatus.HCCtime++;		
			if ( age < (ProgressionData.limit_maxLTage_yrs*52) ) {
				if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*ProgressionData.rate_HCC_LT) ) {
					hcvstatus.fibrosis = HCVStatus::LIVER_TRANSPLANT;
					hcvstatus.LTtime = 0;
					if ( rb.rand(RandomBin::Progression) <= 1-exp(-1*ProgressionData.rate_mort_LT) ) {
						return true;
					}
				}
			}
			return false;
		} else if (hcvstatus.fibrosis == HCVStatus::LIVER_TRANSPLANT) {
			hcvstatus.LTtime++;
		}
	}
	return false;
}

double HCVProgression::additional_mortality(HCVStatus& hcvstatus)
{
	double excess_rate = 0.0;
	if (hcvstatus.fibrosis == HCVStatus::LIVER_TRANSPLANT && hcvstatus.LTtime<13) {
		excess_rate = ProgressionData.rate_mort_LT;
	} else if (hcvstatus.fibrosis == HCVStatus::LIVER_TRANSPLANT && hcvstatus.LTtime>=13) {
		excess_rate = ProgressionData.rate_mort_postLT;
	} else if (hcvstatus.fibrosis == HCVStatus::DC) {
		excess_rate = ProgressionData.rate_mort_DC;
	} else if (hcvstatus.fibrosis == HCVStatus::HCC && hcvstatus.HCCtime<52) {
		excess_rate = ProgressionData.rate_mort_HCCfirstyear;
	} else if (hcvstatus.fibrosis == HCVStatus::HCC && hcvstatus.HCCtime>=52) {
		excess_rate = ProgressionData.rate_mort_HCC2plusyears;
	}
	return excess_rate;
}

double HCVProgression::cure_reduction(HCVStatus& hcvstatus)
{
	if (hcvstatus.treatment == HCVStatus::SUCCESS) {
		return ProgressionData.factor_redux_Cure;
	} else {
		return 1.0;
	}
}

double HCVProgression::treatment_mortality(void)
{
	return ProgressionData.rate_mort_Tx;
}