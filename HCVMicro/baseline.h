////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Baseline.h
////
//// Description: A class that keeps track of distributions of baseline
//// characteristics of individuals so that new Person objects can be
//// assigned appropriate combinations of these values
//// 
////
//// Shan can modify Jeremy's code
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef BASELINE_H
#define BASELINE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "randombin.h"
#include "probdata.h"
#include "hcvstatus.h"

using namespace std;

class Baseline
{

public:

	void init(string& fibrosis_path, string& il28b_path, string& highrisk_path, string& hcv_path, string& genotype_path,
		double nprop_male, double nprop_white, double nprop_nonprogress, double nprop_aware, int nstart_age_yrs);

	int	get_start_age_weeks(void);
	bool is_male(RandomBin& rb);
	bool is_white(RandomBin& rb);
	bool is_nonprogressor(RandomBin& rb, HCVStatus::fibrosis_T fibrosis);
	bool is_aware(RandomBin& rb);
	HCVStatus::fibrosis_T fibrosis_stage(RandomBin& rb);
	HCVRiskStatus::IL28B_T il28_type(RandomBin& rb, bool white);
	HCVRiskStatus::risk_T risk_type(RandomBin& rb, bool white, bool male);
	bool is_hcv(RandomBin& rb, bool white, bool male, bool highrisk);
	HCVStatus::genotype_T hcv_genotype(RandomBin& rb, bool hcv);

private:

	void Baseline::reader_helper(string& path, ProbData& table);

	ProbData	Fibrosis;
	ProbData	IL28B;
	ProbData	HighRisk;
	ProbData	HCV;
	ProbData	Genotype;

	double		prop_male;
	double		prop_white;
	double		prop_nonprogress;
	double		prop_aware;
	int			start_age_yrs;
};


#endif
