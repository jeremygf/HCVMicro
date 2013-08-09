////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Person.h
////
//// Description: A class that represents aspects of a person and 
//// his/her health etc.
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef PERSON_H
#define PERSON_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "baseline.h"
#include "hcvprogression.h"
#include "hcvtreatment.h"
#include "mortality.h"
#include "hcvstatus.h"
using namespace std;

class Person
{

public:

	typedef enum sex_T			{ MALE = 0, FEMALE = 1 };
	typedef enum race_T			{ WHITE = 0, BLACK = 1, OTHER = 2 };

	Person();

	sex_T sex;
	race_T race;

	HCVRiskStatus HCVRisk;
	HCVStatus HCV;

	int age;
	int start_age;

	bool dead;

	RandomBin*				rb;
	Mortality*				mortality;
	HCVProgression*			hcvprogression;
	HCVTreatment*			hcvtreatment;
	HCVTreatmentStatus*			hcvtreatment_status;

	void init(RandomBin* prb, Mortality* pmort, Baseline* pbase, HCVProgression* phcvprog, HCVTreatment* phcvtreatment, HCVTreatmentStatus* phcvtreatment_status);
	bool is_alive();
	void update();
 
private:

};


#endif
