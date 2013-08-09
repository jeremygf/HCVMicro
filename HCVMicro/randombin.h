////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// Title: RandomBin.h
////
//// Description: A class that holds a variety of streams of common random
//// numbers.
////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef RANDOMBIN_H
#define RANDOMBIN_H

#include <vector>
#include "twist.h"
using namespace std;

class RandomBin
{
public:
  enum	Stream_type {Main = -1, Alive, Baseline, Progression, Treatment, STREAM_END};
	RandomBin(unsigned long Seed=0);
	~RandomBin();
	void seed(unsigned long Seed=0);
	double rand(Stream_type Stream);
	int randInt(Stream_type Stream);
	int randInt(Stream_type Stream, int minInt, int maxInt);
	long unsigned int seedReturn() {return MainStream->seedReturn();};
	void resetStreams();
	

private:
	MTRand* GetStream(Stream_type Stream);

	MTRand* MainStream;
	vector<MTRand*> Streams;
};


#endif
