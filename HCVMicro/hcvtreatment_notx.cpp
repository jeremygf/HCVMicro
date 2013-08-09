#include "hcvtreatment_notx.h"

bool HCVTreatment_NoTx::treatment_update(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white, double tx_mort_rate)
{
	// Do Nothing since people immediately go "off treatment" with the notx strategy
	hcvstatus.treatment = HCVStatus::NOTX;
	return false;
}