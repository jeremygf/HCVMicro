////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: hcvcea.h
////
//// Description: A class that keeps track of a person's HRQoL over time 
//// and for various events and likewise keeps track of costs associated
//// with these states and events
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef HCVCEA_H
#define HCVCEA_H

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

class HCVCEA {
public:
	
	void init(double nannual_discount_rate, int start_age_yrs, 
					string& age_costs_utilities_path, string& hcv_state_costs_path, string& hcv_state_utilities_path,
					double nqol_hcvaware_decrement, double nqol_LT_decrement,
					double ncost_recover_F0F3, double ncost_recover_F4,
					double nqol_SVR_F0, double nqol_SVR_F1, double nqol_SVR_F2, double nqol_SVR_F3, double nqol_SVR_F4);

	void record_state(int age, HCVStatus& hcvstatus);
	void record_event(int age, double cost, double qol_decrement);

	double get_discounted_costs();
	double get_discounted_qalys();
	double get_undiscounted_costs();
	double get_undiscounted_qalys();

private:

	double annual_discount_rate;

	double qol_hcvaware_decrement;
	double qol_LT_decrement;

	double qol_SVR_F0;
	double qol_SVR_F1;
	double qol_SVR_F2;
	double qol_SVR_F3;
	double qol_SVR_F4;

	double cost_recover_F0F3;
	double cost_recover_F4;

	vector<double> DiscountFactors;

	int start_age_weeks;
	ProbData AgeCostsUtilitiesTable;
	ProbData HCVStateCostsTable;
	ProbData HCVStateUtilitiesTable;

	double undiscounted_costs;
	double undiscounted_qalys;

	double discounted_costs;
	double discounted_qalys;
};

#endif