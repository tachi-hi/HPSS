
/* -----------------------------------------------
 Objective Functions.

 Despite the theoretical verifications,
 these values do not necesarilly decrease,
 possibly because of numerical errors.
------------------------------------------------- */

#include "objFunction.h"
#include<cmath>


double smoothness_time(	
	const twoDimArray<double> &H, 
	const int n_frame, 
	const int n_freq, 
	const int M)
{
	double sum = 0.;
	for(int n = 0; n < n_frame-0; n++){
		for(int k = 0; k < n_freq-0; k++){
			double h = 0.;
			for(int m = 1; m <= M; m++){ 
				h += (H[n][k] - H[n - m][k]) * (H[n][k] - H[n - m][k]);
			}
			sum += h / M;
		}
	}
	return sum;
}


double smoothness_freq(	
	const twoDimArray<double> &P, 
	const int n_frame, 
	const int n_freq, 
	const int M)
{
	double sum = 0.;
	for(int n = 0; n < n_frame-0; n++){
		for(int k = 0; k < n_freq-0; k++){
			double p = 0.;
			for(int m = 1; m <= M; m++){ 
				p += (P[n][k] - P[n][k - m]) * (P[n][k] - P[n][k - m]);
			}
			sum += p / M;
		}
	}
	return sum;
}

double simple_sum(	
	const twoDimArray<double> &X, 
	const int n_frame, 
	const int n_freq)
{
	double sum = 0.;
	for(int n = 0; n < n_frame-0; n++){
		for(int k = 0; k < n_freq-0; k++){
			sum += (X[n][k]) * (X[n][k]);
		}
	}
	return sum;
}

double diffsum(
	const twoDimArray<double> &H,
	const twoDimArray<double> &P,
	const int n_frame, const int n_freq,
	const double w,
	const int M)
{
	return smoothness_time(H, n_frame, n_freq, M) + w * smoothness_freq(P, n_frame, n_freq, M);
/*
	double sum = 0.;
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){
			double h = 0.;
			double p = 0.;
			for(int m = 1; m <= M; m++){ 
				h += (H[n][k] - H[n - m][k]) * (H[n][k] - H[n - m][k]);
				p += (P[n][k] - P[n][k - m]) * (P[n][k] - P[n][k - m]);
			}
			h /= M;
			p /= M;
			sum += h + w * p;
		}
	}
	return sum;
*/
}


double idiv(
	const twoDimArray<double> &H,
	const twoDimArray<double> &P,
	const twoDimArray<double> &W,
	const int n_frame, const int n_freq,
	const double w, const double c, const int M)
	{
	double sum = 0.;
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){
			double h = 0.;
			double p = 0.;
			for(int q = 1; q <= M; q++){
				h += (H[n][k] - H[n - q][k]) * (H[n][k] - H[n - q][k]);
				p += (P[n][k] - P[n][k - q]) * (P[n][k] - P[n][k - q]);
			}
			h /= M;
			p /= M;
			double r = W[n][k] * W[n][k] * log(W[n][k] * W[n][k] / (H[n][k] * H[n][k] + P[n][k] * P[n][k] + 1e-100) + 1e-100) 
				+ (H[n][k] * H[n][k] + P[n][k] * P[n][k] - W[n][k] * W[n][k]);
			sum += h + w * p + c * r;
		}
	}
	return sum;
}



