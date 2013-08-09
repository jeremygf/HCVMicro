///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Person.cpp
////
//// 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "person.h"
#include "randombin.h"
#include "mortality.h"

Person::Person(void) : dead(false), age(0), start_age(0), sex(Person::FEMALE), race(Person::WHITE), 
						rb(0), mortality(0), hcvprogression(0)
{

}

void Person::init(RandomBin* prb, Mortality* pmort, Baseline* pbase, HCVProgression* phcvprog, 
				  HCVTreatment* phcvtreatment, HCVTreatmentStatus* phcvtreatment_status)
{
	rb						= prb;
	mortality				= pmort;
	hcvprogression			= phcvprog;
	hcvtreatment			= phcvtreatment;
	hcvtreatment_status		= phcvtreatment_status;

	age = pbase->get_start_age_weeks();
	start_age = age;

	if ( pbase->is_white(*rb) ) {
		race = Person::WHITE;
	} else {
		race = Person::BLACK;
	}

	if ( pbase->is_male(*rb) ) {
		sex = Person::MALE;
	} else {
		sex = Person::FEMALE;
	}

	if ( pbase->is_aware(*rb) ) {
		HCV.aware = HCVStatus::AWARE;
	} else {
		HCV.aware = HCVStatus::UNAWARE;
	}

	HCVRisk.il28b = pbase->il28_type(*rb, race == Person::WHITE);
	HCVRisk.risk = pbase->risk_type(*rb, race == Person::WHITE, sex == Person::MALE);
	
	if ( pbase->is_hcv(*rb, race == Person::WHITE, sex == Person::MALE, HCVRisk.risk == HCVRiskStatus::HIGHRISK) ) {
		HCV.infected = HCVStatus::INFECTED;
	} else {
		HCV.infected = HCVStatus::UNINFECTED;
	}

	if ( HCV.infected == HCVStatus::INFECTED ) {
		HCV.fibrosis = pbase->fibrosis_stage(*rb);
		if (HCV.fibrosis == HCVStatus::F0) {
			if ( pbase->is_nonprogressor(*rb, HCV.fibrosis) ) {
				HCV.progressor = HCVStatus::NONPROGRESSOR;
			} else {
				HCV.progressor = HCVStatus::PROGRESSOR;
			}
		} else {
			HCV.progressor = HCVStatus::PROGRESSOR;
		}
		HCV.genotype = pbase->hcv_genotype(*rb, HCV.infected == HCVStatus::INFECTED);
	}
}

bool Person::is_alive()
{
	if (dead) return false;
	dead = mortality->died(*rb, age, sex==Person::MALE, race==Person::WHITE, HCVRisk.risk==HCVRiskStatus::LOWRISK, (HCV.infected==HCVStatus::INFECTED || HCV.infected==HCVStatus::PREVINFECTED), hcvprogression->additional_mortality(HCV), hcvprogression->cure_reduction(HCV));
	return dead==false;
}

void Person::update()
{
	if (hcvtreatment->start_treatment(*rb, age, start_age)) {
		HCV.treatment = HCVStatus::ONGOING;
	}
	dead = hcvprogression->progress(*rb, age, sex==Person::MALE, race==Person::WHITE, HCVRisk.risk==HCVRiskStatus::LOWRISK, HCV);
	if (HCV.treatment == HCVStatus::ONGOING) {
		dead = hcvtreatment->treatment_update(*rb, age, *hcvtreatment_status, HCV, HCVRisk, sex==Person::MALE, race==Person::WHITE, hcvprogression->treatment_mortality());
	}
	age++;
}

