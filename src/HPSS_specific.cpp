

#include "HPSS_specific.h"

#include<algorithm>

// -------------------------------------------------------------------------------------------------
// mean and median
// -------------------------------------------------------------------------------------------------

inline double n_mean(const twoDimArray<double>& dat, const int n, const int k, const int M){
	double sum = 0.;
	for(int i = 1 ;i <= M; i++){
		sum += dat[n + i][k] + dat[n - i][k];
	}
	return sum / (2. * M);
}

inline double k_mean(const twoDimArray<double>& dat, const int n, const int k, const int M){
	double sum = 0.;
	for(int i = 1 ;i <= M; i++){
		sum += dat[n][k + i] + dat[n][k - i];
	}
	return sum / (2. * M);
}

inline double median_h(const twoDimArray<double>& dat, const int n, const int k, const int M){
	double tmp[2 * M + 1];
	for(int i = -M ;i <= M; i++){
		tmp[M + i] = dat[n + i][k];
	}
	std::sort(tmp, tmp + 2 * M + 1);
	return tmp[M];
}

inline double median_p(const twoDimArray<double>& dat, const int n, const int k, const int M){
	double tmp[2 * M + 1];
	for(int i = -M ;i <= M; i++){
		tmp[M + i] = dat[n][k + i];
	}
	std::sort(tmp, tmp + 2 * M + 1);
	return tmp[M];
}


// -------------------------------------------------------------------------------------------------
// Hard Mixing Model.
// Equation (21) and (22)
// -------------------------------------------------------------------------------------------------

void HPSS_HM2_1::update(void){
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){
			double H_time = n_mean(H, n, k, M);
			double P_freq = k_mean(P, n, k, M);

			double r = sqrt( H_time * H_time + P_freq * P_freq );

			H[n][k] = H_time * W[n][k] / (r + 1e-300);
			P[n][k] = P_freq * W[n][k] / (r + 1e-300);
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Hard Mixing Model.
// Equation (21) and (22)
// -------------------------------------------------------------------------------------------------

void HPSS_HM2_2::update(void){
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){
			double H_time = n_mean(H, n, k, M);
			double P_freq = k_mean(P, n, k, M);
			double tmp_h = (w * H_time + (W[n][k] - P_freq)) / (1 + w);
			double tmp_p = (P_freq + w * (W[n][k] - H_time)) / (1 + w);
			H[n][k] = tmp_h < 0 ? 0 : tmp_h < W[n][k] ? tmp_h : W[n][k];
			P[n][k] = tmp_p < 0 ? 0 : tmp_p < W[n][k] ? tmp_p : W[n][k];
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Idiv
// Updating Equations (27), (31), and (32)
// -------------------------------------------------------------------------------------------------

void HPSS_IDIV::update(void){
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){

			double A1 = 2 + c;
			double B1 = n_mean(H, n, k, M);
			double C1 = c * th[n][k] * W[n][k] * W[n][k];
			double A2 = 2 + c / w;
			double B2 = k_mean(P, n, k, M);
			double C2 = c/w * (1 - th[n][k]) * W[n][k] * W[n][k];

			H[n][k]  = (B1 + sqrt(B1 * B1 + A1 * C1)) / A1;
			P[n][k]  = (B2 + sqrt(B2 * B2 + A2 * C2)) / A2;
			th[n][k] = H[n][k] * H[n][k] / (H[n][k] * H[n][k] + P[n][k] * P[n][k] + 1e-100);
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Idiv sliding update
// -------------------------------------------------------------------------------------------------

void HPSS_IDIV_sliding::update(void){
const int WIN_SIZE = 30;
const int N_ITERATION = 10;
	for(int iii = 0; iii != N_ITERATION; iii++){
		for(int n = this->win_start; n < min(n_frame, this->win_start + WIN_SIZE); n++){
			for(int k = 0; k < n_freq; k++){

				double A1 = 2 + c;
				double B1 = n_mean(H, n, k, M);
				double C1 = c * th[n][k] * W[n][k] * W[n][k];
				double A2 = 2 + c / w;
				double B2 = k_mean(P, n, k, M);
				double C2 = c/w * (1 - th[n][k]) * W[n][k] * W[n][k];

				H[n][k]  = (B1 + sqrt(B1 * B1 + A1 * C1)) / A1;
				P[n][k]  = (B2 + sqrt(B2 * B2 + A2 * C2)) / A2;
				th[n][k] = H[n][k] * H[n][k] / (H[n][k] * H[n][k] + P[n][k] * P[n][k] + 1e-100);
			}
		}
	}
	this->win_start++;
}


// -------------------------------------------------------------------------------------------------
// FitzGerald's median filtering 
// -------------------------------------------------------------------------------------------------
void HPSS_median::update(void){
	for(int n = 0; n < n_frame; n++){
		for(int k = 0; k < n_freq; k++){
			H[n][k] = median_h(H, n, k, M);
			P[n][k] = median_p(P, n, k, M);
		}
	}
}

