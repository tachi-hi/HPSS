
#pragma once
#include<vector>
#include<complex>
#include"twoDimArray.h"

//inv stft
void invSTFT(
		const twoDimArray<double>& abs, 
		const twoDimArray<std::complex<double> >& phase, 
		std::vector<double>* output, 
		int frame, int shift,
		double(*win)(int, int) );

// Normalize signal
void normalization(std::vector<double>* signal1, std::vector<double>* signal2);

