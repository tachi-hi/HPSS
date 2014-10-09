
#include "twoDimArray.h"
#include<cmath>

void exp_spectrum(twoDimArray<double>* spec, double gamma){
	for(int i = 0; i < spec->get_n_frame(); i++){
		for(int j = 0; j < spec->get_n_freq(); j++){
			(*spec)[i][j] = pow((*spec)[i][j], gamma);
		}
	}
}

