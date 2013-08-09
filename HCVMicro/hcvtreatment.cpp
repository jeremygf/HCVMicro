#include "hcvtreatment.h"

void HCVTreatmentStatus::init(void) 
{
	week = 0;
}

HCVTreatment::HCVTreatment() : hcvcea(0), hcvtreatmentstatus(0)
{

}

HCVTreatment::~HCVTreatment()
{
	delete hcvtreatmentstatus;
}

HCVTreatmentStatus*	HCVTreatment::get_treatment_status(void)
{
	if (hcvtreatmentstatus == 0) {
		hcvtreatmentstatus = new HCVTreatmentStatus();
	}
	return hcvtreatmentstatus;
}

void HCVTreatment::init(CIniFile& INIFile, HCVCEA* phcvcea)
{
	hcvcea = phcvcea;
	hcvtreatmentstatus = get_treatment_status();
}

bool HCVTreatment::start_treatment(RandomBin& rb, int age, int start_age)
{
	return (age == start_age);
}