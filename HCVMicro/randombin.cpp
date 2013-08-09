////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: RandomBin.cpp
////
//// Description: The implementation of a class that creates a number of
//// streams of random numbers to maintain common random numbers.
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "randombin.h"

///////////////////////////////////////////////////////////
// RandomBin :: RandomBin(Seed)
// constructor using a submitted seed.

RandomBin :: RandomBin(unsigned long Seed) 
{

	// initalize all streams

	MainStream = new MTRand(Seed);
	for (int i = 0; i < STREAM_END; i++) 
	{
		Streams.push_back(new MTRand(MainStream -> randInt()));
	}
}

///////////////////////////////////////////////
// Random Bin::seed(Seed)
// reseeds the entire bin
void RandomBin::seed(unsigned long Seed)
{
	if (Seed == 0) {
		MainStream -> seed();
	} else {
		MainStream -> seed(Seed);
	}
	for (int i = 0; i < STREAM_END; i++) 
	{
		Streams[i]->seed(MainStream->randInt());
	}
}

///////////////////////////////////////////////
// Random Bin deconstructor

RandomBin :: ~RandomBin()
{
	delete	MainStream;
	for (int i = 0; i < STREAM_END; i++) 
	{
		delete Streams[i];
	}
}

MTRand* RandomBin::GetStream(Stream_type Stream)
{
	if (Stream == Main) {
		return MainStream;
	} else {
		if (Stream >= 0 && Stream < STREAM_END) {
			return Streams[Stream];
		} else {
			std::cerr << "Error: unrecognized Stream_type in RandomBin" << endl;
			return NULL;
		}
	}
}

///////////////////////////////////////////////////////////
// RandomBin :: rand(enum Stream)
// switch statement to select a random number in (0,1) from the 
// requested stream
//

double RandomBin :: rand(Stream_type Stream)
{
	MTRand *streamPtr = GetStream(Stream);
	if (streamPtr != NULL) {
		return streamPtr->randDblExc();
	}
	return 0.0;	
}
// RandomBin :: rand(enum Stream)
///////////////////////////////////////////////////////////
// switch statement to select a random number from the 
// requested stream
//

int RandomBin :: randInt(Stream_type Stream)
{
	MTRand *streamPtr = GetStream(Stream);
	if (streamPtr != NULL) {
		return streamPtr->randInt();
	}
	return 0;
}

int RandomBin::randInt(Stream_type Stream, int minInt, int maxInt)
{
	double randNum = 0;
	int retInt = 0;
	MTRand *streamPtr = GetStream(Stream);
	if (streamPtr != NULL) {
		randNum = streamPtr->rand();
		randNum = randNum * static_cast<double>((maxInt - minInt + 1));
		retInt = static_cast<int>(ceil(randNum)) - 1 + minInt;
		return retInt;
	}
	return 0;
}

///////////////////////////////////////////////////////////
// RandomBin :: resetSubStream();
// reseeds those streams needed for common random numbers
///
void RandomBin :: resetStreams()
{
	// reset those Streams that are to be reseeded
	for (int i = 0; i < STREAM_END; i++) 
	{
		Streams[i]->seed(MainStream->randInt());
	}
}
