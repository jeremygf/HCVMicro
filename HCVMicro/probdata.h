///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//// Title: probdata.h
////
//// Description: This is a class that produces a table-like data structure to store
//// different probabilities in.
////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifndef PROBDATA_H
#define PROBDATA_H 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

class ProbData
{
protected:
	typedef  vector<double> PCol;
	vector<PCol> ProbMat;

public:
	ProbData();		// Default Constructor
	ProbData(const vector<PCol>&);	// Value Constructor
	ProbData(const ProbData&);		// Copy Constructor


	void 	printData();	// used for debugging, prints to screen
	void 	printData(char * filename);  // used for debugging, prints to file

	void 	loadData(ifstream&);	// reads data from file and loads into ProbMat
	double 	getValue(unsigned Row, unsigned Col);	// Access a single value from ProbMat
	PCol	getCol(unsigned);				// Acces an entire column of ProbMat
	void	setValue(unsigned Row, unsigned Col, double Value);	// Set a single value
	unsigned	numRows (void) {return static_cast<unsigned>(ProbMat[0].size());};	// get number of rows
	unsigned	numCols (void) {return static_cast<unsigned>(ProbMat.size());};	// get number of columns
	bool	checkSize (unsigned NumRows, unsigned NumCols);
	int		cumulativeProbability(double targetProb, unsigned Col=0); // Assumes that data are probabilities of each category and can be summed to get cumulative probability
	void	ratesToProbabilities(void);
	
};

////////////////////////////////////////////////////////////////////////////
//// Class LifeTableData
////
//// A class that provides that provides the cumulative probability of having
//// died by a given age. 
//// NOTE: The life table is conditional, meaning that is is computed 
//// conditional on having survived to StartAge.
//// IMPORTANT NOTE: accessing the data then is done offset from the StartAge
//// so row 0 corresponds to the probability of dying in the first month after start age
//// conditional on having survived to start age.
////

class LifeTableData : public ProbData
{
public:
	static const unsigned MonthsPerYear = 12;
	void	loadData(unsigned StartAge, unsigned MaxMonths, ProbData & ASRMort);
};

////////////////////////////////////////////////////////////////////////////
//// Class MatrixCol
////
//// A class that defines several enumerators of rows for the Natural
//// History probability matrices (probdata objects) in Nugget
////

class MatrixCol
{
public:
	static const unsigned hpv_strata = 4;
		
	// progression matrix columns
	enum progCol {	
//					Healthy_HPV = 0, 
					HPV_CIN1 = 0,
					HPV_CIN23,
					CIN1_CIN23,
					CIN23_CA1,
					CA1_CA2,
					CA2_CA3,
					CA3_CA4,
					NUM_PROG_COL
	};
		
	// regression matrix columns
	enum regCol {	
					CIN23_CIN1 = 0,
					CIN23_HPV,
					CIN23_Healthy,
					CIN1_HPV,
					CIN1_Healthy,
					HPV_Healthy,
					NUM_REG_COL
	};

	// detection matrix columns
	enum detCol {	CA1_CA1d = 0,
					CA2_CA2d = 1,
					CA3_CA3d = 2,
					CA4_CA4d = 3,
					NUM_DET_COL
	};

	// mortality matrix columns
	enum mortCol {	
//					ASR,
					CA1Mort,
					CA2Mort,
					CA3Mort,
					CA4Mort,
					CA1dMort,
					CA2dMort,
					CA3dMort,
					CA4dMort,
					NUM_MORT_COL
	};

	// Test Columns and Rows
	enum TestCol {	NL = 0, CIN1 = 1, CIN23 = 2, CA = 3, NumCytoTestCols, NumHPVTestCols = NumCytoTestCols * 4 + 1, NumColpoTestCols = 16, NumBiopsyCols = 16};
//	enum hpvTestCol	 {	No = 0, Low = 1, High = 2, High16 = 3, High18 = 4};
	enum cytoTestRow {	ASCUS, ACIN23, LSIL, HSIL, Result5, NumCytoPosResults};
	enum hpvTestRow	 {	Negative, LowRisk, HighRisk, HighRisk16, HighRisk18, Result4};

	// Rows for Logistic Coefficients
	enum coefRow {UseMe = 0, Alpha = 0, B_Age, B_AgeSq, B_Dur, B_DurSq, NumCoefficients};

	// Columns for ScreenData and RFData
	enum RFDataCol {Smoke = 0, Hexaparous, OC, NumRFCats};
	enum ScreenBehaviorCat {NeverScreen, ScreenCatA, ScreenCatB, ScreenCatC,
			ScreenCatD, ScreenCatE, NumScreenCats, ScreenStartAge = NumScreenCats};

	enum OverDxCol {CytoNeg = 0, ASCUSTot, ASCUSNeg, ASCUSCIN1, ASCUSCIN23, ASCUSCA,
		LSILTot, LSILNeg, LSILNegToNeg, LSILNegToCIN1, LSILNegToCIN23, LSILNegToCA, LSILCIN1, LSILCIN1ToNeg, LSILCIN1ToCIN1, LSILCIN1ToCIN23, LSILCIN1ToCA, LSILCIN23, LSILCA,
		HSILTot, HSILNeg, HSILCIN1, HSILCIN23, HSILCA, ACIN23Tot, ACIN23Neg, ACIN23CIN1, ACIN23CIN23, ACIN23CA,
		HPVNeg, HPVLR,
		HPVHROTot, HRONeg, HRONegToCA, HRONegToCIN23, HRONegToCIN1, HRONegToNeg, HROCIN1, HROCIN1ToCA, HROCIN1ToCIN23, HROCIN1ToNeg, HROCIN1ToCIN1, HROCIN23, HROCA,
		HPVHR16Tot, HR16Neg, HR16NegToCA, HR16NegToCIN23, HR16NegToCIN1, HR16NegToNeg, HR16CIN1, HR16CIN1ToCA, HR16CIN1ToCIN23, HR16CIN1ToNeg, HR16CIN1ToCIN1, HR16CIN23, HR16CA,
		HPVHR18Tot, HR18Neg, HR18NegToCA, HR18NegToCIN23, HR18NegToCIN1, HR18NegToNeg, HR18CIN1, HR18CIN1ToCA, HR18CIN1ToCIN23, HR18CIN1ToNeg, HR18CIN1ToCIN1, HR18CIN23, HR18CA,
		ColpoTot, ColpoNeg, ColpoCIN1, ColpoCIN23, ColpoCA,
		NumOverDxCol
	};
};

#endif
