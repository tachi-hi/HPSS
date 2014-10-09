#pragma once

#include "twoDimArray.h"


double smoothness_time(	
	const twoDimArray<double> &H, 
	const int n_frame, 
	const int n_freq, 
	const int M);

double smoothness_freq(	
	const twoDimArray<double> &P, 
	const int n_frame, 
	const int n_freq, 
	const int M);

double simple_sum(
	const twoDimArray<double> &X, 
	const int n_frame, 
	const int n_freq);

double diffsum(
	const twoDimArray<double> &H,
	const twoDimArray<double> &P,
	const int n_frame, const int n_freq,
	const double weight,
	const int M);

double idiv(
	const twoDimArray<double> &H,
	const twoDimArray<double> &P,
	const twoDimArray<double> &W,
	const int n_frame, const int n_freq,
	const double w, const double c, const int M);



