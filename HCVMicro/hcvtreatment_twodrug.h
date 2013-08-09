////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: hcvtreatment_notx.h
////
//// Description: A class that implements no treatment strategy.
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef HCVTREATMENT_TWODRUG_H
#define HCVTREATMENT_TWODRUG_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"
#include "hcvstatus.h"
#include "hcvtreatment.h"

using namespace std;

class HCVTreatment_TwoDrug : public HCVTreatment {

public:
	void					init(CIniFile& INIFile, HCVCEA* phcvcea);
	bool					treatment_update(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white, double tx_mort_rate);

private:

	void					process_ini_file(CIniFile& INIFile);
	void					process_G1(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white);
	void					process_G2(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white);
	void					process_G3(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white);

	double					rate_mort_Tx;
	double					rate_quit_Tx;
	double					cost_Tx_wk;
	double					cost_Tx_sideeffects;
	double					qol_Tx_decrement_wk;
	ProbData				EffectivenessTable;
};

#endif