#include "hcvtreatment_twodrug.h"

void HCVTreatment_TwoDrug::process_ini_file(CIniFile& INIFile)
{
	const string Directories_Name				= "Directories";
	const string InputDir_Name					= "InputDir";
	const string HCVTreatment_TwoDrugs_Name		= "HCVTreatment_TwoDrugs";
	const string rate_mort_Tx_Name				= "rate_mort_Tx";
	const string rate_quit_Tx_Name				= "rate_quit_Tx";
	const string Effectiveness_Name				= "Effectiveness";
	const string cost_Tx_wk_Name				= "cost_Tx_wk";
	const string cost_Tx_sideeffects_Name		= "cost_Tx_sideeffects";
	const string qol_Tx_decrement_wk_Name		= "qol_Tx_decrement_wk";

	rate_mort_Tx = INIFile.GetValueF(HCVTreatment_TwoDrugs_Name, rate_mort_Tx_Name);
	rate_quit_Tx = INIFile.GetValueF(HCVTreatment_TwoDrugs_Name, rate_quit_Tx_Name);
	cost_Tx_wk = INIFile.GetValueF(HCVTreatment_TwoDrugs_Name, cost_Tx_wk_Name);
	cost_Tx_sideeffects = INIFile.GetValueF(HCVTreatment_TwoDrugs_Name, cost_Tx_sideeffects_Name);
	qol_Tx_decrement_wk = INIFile.GetValueF(HCVTreatment_TwoDrugs_Name, qol_Tx_decrement_wk_Name);

	string InputDir = INIFile.GetValue(Directories_Name, InputDir_Name);
	string Effectiveness_File = INIFile.GetValue(HCVTreatment_TwoDrugs_Name, Effectiveness_Name);

	string effectiveness_path(InputDir);
	effectiveness_path.append(Effectiveness_File);

	ifstream effectiveness_data;
	effectiveness_data.open(effectiveness_path.c_str() );

	if (!effectiveness_data) {   // Check that the file was opened
		cerr << "Unable to open file " << effectiveness_path << endl;
		exit(1);   // call system to stop
	}

	EffectivenessTable.loadData(effectiveness_data);

	effectiveness_data.close();
}

void HCVTreatment_TwoDrug::init(CIniFile& INIFile, HCVCEA* phcvcea)
{
	// PROCESS INI FILE AND DO OTHER STRATEGY-SPECIFIC INITIALIZATION
	process_ini_file(INIFile);

	// CALL PARENT METHOD FOR OTHER INITIALIZATION
	HCVTreatment::init(INIFile, phcvcea);
}

void HCVTreatment_TwoDrug::process_G1(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white)
{

	int colidx = static_cast<int>(hcvriskstatus.il28b) + 3*(white==false);

	hcvcea->record_event(age, cost_Tx_wk, qol_Tx_decrement_wk);

	if (hcv_treatment_status.week == 11) {
		double prob_base = EffectivenessTable.getValue(0, colidx);
		if ( rb.rand(RandomBin::Treatment) > prob_base ) {
			hcvstatus.treatment = HCVStatus::FAIL;
		}
	} else if (hcv_treatment_status.week == 23) {
		double prob_base = EffectivenessTable.getValue(1, colidx);
		if ( rb.rand(RandomBin::Treatment) > prob_base ) {
			hcvstatus.treatment = HCVStatus::FAIL;
		}
	} else if (hcv_treatment_status.week == 47) {
		double prob_base = EffectivenessTable.getValue(2, colidx);
		double prob_mult = 1.0;
		if (hcvstatus.fibrosis == HCVStatus::F4) {
			prob_mult = EffectivenessTable.getValue(3, colidx);
		}
		if ( rb.rand(RandomBin::Treatment) <= prob_base*prob_mult ) {
			hcvstatus.treatment = HCVStatus::SUCCESS;
		} else {
			hcvstatus.treatment = HCVStatus::FAIL;
		}
	}
}

