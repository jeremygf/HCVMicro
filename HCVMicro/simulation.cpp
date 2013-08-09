////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Simulation.cpp
////
////
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include "simulation.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
#include "mortality.h"
#include "baseline.h"
#include "hcvprogression.h"
#include "hcvcea.h"
#include "hcvtreatment.h"
#include "hcvtreatment_notx.h"
#include "hcvtreatment_twodrug.h"
#include "population.h"

using namespace std;

HCVTreatment* Simulation::get_treatment_strategy(string& strategy_name, CIniFile* INIFile, HCVCEA* phcvcea) {
	HCVTreatment *phcvtx = 0;
	if (strategy_name == "NoTreatment") {
		phcvtx = static_cast<HCVTreatment*>(new HCVTreatment_NoTx());
	} else if (strategy_name == "TwoDrug") {
		phcvtx = static_cast<HCVTreatment*>(new HCVTreatment_TwoDrug());
	} else {
		cerr << "Undefined treatment strategy name: " << strategy_name << endl;
		assert(false);
	}
	phcvtx->init(*INIFile, phcvcea);
	return phcvtx;
}

void Simulation::init(CIniFile& Master)
{
  // Section names (keys for Master)
  const string InputDataFiles_Name			= "InputDataFiles";
  const string OutputDataFiles_Name			= "OutputDataFiles";
  const string DataValues_Name				= "DataValues";
  const string Directories_Name				= "Directories";
  
  // Input data Files
  const string Mortality_Name				= "Mortality";
  const string Fibrosis_Name				= "Fibrosis";
  const string IL28B_Name					= "IL28B";
  const string HighRisk_Name				= "HighRisk";
  const string HCV_Name						= "HCV";
  const string Genotype_Name				= "Genotype";
  const string FibrosisProgression_Name		= "FibrosisProgression";
  const string AgeCostsUtilities_Name		= "AgeCostsUtilities";
  const string HCVStateCosts_Name			= "HCVStateCosts";
  const string HCVStateUtilities_Name		= "HCVStateUtilities";

  // Output data files
  const string Output_BaselinePrev_Name		= "Output_BaselinePrev";
  const string Output_LE_Name				= "Output_LE";
  const string Output_MarkovTrace_Name		= "Output_MarkovTrace";
  const string Output_CEA_Name				= "Output_CEA";
      
  // Data values
  const string sim_size_Name				= "sim_size";
  const string start_age_yrs_Name			= "start_age_yrs";
  const string prop_male_Name				= "prop_male";
  const string prop_white_Name				= "prop_white";
  const string prop_nonprogress_Name		= "prop_nonprogress";
  const string prop_aware_Name				= "prop_aware";

  const string rate_remission_Name			= "rate_remission";
  const string rate_F4_DC_Name				= "rate_F4_DC";
  const string rate_F4DC_HCC_Name			= "rate_F4DC_HCC";
  const string rate_F4TX_HCC_Name			= "rate_F4TX_HCC";
  const string rate_DC_LT_Name				= "rate_DC_LT";
  const string rate_HCC_LT_Name				= "rate_HCC_LT";
  const string rate_mort_LT_Name			= "rate_mort_LT";
  const string rate_mort_postLT_Name		= "rate_mort_postLT";
  const string rate_mort_DC_Name			= "rate_mort_DC";
  const string rate_mort_HCCfirstyear_Name	= "rate_mort_HCCfirstyear";
  const string rate_mort_HCC2plusyears_Name	= "rate_mort_HCC2plusyears";
  const string rate_mort_Tx_Name			= "rate_mort_Tx";
  const string factor_redux_Cure_Name		= "factor_redux_Cure";
  const string prob_mort_liverbiopsy_Name	= "prob_mort_liverbiopsy";
  const string limit_maxLTage_yrs_Name		= "limit_maxLTage_yrs";

  const string annual_discount_rate_Name	= "annual_discount_rate";

  const string qol_hcvaware_decrement_Name	= "qol_hcvaware_decrement";
  const string qol_LT_decrement_Name		= "qol_LT_decrement";
  const string qol_SVR_F0_Name				= "qol_SVR_F0";
  const string qol_SVR_F1_Name				= "qol_SVR_F1";
  const string qol_SVR_F2_Name				= "qol_SVR_F2";
  const string qol_SVR_F3_Name				= "qol_SVR_F3";
  const string qol_SVR_F4_Name				= "qol_SVR_F4";
  const string cost_recover_F0F3_Name		= "cost_recover_F0F3";
  const string cost_recover_F4_Name			= "cost_recover_F4";


  const string treatment_strategy_Name		= "treatment_strategy";

  // Directories
  const string InputDir_Name				= "InputDir";
  const string OutputDir_Name				= "OutputDir";

  // Read names for directories
  string InputDir = Master.GetValue(Directories_Name, InputDir_Name);
  string OutputDir = Master.GetValue(Directories_Name, OutputDir_Name);

  // Number of People to Simulate and other Simulation Parameters
  sim_size = Master.GetValueF(DataValues_Name, sim_size_Name);
  int start_age_yrs = Master.GetValueF(DataValues_Name, start_age_yrs_Name);

  // Setup Random Number Generator System
  RandGen = new RandomBin;      // Create an instance of RandomBin called RandGen            
  RandGen->seed();              // Seed RandGen

  // Mortality Risks
  string Mortality_File = Master.GetValue(InputDataFiles_Name, Mortality_Name);
  mortality = new Mortality;
  string mortality_path(InputDir);
  mortality_path.append(Mortality_File);
  mortality->init(mortality_path);

  // Baseline Values
  string Fibrosis_File = Master.GetValue(InputDataFiles_Name, Fibrosis_Name);
  string IL28B_File = Master.GetValue(InputDataFiles_Name, IL28B_Name);
  string HighRisk_File = Master.GetValue(InputDataFiles_Name, HighRisk_Name);
  string HCV_File = Master.GetValue(InputDataFiles_Name, HCV_Name);
  string Genotype_File = Master.GetValue(InputDataFiles_Name, Genotype_Name);
  double prop_male = Master.GetValueF(DataValues_Name, prop_male_Name);
  double prop_white = Master.GetValueF(DataValues_Name, prop_white_Name);
  double prop_nonprogress = Master.GetValueF(DataValues_Name, prop_nonprogress_Name);
  double prop_aware = Master.GetValueF(DataValues_Name, prop_aware_Name);
  baseline = new Baseline;
  string fibrosis_path(InputDir);
  string il28b_path(InputDir);
  string highrisk_path(InputDir);
  string hcv_path(InputDir);
  string genotype_path(InputDir);
  fibrosis_path.append(Fibrosis_File);
  il28b_path.append(IL28B_File);
  highrisk_path.append(HighRisk_File);
  hcv_path.append(HCV_File);
  genotype_path.append(Genotype_File);
  baseline->init(fibrosis_path, il28b_path, highrisk_path, hcv_path, genotype_path, prop_male, prop_white, prop_nonprogress, prop_aware, start_age_yrs);

  // Costs and Utility Weights
  string AgeCostsUtilities_File = Master.GetValue(InputDataFiles_Name, AgeCostsUtilities_Name);
  string HCVStateCosts_File = Master.GetValue(InputDataFiles_Name, HCVStateCosts_Name);
  string HCVStateUtilities_File = Master.GetValue(InputDataFiles_Name, HCVStateUtilities_Name);
  string age_costs_utilities_path(InputDir);
  string hcv_state_costs_path(InputDir);
  string hcv_state_utilities_path(InputDir);
  age_costs_utilities_path.append(AgeCostsUtilities_File);
  hcv_state_costs_path.append(HCVStateCosts_File);
  hcv_state_utilities_path.append(HCVStateUtilities_File);
  double annual_discount_rate = Master.GetValueF(DataValues_Name, annual_discount_rate_Name);
  double qol_hcvaware_decrement = Master.GetValueF(DataValues_Name,  qol_hcvaware_decrement_Name);
  double qol_LT_decrement = Master.GetValueF(DataValues_Name,  qol_LT_decrement_Name);
  double qol_SVR_F0 = Master.GetValueF(DataValues_Name,  qol_SVR_F0_Name);
  double qol_SVR_F1 = Master.GetValueF(DataValues_Name,  qol_SVR_F1_Name);
  double qol_SVR_F2 = Master.GetValueF(DataValues_Name,  qol_SVR_F2_Name);
  double qol_SVR_F3 = Master.GetValueF(DataValues_Name,  qol_SVR_F3_Name);
  double qol_SVR_F4 = Master.GetValueF(DataValues_Name,  qol_SVR_F4_Name);
  double cost_recover_F0F3 = Master.GetValueF(DataValues_Name, cost_recover_F0F3_Name);
  double cost_recover_F4 = Master.GetValueF(DataValues_Name, cost_recover_F4_Name);
  hcvcea = new HCVCEA();
  hcvcea->init(annual_discount_rate, start_age_yrs, age_costs_utilities_path, hcv_state_costs_path, hcv_state_utilities_path, 
					qol_hcvaware_decrement, qol_LT_decrement,
					cost_recover_F0F3, cost_recover_F4,
					qol_SVR_F0, qol_SVR_F1, qol_SVR_F2, qol_SVR_F3, qol_SVR_F4);

  // Progression Values
  string FibrosisProgression_File = Master.GetValue(InputDataFiles_Name, FibrosisProgression_Name);
  string fibrosisprogression_path(InputDir);
  fibrosisprogression_path.append(FibrosisProgression_File);
  HCVProgression::HCVProgressionData progdata;
  progdata.rate_remission = Master.GetValueF(DataValues_Name, rate_remission_Name);
  progdata.rate_F4_DC = Master.GetValueF(DataValues_Name, rate_F4_DC_Name);
  progdata.rate_F4DC_HCC = Master.GetValueF(DataValues_Name, rate_F4DC_HCC_Name);
  progdata.rate_F4TX_HCC = Master.GetValueF(DataValues_Name, rate_F4TX_HCC_Name);
  progdata.rate_DC_LT = Master.GetValueF(DataValues_Name, rate_DC_LT_Name);
  progdata.rate_HCC_LT = Master.GetValueF(DataValues_Name, rate_HCC_LT_Name);
  progdata.rate_mort_LT = Master.GetValueF(DataValues_Name, rate_mort_LT_Name);
  progdata.rate_mort_postLT = Master.GetValueF(DataValues_Name, rate_mort_postLT_Name);
  progdata.rate_mort_DC = Master.GetValueF(DataValues_Name, rate_mort_DC_Name);
  progdata.rate_mort_HCCfirstyear = Master.GetValueF(DataValues_Name, rate_mort_HCCfirstyear_Name);
  progdata.rate_mort_HCC2plusyears = Master.GetValueF(DataValues_Name, rate_mort_HCC2plusyears_Name);
  progdata.rate_mort_Tx = Master.GetValueF(DataValues_Name, rate_mort_Tx_Name);
  progdata.factor_redux_Cure = Master.GetValueF(DataValues_Name, factor_redux_Cure_Name);
  progdata.prob_mort_liverbiopsy = Master.GetValueF(DataValues_Name, prob_mort_liverbiopsy_Name);
  progdata.limit_maxLTage_yrs = Master.GetValueF(DataValues_Name, limit_maxLTage_yrs_Name);
  hcvprogression = new HCVProgression();
  hcvprogression->init(fibrosisprogression_path, progdata, hcvcea);

  // Treatment Strategy
  string treatment_strategy = Master.GetValue(DataValues_Name, treatment_strategy_Name);
  hcvtreatment = get_treatment_strategy(treatment_strategy, &Master, hcvcea);
  hcvtreatment_status = hcvtreatment->get_treatment_status();

  // Setup Population for Output of Simulation Information
  string BaselinePrev_File = Master.GetValue(OutputDataFiles_Name, Output_BaselinePrev_Name);
  string LE_File = Master.GetValue(OutputDataFiles_Name, Output_LE_Name);
  string MarkovTrace_File = Master.GetValue(OutputDataFiles_Name, Output_MarkovTrace_Name);
  string CEA_File = Master.GetValue(OutputDataFiles_Name, Output_CEA_Name);
  string baselineprev_path(OutputDir);
  string le_path(OutputDir);
  string markovtrace_path(OutputDir);
  string cea_path(OutputDir);
  baselineprev_path.append(BaselinePrev_File);
  le_path.append(LE_File);
  markovtrace_path.append(MarkovTrace_File);
  cea_path.append(CEA_File);
  population = new Population();
  population->init(hcvcea, baselineprev_path, le_path, markovtrace_path, cea_path);

}


