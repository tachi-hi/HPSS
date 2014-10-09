

#include "HPSS.h"

// -----------------------------------------------------------
// constructor: 
// allocate all the spectrograms and copy the content to W
// -----------------------------------------------------------
HPSS::HPSS(const twoDimArray<double>& spec):
	W (spec),
	H (spec.get_n_frame(), spec.get_n_freq()),
	P (spec.get_n_frame(), spec.get_n_freq()),
	m (spec.get_n_frame(), spec.get_n_freq())
{
	n_frame = W.get_n_frame();
	n_freq = W.get_n_freq();
}

// -----------------------------------------------------------
// initialize H and P and mask
// -----------------------------------------------------------
void HPSS::setInitialValue(const HPSS::InitializationStyle style){ 
	genUniRand rnd;
	int n_frame = W.get_n_frame();
	int n_freq  = W.get_n_freq();
	for(int t = 0; t < n_frame; t++){
		for(int f = 0; f < n_freq; f++){
			if(style == equal) 
				H[t][f] = P[t][f] = W[t][f] / 2;
			if(style == allH)  
				H[t][f] = W[t][f], P[t][f] = 0.;
			if(style == allP)  
				H[t][f] = 0., P[t][f] = W[t][f];
			if(style == binary_random)
				H[t][f] = rnd.get1() > 0.5 ? 1 : 0, P[t][f] = W[t][f] - H[t][f];
			if(style == cont_random)
				H[t][f] = rnd.get1() * W[t][f], P[t][f] = W[t][f] - H[t][f];
		}		
	}
}

// -----------------------------------------------------------
// time frequency masking
// Wiener:
//                    H[t][k]^p
//  H[t][k]^p = --------------------- W[t][k]^p
//              H[t][k]^p + P[t][k]^p
//
//                          H[t][k]
//  <=> H[t][k]   = ------------------------------- W[t][k]
//                   (H[t][k]^p + P[t][k]^p)^{1/p}
//
//                        H[t][k]
//  <=> H[t][k]   = ----------------------- W[t][k]
//                   (H_tmp + P_tmp)^{1/p}
//
// -----------------------------------------------------------
void HPSS::masking(const HPSS::mask M, const double factor){
	const int n_frame = W.get_n_frame();
	const int n_freq  = W.get_n_freq();
	for(int t = 0; t < n_frame; t++){
		for(int k = 0; k < n_freq; k++){
			if(M == wiener){
				double H_tmp = pow(H[t][k], factor);
				double P_tmp = pow(P[t][k], factor);
				H[t][k] = 	H[t][k] / (pow(H_tmp + P_tmp, 1. / factor) + 1e-100) * W[t][k];
				P[t][k] = 	P[t][k] / (pow(H_tmp + P_tmp, 1. / factor) + 1e-100) * W[t][k];
			}else if(M == binary){
				double H_tmp = H[t][k];
				double P_tmp = P[t][k];
				H[t][k] = H_tmp > P_tmp ? W[t][k] : 0;
				P[t][k] = H_tmp > P_tmp ? 0 : W[t][k];
			}else if(M == none){
				; // do nothing
			}else{
				assert ( M == wiener || M == binary || M == none );
			}
		}
	}
}