void HCVTreatment_TwoDrug::process_G2(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white)
{
	int colidx = static_cast<int>(hcvriskstatus.il28b) + 3*(white==false);
	int rowidx_base = 4;

	hcvcea->record_event(age, cost_Tx_wk, qol_Tx_decrement_wk);

	if (hcv_treatment_status.week == 23) {
		double prob_base = EffectivenessTable.getValue(rowidx_base, colidx);
		double prob_mult = 1.0;
		if (hcvstatus.fibrosis == HCVStatus::F4) {
			prob_mult = EffectivenessTable.getValue(rowidx_base + 1, colidx);
		}
		if ( rb.rand(RandomBin::Treatment) <= prob_base*prob_mult ) {
			hcvstatus.treatment = HCVStatus::SUCCESS;
		} else {
			hcvstatus.treatment = HCVStatus::FAIL;
		}
	}
}

void HCVTreatment_TwoDrug::process_G3(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, bool male, bool white)
{
	int colidx = static_cast<int>(hcvriskstatus.il28b) + 3*(white==false);
	int rowidx_base = 6;

	hcvcea->record_event(age, cost_Tx_wk, qol_Tx_decrement_wk);

	if (hcv_treatment_status.week == 23) {
		double prob_base = EffectivenessTable.getValue(rowidx_base, colidx);
		double prob_mult = 1.0;
		if (hcvstatus.fibrosis == HCVStatus::F4) {
			prob_mult = EffectivenessTable.getValue(rowidx_base + 1, colidx);
		}
		if ( rb.rand(RandomBin::Treatment) <= prob_base*prob_mult ) {
			hcvstatus.treatment = HCVStatus::SUCCESS;
		} else {
			hcvstatus.treatment = HCVStatus::FAIL;
		}
	}

}

bool HCVTreatment_TwoDrug::treatment_update(RandomBin& rb, int age, HCVTreatmentStatus& hcv_treatment_status, HCVStatus& hcvstatus, HCVRiskStatus& hcvriskstatus, 
											bool male, bool white, double tx_mort_rate)
{
	if (hcvstatus.treatment == HCVStatus::ONGOING && hcvstatus.fibrosis >= HCVStatus::F0 && hcvstatus.fibrosis <= HCVStatus::F4) {
		
		if (hcv_treatment_status.week == 0) {
			// avg'd so everyone gets charged even though everyone does not get side effects
			hcvcea->record_event(age, cost_Tx_sideeffects, 0);
		}

		if ( rb.rand(RandomBin::Treatment) < ( 1-exp(-1*tx_mort_rate) ) ) {
			hcvstatus.treatment = HCVStatus::FAIL;
			return true;
		}

		if (hcvstatus.genotype == HCVStatus::G1) {
			if (hcv_treatment_status.week >= 48) {
				hcvstatus.treatment = HCVStatus::FAIL;
				hcv_treatment_status.week = 0;
			} else {
				process_G1(rb, age, hcv_treatment_status, hcvstatus, hcvriskstatus, male, white);
				hcv_treatment_status.week++;
			}
		} else if (hcvstatus.genotype == HCVStatus::G2) {
			if (hcv_treatment_status.week >= 24) {
				hcvstatus.treatment = HCVStatus::FAIL;
				hcv_treatment_status.week = 0;
			} else {
				process_G2(rb, age, hcv_treatment_status, hcvstatus, hcvriskstatus, male, white);
				hcv_treatment_status.week++;
			}
		} else if (hcvstatus.genotype == HCVStatus::G3) {
			if (hcv_treatment_status.week >= 24) {
				hcvstatus.treatment = HCVStatus::FAIL;
				hcv_treatment_status.week = 0;
			} else {
				process_G3(rb, age, hcv_treatment_status, hcvstatus, hcvriskstatus, male, white);
				hcv_treatment_status.week++;
			}
		}

	} else {
		if (hcvstatus.treatment == HCVStatus::ONGOING && hcvstatus.fibrosis >= HCVStatus::F4) {
			hcvstatus.treatment = HCVStatus::FAIL;
			hcv_treatment_status.week = 0;
		}
	}
	return false;
}
