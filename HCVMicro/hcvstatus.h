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

#ifndef HCVSTATUS_H
#define HCVSTATUS_H

using namespace std;

class HCVStatus {

public:

	typedef enum infected_T		{ UNINFECTED = 0, INFECTED = 1, PREVINFECTED = 2 };
	typedef enum genotype_T		{ GNONE = -1, G1 = 0, G2 = 1, G3 = 2, GOther = 3 };
	typedef enum fibrosis_T		{ NOFIB = -1, F0 = 0, F1 = 1, F2 = 2, F3 = 3, F4 = 4, DC = 5, HCC = 6, LIVER_TRANSPLANT = 7, FDEAD = 8};
	typedef enum progressor_T	{ PNONE = -1, PROGRESSOR = 0, NONPROGRESSOR = 1 };
	typedef enum treatment_T	{ NOTX = 0, ONGOING = 1, SUCCESS = 2, FAIL = 3 };
	typedef enum aware_T		{ ANONE = -1, UNAWARE = 0, AWARE = 1 };


	HCVStatus();

	infected_T		infected;
	genotype_T		genotype;
	fibrosis_T		fibrosis;
	progressor_T	progressor;
	treatment_T		treatment;
	aware_T			aware;

	int				HCCtime;
	int				LTtime;
};

class HCVRiskStatus {
public:

	typedef enum IL28B_T		{ CC = 0, CT = 1, TT = 2 }; 
	typedef enum risk_T			{ LOWRISK = 0, HIGHRISK = 1 };

	HCVRiskStatus();

	IL28B_T il28b;
	risk_T risk;
};

#endif