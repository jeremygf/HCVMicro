///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//// Title: probdata.cpp
////
//// Description: This is a class that produces a table-like data structure to store
//// different probabilities in.
////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "probdata.h"


// Default Constructor
ProbData::ProbData()
{}

// Value Constructor
ProbData::ProbData(const vector<PCol>& Stuff): ProbMat(Stuff)
{}

// Copy Constructor
ProbData::ProbData(const ProbData& Other): ProbMat(Other.ProbMat)
{}

//////////////////////////////////////////////////////////
// Function: loadData
//
// Given an open file, load tab delimited data into itself
// as a side effect of the method.
///

void 	ProbData::loadData(ifstream& Infile)
{
	string	TempString;
	PCol	TempVect;
	string  Chillin;
	double  TempDouble; 
	unsigned col = 0;
	
// Get Header Line and....do nothing with it.
	Infile.ignore(10000,'\n');

	Chillin = " ";
//	TempVect.clear();

	// Copy first line of data into Chillin
	getline(Infile, Chillin, '\n');

	// Copy Chillin into FullLine
	std::istringstream FullLine(Chillin.c_str());

	// Ignore the header column
	FullLine.ignore(1000, '\t');

	// Read the first row and create a vector of vectors
	while(FullLine >> TempString)
	{
		TempVect.clear();
		TempDouble = atof(TempString.c_str());
		TempVect.push_back(TempDouble);
		ProbMat.push_back(TempVect);
	}	

	int row = 0;
	while (!Infile.eof())
	{
		col = 0;
		Chillin = " ";
		TempVect.clear();
		
		getline(Infile, Chillin, '\n');
		istringstream FullLine(Chillin.c_str());
		FullLine.ignore(1000, '\t');

		while(FullLine >> TempString)
		{
			TempDouble = atof(TempString.c_str());
			ProbMat[col].push_back(TempDouble);
			if (col > 0) {
				if(ProbMat[col].size() != ProbMat[col-1].size()) {
					printf("FUNNY, FUNNY HERE!: %d, %d\n", row, col);
					assert(false);
				}
			}
			col++;
		}
		row++;
	}
}


////////////////////////////////////////////////////////
// Obsolete Function.
//
// Prints a matrix to the screen.
// Useful for debugging.
//

void 	ProbData::printData()
{
	if (!ProbMat.size())
	{
		std::cout << "Empty Matrix" << endl;
		return;
	}
	
	std::cout << endl;
	for (unsigned r = 0; r < ProbMat[0].size(); r++)
	{
		std::cout << r << ": ";

		for (unsigned c = 0; c < ProbMat.size(); c++) {
			std::cout << this->getValue(r, c) << " ";
		}

		std::cout << endl;
	}
	std::cout << endl;
}

////////////////////////////////////////////////////////
// Obsolete Function: printData(char * filename)
//
// Prints a matrix to the Outfile.
// Useful for debugging.
//
void 	ProbData::printData(char * filename)
{
	ofstream Outfile(filename);
	if (!Outfile) {
		std::cout << "Warning: Unable to open " << filename << endl;
		return;
	}

	if (!ProbMat.size())
	{
		Outfile << "Empty Matrix" << endl;
		return;
	}
	
	Outfile << endl;
	for (unsigned r = 0; r < ProbMat[0].size(); r++)
	{
		Outfile << r;

		for (unsigned c = 0; c < ProbMat.size(); c++)
			Outfile << '\t' << this->getValue(r, c);

		Outfile << endl;
	}
}

////////////////////////////////////////////////////////
// Function: getValue()
//
// Input: Row and Column coordinates
//
// Output: Value at matrix coordinates
//
double ProbData::getValue(unsigned Row, unsigned Col)
{
	if (Col >= ProbMat.size())
	{
		std::cout << "\nError: Number of Columns is: " << static_cast<unsigned>(ProbMat.size()) << endl;
		std::cout << "Illegal Access in Column " << Col << endl;
		assert(false);
		printData();
	}
	if (Row >= ProbMat[Col].size())
	{
		std::cout << "\nError: Number of Rows is: " << static_cast<unsigned>(ProbMat[Col].size()) << endl;
		std::cout << "Illegal Access in Row " << Row << endl;
		assert(false);
		printData();
  }
	return ProbMat[Col][Row];
}

////////////////////////////////////////////////////////
// Function: setValue()
//
// Input: Row and Column coordinates and a Value
//

void ProbData::setValue(unsigned Row, unsigned Col, double Value)
{
	if (Col >= signed (ProbMat.size()))
	{
		std::cout << "\nError: Number of Columns is: " << static_cast<unsigned>(ProbMat.size()) << endl;
		std::cout << "Illegal Access in Column " << Col << endl;
		printData();
	}
	if (Row>= signed (ProbMat[Col].size()))
	{
		std::cout << "\nError: Number of Rows is: " << static_cast<unsigned>(ProbMat[Col].size()) << endl;
		std::cout << "Illegal Access in Row " << Row << endl;
		printData();
  }
	ProbMat[Col][Row] = Value;
}

