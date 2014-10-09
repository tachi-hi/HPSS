#pragma once

#include<cstdlib>

class genUniRand{
public:
	genUniRand(){}
	~genUniRand(){}
	void get(double* ret, int length){
		for(int i = 0; i < length; i++){
			ret[i] = (double)rand() / RAND_MAX;
		}
	}
	double get1(void){
		return (double)rand() / RAND_MAX;
	}
	void resetSeed(void);
protected:

};

