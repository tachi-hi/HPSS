#pragma once

#include <cmath>
#include <cassert>
#include "twoDimArray.h"
#include "rand.h"
#include "objFunction.h"
#include<iostream>
#include<vector>
using namespace std;

// -------------------------------------------------------------------------------------------------

class HPSS{
public:
	HPSS(const twoDimArray<double>&);
	virtual ~HPSS(){}

	// initialization
	typedef enum{
		equal, 
		allH, 
		allP, 
		binary_random, 
		cont_random} InitializationStyle;
	void setInitialValue(const InitializationStyle);

	// masking
	typedef enum{
		wiener,
		binary,
		none} mask;
	void masking(const mask, const double);

	// parameters
	void set_M     (int M_)	     {M = M_;}
	void set_w     (double w_)   {w = w_;}
	void set_c     (double c_)   {c = c_;}

	// virtual
	virtual void update(void){assert(false);};
	virtual double objective(void){assert(false);}

	// getter
	void get_H(twoDimArray<double>* H_){
		for(int i = 0; i < W.get_n_frame(); i++){
			for(int j = 0; j < W.get_n_freq(); j++){
				(*H_)[i][j] = H[i][j];
			}
		}
	}
	void get_P(twoDimArray<double>* P_){
		for(int i = 0; i < W.get_n_frame(); i++){
			for(int j = 0; j < W.get_n_freq(); j++){
				(*P_)[i][j] = P[i][j];
			}
		}
	}


protected:
	twoDimArray<double> W;
	twoDimArray<double> H;
	twoDimArray<double> P;
	twoDimArray<double> m;

	// basic parameters
	int M;    // M
	double w; // w
	double c; // c

	// n_frame and n_freq
	int n_frame;
	int n_freq;
};



