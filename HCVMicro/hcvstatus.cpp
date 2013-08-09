#include "hcvstatus.h"

HCVStatus::HCVStatus(void) : infected(HCVStatus::UNINFECTED), genotype(HCVStatus::GNONE), fibrosis(HCVStatus::NOFIB), 
								progressor(HCVStatus::PNONE), treatment(HCVStatus::NOTX), aware(HCVStatus::ANONE), HCCtime(0), LTtime(0)
{

}

HCVRiskStatus::HCVRiskStatus(void) : il28b(HCVRiskStatus::CC), risk(HCVRiskStatus::LOWRISK)
{

}