void Simulation::run()
{

	double LE = 0;
	double maxLE = 0;

	population->open_outputfiles();

	// Loop through simulated persons
	for (int number = 0; number < sim_size; number++) {

		Person person;
		hcvtreatment_status->init();
		person.init(RandGen, mortality, baseline, hcvprogression, hcvtreatment, hcvtreatment_status);

		population->register_person(number, person);

		if ((number % 1000) == 0) {
			cout << number << " SE: " << person.sex << " RA: " << person.race <<
				" IL: " << person.HCVRisk.il28b << " RI: " << person.HCVRisk.risk <<
				" IN: " << person.HCV.infected << " GE: " << person.HCV.genotype <<
				" FI: " << person.HCV.fibrosis << " PR: " << person.HCV.progressor <<
				" AW: " << person.HCV.aware <<
				endl;
		}

		while (person.is_alive()) {
			person.update();
			population->record_status(number, person);
		}

		population->record_le(number, person.age);

		if (person.age > maxLE) {
			maxLE = person.age;
		}
		if (number>0) {
			LE = (LE*number + person.age)/(number+1);
		} else {
			LE = person.age;
		}
	}

	cout << "LE is: " << LE << endl;
	cout << "MAX LE is: " << maxLE << endl;

	population->close_outputfiles();
}

void Simulation::write()
{
// STUBBED OUT FOR NOW
}


void Simulation::clean_up()
{
  delete RandGen, mortality, baseline, hcvprogression, hcvcea, population;
}