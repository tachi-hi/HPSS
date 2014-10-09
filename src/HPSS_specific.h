
#pragma once
#include "HPSS.h"
using namespace std;


// -------------------------------------------------------------------------------------------------
// HM2-1
// -------------------------------------------------------------------------------------------------
class HPSS_HM2_1 : public HPSS{
public:
	HPSS_HM2_1(const twoDimArray<double>& W) : HPSS(W){}
	~HPSS_HM2_1(){}
	void update(void);
	double objective(void){
		return diffsum(H, P, n_frame, n_freq, w, M);
	}
};

// -------------------------------------------------------------------------------------------------
// HM2-2
// -------------------------------------------------------------------------------------------------
class HPSS_HM2_2 : public HPSS{
public:
	HPSS_HM2_2(const twoDimArray<double>& W) : HPSS(W){}
	~HPSS_HM2_2(){}
	void update(void);
	double objective(void){
		return diffsum(H, P, n_frame, n_freq, w, M);
	}
};

// -------------------------------------------------------------------------------------------------
// Idiv
// -------------------------------------------------------------------------------------------------
class HPSS_IDIV : public HPSS{
public:
	HPSS_IDIV(const twoDimArray<double>& W) : 
		HPSS(W),
		th (W.get_n_frame(), W.get_n_freq()){
		for(int t = 0; t < n_frame; t++){
			for(int f = 0; f < n_freq; f++){
				th[t][f] = 0.5;
			}
		}
	}

	~HPSS_IDIV(){}

	void update(void);
	double objective(void){	
		return idiv(H, P, W, n_frame, n_freq, w, c, M);
	}
private:
	twoDimArray<double> th;
};


// -------------------------------------------------------------------------------------------------
// Fitzgerald
// -------------------------------------------------------------------------------------------------
class HPSS_median : public HPSS{
public:
	HPSS_median(const twoDimArray<double>& W) : HPSS(W){}
	~HPSS_median(){};
	void update(void);
	double objective(void){
		return 0;
	}
};


