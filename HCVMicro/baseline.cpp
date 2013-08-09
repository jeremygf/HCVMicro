
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Mortality.cpp
////
//// 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "baseline.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include "randombin.h"
#include "probdata.h"

void Baseline::reader_helper(string& path, ProbData& table)
{
	ifstream data;
	data.open(path.c_str() );

	if (!data) {   // Check that the file was opened
		cerr << "Unable to open file " << path << endl;
		exit(1);   // call system to stop
	}

	table.loadData(data);

	data.close();
}

void Baseline::init(string& fibrosis_path, string& il28b_path, string& highrisk_path, string& hcv_path, string& genotype_path,
					double nprop_male, double nprop_white, double nprop_nonprogress, double nprop_aware, int nstart_age_yrs)
{
	reader_helper(fibrosis_path, Fibrosis);
	reader_helper(il28b_path, IL28B);
	reader_helper(highrisk_path, HighRisk);
	reader_helper(hcv_path, HCV);
	reader_helper(genotype_path, Genotype);

	prop_male = nprop_male;
	prop_white = nprop_white;
	prop_nonprogress = nprop_nonprogress;
	prop_aware = nprop_aware;
	start_age_yrs = nstart_age_yrs;
}

int	Baseline::get_start_age_weeks(void) 
{
	return start_age_yrs*52;
}

bool Baseline::is_male(RandomBin& rb)
{
	return (rb.rand(RandomBin::Baseline) <= prop_male);
}

bool Baseline::is_white(RandomBin& rb)
{
	return (rb.rand(RandomBin::Baseline) <= prop_white);
}

bool Baseline::is_aware(RandomBin& rb)
{
	return (rb.rand(RandomBin::Baseline) <= prop_aware);
}

bool Baseline::is_nonprogressor(RandomBin& rb, HCVStatus::fibrosis_T fibrosis)
{
	if (fibrosis == HCVStatus::F0) {
		return (rb.rand(RandomBin::Baseline) <= prop_nonprogress);
	} 
	return false;
}

HCVStatus::fibrosis_T Baseline::fibrosis_stage(RandomBin& rb)
{
	double cdf = 0.0;
	HCVStatus::fibrosis_T fibrosis = HCVStatus::F0;
	
	double rand_draw = rb.rand(RandomBin::Baseline);
	for (int i = 0; i < Fibrosis.numRows(); i++) {
		cdf += Fibrosis.getValue(i, 0);
		if (rand_draw <= cdf) {
			fibrosis = static_cast<HCVStatus::fibrosis_T>(i);
			break;
		}
	}

	return fibrosis;
}

HCVRiskStatus::IL28B_T Baseline::il28_type(RandomBin& rb, bool white)
{
	int colidx = 1*(white==false);

	double cdf = 0.0;
	HCVRiskStatus::IL28B_T il28b = HCVRiskStatus::CC;
	
	double rand_draw = rb.rand(RandomBin::Baseline);
	for (int i = 0; i < IL28B.numRows(); i++) {
		cdf += IL28B.getValue(i, colidx);
		if (rand_draw <= cdf) {
			il28b = static_cast<HCVRiskStatus::IL28B_T>(i);
			break;
		}
	}

	return il28b;
}

HCVRiskStatus::risk_T Baseline::risk_type(RandomBin& rb, bool white, bool male)
{
	int colidx = 2*(male) + 1*(white==false);

	double cdf = 0.0;
	HCVRiskStatus::risk_T risk = HCVRiskStatus::LOWRISK;
	
	double rand_draw = rb.rand(RandomBin::Baseline);
	for (int i = 0; i < HighRisk.numRows(); i++) {
		cdf += HighRisk.getValue(i, colidx);
		if (rand_draw <= cdf) {
			risk = static_cast<HCVRiskStatus::risk_T>(i);
			break;
		}
	}

	return risk;
}

bool Baseline::is_hcv(RandomBin& rb, bool white, bool male, bool highrisk)
{
	int colidx = 4*(male) + 2*(white==false) + 1*(highrisk);

	double cdf = 0.0;
	HCVStatus::infected_T hcv = HCVStatus::UNINFECTED;

	double rand_draw = rb.rand(RandomBin::Baseline);
	for (int i = 0; i < HCV.numRows(); i++) {
		cdf += HCV.getValue(i, colidx);
		if (rand_draw <= cdf) {
			hcv = static_cast<HCVStatus::infected_T>(i);
			break;
		}
	}

	return (hcv == HCVStatus::INFECTED);
}

HCVStatus::genotype_T Baseline::hcv_genotype(RandomBin& rb, bool hcv)
{
	if (hcv == true) {
		double cdf = 0.0;
		HCVStatus::genotype_T genotype = HCVStatus::G1;

		double rand_draw = rb.rand(RandomBin::Baseline);
		for (int i = 0; i < Genotype.numRows(); i++) {
			cdf += Genotype.getValue(i, 0);
			if (rand_draw <= cdf) {
				genotype = static_cast<HCVStatus::genotype_T>(i);
				break;
			}
		}

		return genotype;
	} 

	assert(false);
	return HCVStatus::GOther;
}
