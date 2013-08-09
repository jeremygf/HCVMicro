////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: hcvtreatment.h
////
//// Description: A class that defines a generic abstract treatment class.
//// It is intended to define an interace that subclasses will implement.
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef HCVTREATMENT_H
#define HCVTREATMENT_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "iniFile.h"
#include "randombin.h"
#include "probdata.h"
#include "hcvstatus.h"
#include "hcvcea.h"

using namespace std;

class HCVTreatmentStatus {
public:
	virtual void	init(void);

	int week;
};

class HCVTreatment {

public:

	HCVTreatment();
	~HCVTreatment();

	virtual	void					init(CIniFile& INIFile, HCVCEA* phcvcea);
	virtual HCVTreatmentStatus*		get_treatment_status(void);
	virtual bool					treatment_update(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white, double tx_mort_rate)	{ return false; };
	virtual bool					start_treatment(RandomBin& rb, int age, int start_age);

protected:

	HCVCEA*					hcvcea;

private:

	HCVTreatmentStatus *	hcvtreatmentstatus;

};

#endif