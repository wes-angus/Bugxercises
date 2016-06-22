#pragma once 
#include <stdlib.h>
#include <time.h>

class RNG{
public:
	RNG(){}
	~RNG(){}
	float ReturnNumber(float min, float max){
		srand(time(NULL));
		return (min+(rand() % (int)(max-min)));
	}
private:
	float InteralSeed;
};