////////////////////////////////////////////////////////
// Function: getCol()
//
// Input: Column coordinate
//
// Output: A column (vector)
//

ProbData::PCol	ProbData::getCol(unsigned Col)
{
	return ProbMat[Col];
}

////////////////////////////////////////////////////////
// Function checkSize ()
//
// This function checks to make sure that the matrix has the correct number of columns, 
// and that each column has the correct number of rows.
//

bool ProbData::checkSize (unsigned NumRows, unsigned NumCols)
{
	if (ProbMat.size() != NumCols) return false;
	for (unsigned i = 0; i < NumCols; i++) {
		if (ProbMat.at(i).size() != NumRows) return false;
	}
	return true;
}

int	ProbData::cumulativeProbability(double targetProb, unsigned Col)
{
	int n = ProbMat[Col].size();
	double curProb = 0;
	for(int i = 0; i < n; i++) {
		curProb += getValue(i, Col);
		if (curProb >= targetProb) {
			return i;
		}
	}
	return n;
}

void ProbData::ratesToProbabilities(void)
{
	int n = ProbMat.size();
	for (int i = 0; i < n; i++) {
		int m = ProbMat[i].size();
		for (int j = 0; j < m; j++) {
			setValue(j, i, 1.0 - exp(-1.0 * getValue(j, i)));
		}
	}
}

//////////////////////////////////////////////////////////
// LifeTableData::loadData(unsigned StartAge, unsigned MaxMonths, ProbData& MortData)
//
// This function uses the mortality data to create a 
// cumulative distribution of the age of death
// The mortality data contains monthly rates of death
// For internal years, we use a linear interpolation
//
void LifeTableData::loadData(unsigned StartYearAge, unsigned MaxMonths, ProbData& MortData)
{
	unsigned Cohort;
	unsigned NumCohorts;
	unsigned NumYears;
	unsigned NumMonths;
	unsigned EndYearAge;
	unsigned Year;
	unsigned Month;
	double ASRMort = 0.0;
	double MonthInc = 0.0;
	double MortRateThisYear;
	double MortRateLastYear;
	// double MortRateThisMonth;
	vector<double> MonthlyRates;
	double MonthProb;
	PCol TempCol;
//	time_t start, end;
	std::cout << "Calculating Life Tables" << endl;
	NumCohorts = MortData.numCols();
	NumYears = MortData.numRows() - StartYearAge;
//	NumYears = MortData.numRows();
//	time(&start);
	EndYearAge = StartYearAge + NumYears - 1;
	NumMonths = NumYears * MonthsPerYear;
	MonthlyRates.reserve(NumMonths);
	MonthlyRates.insert(MonthlyRates.begin(), NumYears * MonthsPerYear, 0);

	std::cout << "NumCohorts = " << NumCohorts << endl;

	for (Cohort = 0; Cohort < NumCohorts; Cohort++) {
	//	MortRateThisYear = MortData.getValue(StartYearAge - 1, Cohort);
		if (StartYearAge > 0) {
			MortRateThisYear = MortData.getValue(StartYearAge - 1, Cohort);
		} else {
			MortRateThisYear = MortData.getValue(0, Cohort);
		}

		// First, get all multiples of 12
		for (Year = 0; Year < NumYears; Year++) {
			MortRateLastYear = MortRateThisYear;	// save previous rate
			MortRateThisYear = MortData.getValue(Year+StartYearAge, Cohort);	// get new rate
			MonthlyRates[Year*MonthsPerYear] = 0.5 * (MortRateLastYear + MortRateThisYear);
	//		MonthlyRates[Year*MonthsPerYear] = MortRateThisYear;
		}

		// Then do a linear interpolation between them
		for (Year = 0; Year < NumYears; Year++) {
			if (Year == NumYears - 1) {
		//	if (1) {
				MonthInc = 0.0;
			} else {
				MonthInc = (MonthlyRates[(Year+1)*MonthsPerYear] - MonthlyRates[Year*MonthsPerYear]) / 12.0;
			}
			for (Month = 1+Year*MonthsPerYear; Month < (Year+1)*MonthsPerYear; Month++) {
				MonthlyRates[Month] = MonthlyRates[Month-1] + MonthInc;
			}
		}


		// Finally, calculate the life table from the monthly rates
		TempCol.reserve(MaxMonths);
		double probtot = 0;
//		TempCol.insert(TempCol.begin(), StartYearAge*MonthsPerYear, 0);
//		TempCol.push_back(0);
		for (Month = 0; Month < MaxMonths - (StartYearAge*MonthsPerYear) - 1; Month++) {
			if (Month < NumMonths) // If not past NumYears, get data for current year
			{
				MonthProb = 1 - exp(-MonthlyRates[Month]);
				
			} 
			probtot = probtot + ((1 - probtot) * MonthProb);
			TempCol.push_back(probtot);
//			TempCol.push_back(TempCol.back() + (1 - TempCol.back()) * MonthProb);
		}
		TempCol.push_back(1);

		ProbMat.push_back(TempCol);
		TempCol.clear();
	}

	//exit(0);
	
}

//end of file
