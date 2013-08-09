////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: Simulation.h
////
//// Description: A class that manages data inputs and make sure that the 
//// Population of individuals to be simulated is setup to run properly
//// 
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "randombin.h"
#include "probdata.h"
#include "baseline.h"
#include "hcvprogression.h"
#include "hcvtreatment.h"
#include "hcvcea.h"
#include "population.h"
#include "mortality.h"
#include "iniFile.h"

using namespace std;


class Simulation
{

 public:

	void				init(CIniFile& Master);
	void				run();
	void				write();
	void				clean_up();
	
	RandomBin*			RandGen;    
	Population*			population;
	Baseline*			baseline;
	HCVProgression*		hcvprogression;
	HCVCEA*				hcvcea;
	Mortality*			mortality;
	HCVTreatment*		hcvtreatment;
	HCVTreatmentStatus*	hcvtreatment_status;
	
	int sim_size;

private:

	HCVTreatment*		get_treatment_strategy(string& strategy_name, CIniFile* INIFile, HCVCEA* hcvcea);

};


#endif
