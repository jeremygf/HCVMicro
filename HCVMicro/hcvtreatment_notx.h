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

#ifndef HCVTREATMENT_NOTX_H
#define HCVTREATMENT_NOTX_H

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

class HCVTreatment_NoTx : public HCVTreatment {
public:
	bool	treatment_update(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white, double tx_mort_rate);
};

#endif