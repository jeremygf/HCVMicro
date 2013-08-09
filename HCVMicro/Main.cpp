// ***********************************************************************
// Main.cpp
// Entry point into application
// Handles reading in the control file (master.ini)
// Passing information to the simulation objects so that they can
// 1) initialize themselves
// 2) run through the simulations
// 3) report back the simulation results
// 
// TOOK OUT ONE LINE
// ADDED ONE BACK IN
// THIRD LINE FROM SHAN
// FOURTH FROM JEREMY
// ***********************************************************************

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "iniFile.h"
#include "simulation.h"

using namespace std;


void init_sim(string& master_file, Simulation& sim) {
  string data = ".\\";
  string MasterName(data);
  MasterName.append(master_file);

  cout << MasterName << endl;

  // Check that Master is readable
  CIniFile Master(MasterName);
  if (!Master.ReadFile()) {
    std::cerr << "\nError: Could not read Master File: " << MasterName << endl;
	exit(-1);
  }

  sim.init(Master);
}

int main(int argc, char* argv[])
{
	cout << "HCVMicrosimulation Starting" << endl;

	// First argument gives the control file name
	assert(argc>=2);
	string master_file = argv[1];

	Simulation sim;
	init_sim(master_file, sim);
	
	sim.run();                          // Run simulation
	sim.write();				        // Print output tables etc.
	sim.clean_up();						// Clean up memory etc.

	cout << "HCVMicrosimulation Finishing" << endl;
	return(0);
}










