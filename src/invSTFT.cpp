
#include<complex>
#include "invSTFT.h"
#include"myfft.h"

void invSTFT(
		const twoDimArray<double>& abs, 
		const twoDimArray<std::complex<double> >& phase, 
		std::vector<double>* output, 
		int frame, int shift,
		double(*win)(int, int) )
{
	MyFFT myfft_inv (MyFFT::backward, frame);

	int n_frame = abs.get_n_frame();
	int n_freq = abs.get_n_freq();
	for(int n = 0; n < n_frame; n++){
		for(int f = 0; f < n_freq; f++){
			static_cast<std::complex<double>*>(myfft_inv.in)[f] = abs[n][f] * phase[n][f];
		}
		myfft_inv.exec();
		for(int i = 0; i < frame; i++){
			(*output)[n * shift + i ] += static_cast<double*>(myfft_inv.out)[i] / frame * 2 * win(i, frame);
		}
	}
}

void normalization(std::vector<double>* signal1, std::vector<double>* signal2){
	double max = 0;
	const int len = signal1->size();
	for(int n = 0; n < len; n++){
		if(max < abs((*signal1)[n])){
			max = abs((*signal1)[n]);
		}
		if(max < abs((*signal2)[n])){
			max = abs((*signal2)[n]);
		}
	}
	for(int n = 0; n < len; n++){
		(*signal1)[n] /= (max+1e-30);
		(*signal2)[n] /= (max+1e-30);
	}
}

