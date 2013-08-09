////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: hcvcea.cpp
////
//// 
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "hcvcea.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include "randombin.h"
#include "probdata.h"


void HCVCEA::init(double nannual_discount_rate, int start_age_yrs, 
				  string& age_costs_utilities_path, string& hcv_state_costs_path, string& hcv_state_utilities_path,
				  double nqol_hcvaware_decrement, double nqol_LT_decrement,
				  double ncost_recover_F0F3, double ncost_recover_F4,
				  double nqol_SVR_F0, double nqol_SVR_F1, double nqol_SVR_F2, double nqol_SVR_F3, double nqol_SVR_F4)
{
	undiscounted_costs = 0.0;
	undiscounted_qalys = 0.0;
	discounted_costs = 0.0;
	discounted_qalys = 0.0;
	
	annual_discount_rate = nannual_discount_rate;
	start_age_weeks = start_age_yrs*52;

	qol_hcvaware_decrement = nqol_hcvaware_decrement;
	qol_LT_decrement = nqol_LT_decrement;

	qol_SVR_F0 = nqol_SVR_F0;
	qol_SVR_F1 = nqol_SVR_F1;
	qol_SVR_F2 = nqol_SVR_F2;
	qol_SVR_F3 = nqol_SVR_F3;
	qol_SVR_F4 = nqol_SVR_F4;

	cost_recover_F0F3 = ncost_recover_F0F3;
	cost_recover_F4 = ncost_recover_F4;

	ifstream acu_data;
	acu_data.open(age_costs_utilities_path.c_str() );

	if (!acu_data) {   // Check that the file was opened
		cerr << "Unable to open file " << age_costs_utilities_path << endl;
		exit(1);   // call system to stop
	}

	AgeCostsUtilitiesTable.loadData(acu_data);

	acu_data.close();

	ifstream hsc_data;
	hsc_data.open(hcv_state_costs_path.c_str() );

	if (!hsc_data) {   // Check that the file was opened
		cerr << "Unable to open file " << hcv_state_costs_path << endl;
		exit(1);   // call system to stop
	}

	HCVStateCostsTable.loadData(hsc_data);

	hsc_data.close();

	ifstream hsu_data;
	hsu_data.open(hcv_state_utilities_path.c_str() );

	if (!hsu_data) {   // Check that the file was opened
		cerr << "Unable to open file " << hcv_state_utilities_path << endl;
		exit(1);   // call system to stop
	}

	HCVStateUtilitiesTable.loadData(hsu_data);

	hsu_data.close();

	DiscountFactors.resize( (120*52)+1, 0.0 );
	for (int i = 0; i < DiscountFactors.size(); i++) {
		if (i < start_age_weeks) {
			DiscountFactors[i] = 0;
		} else {
			DiscountFactors[i] = 1.0 / pow( (1.0 + annual_discount_rate) , ( ( (i - start_age_weeks) + 0.5 ) / 52.0 ) );
		}
	}
}

void HCVCEA::record_state(int age, HCVStatus& hcvstatus)
{
	double discount_factor = DiscountFactors[age];

	double age_cost = AgeCostsUtilitiesTable.getValue(age, 0);
	double age_qol = AgeCostsUtilitiesTable.getValue(age, 1);

	double decrement_val = 0.0;
	double state_cost = 0.0; 
	double state_qaly = 0.0;

	if (hcvstatus.treatment != HCVStatus::SUCCESS) {

		if (hcvstatus.aware == HCVStatus::AWARE && hcvstatus.infected == HCVStatus::INFECTED && hcvstatus.fibrosis <= HCVStatus::F4 && hcvstatus.fibrosis >= HCVStatus::F0 ) {
			decrement_val += (qol_hcvaware_decrement/52);
		}
		int colidx = (hcvstatus.aware == HCVStatus::AWARE)*1;
		int rowidx = 0;
		if ( hcvstatus.infected != HCVStatus::INFECTED) {
			rowidx = 0;
		} else 	if ( hcvstatus.fibrosis >= HCVStatus::F0 && hcvstatus.fibrosis < HCVStatus::HCC ) {
			rowidx = static_cast<int>(hcvstatus.fibrosis) + 1;
		} else if ( hcvstatus.fibrosis == HCVStatus::HCC ) {
			if (hcvstatus.HCCtime >= 52) {
				rowidx = static_cast<int>(HCVStatus::DC) + 3;
			} else {
				rowidx = static_cast<int>(HCVStatus::DC) + 2;
			}
		} else if ( hcvstatus.fibrosis == HCVStatus::LIVER_TRANSPLANT ) {
			if (hcvstatus.LTtime >= 13) {
				rowidx = static_cast<int>(HCVStatus::DC) + 5;
			} else {
				rowidx = static_cast<int>(HCVStatus::DC) + 4;
				decrement_val += (qol_LT_decrement/52);
			}
		}

		state_cost = HCVStateCostsTable.getValue(rowidx, colidx);
		state_qaly = HCVStateUtilitiesTable.getValue(rowidx, 0);

	} else if (hcvstatus.treatment == HCVStatus::SUCCESS) {
		if (hcvstatus.fibrosis == HCVStatus::F0) {
			state_cost = cost_recover_F0F3;
			state_qaly = qol_SVR_F0;
		} else if (hcvstatus.fibrosis == HCVStatus::F1) {
			state_cost = cost_recover_F0F3;
			state_qaly = qol_SVR_F1;
		} else if (hcvstatus.fibrosis == HCVStatus::F2) {
			state_cost = cost_recover_F0F3;
			state_qaly = qol_SVR_F2;
		} else if (hcvstatus.fibrosis == HCVStatus::F3) {
			state_cost = cost_recover_F0F3;
			state_qaly = qol_SVR_F3;
		} else if (hcvstatus.fibrosis == HCVStatus::F4) {
			state_cost = cost_recover_F4;
			state_qaly = qol_SVR_F4;
		}
	}

	undiscounted_qalys += ((age_qol * state_qaly) + decrement_val);
	undiscounted_costs += (age_cost + state_cost);

	discounted_qalys += (((age_qol * state_qaly) + decrement_val) * discount_factor);
	discounted_costs += ((age_cost + state_cost) * discount_factor);
}

void HCVCEA::record_event(int age, double cost, double qol_decrement)
{
	double discount_factor = DiscountFactors[age];

	undiscounted_qalys += qol_decrement;
	undiscounted_costs += cost;

	discounted_qalys += ((qol_decrement) * discount_factor);
	discounted_costs += ((cost) * discount_factor);
}

double HCVCEA::get_discounted_costs()
{
	return discounted_costs;
}

double HCVCEA::get_discounted_qalys()
{
	return discounted_qalys;
}

double HCVCEA::get_undiscounted_costs()
{
	return undiscounted_costs;
}

double HCVCEA::get_undiscounted_qalys()
{
	return undiscounted_qalys;
}

