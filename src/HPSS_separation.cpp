
#include"trapfpe.c"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <complex>
#include <algorithm>

#include "waviostream.hpp"
#include "HPSS_separation.h"

#include "my_timer.h"

#include "STFT.h"
#include "invSTFT.h"
#include "HPSS.h"
#include "HPSS_specific.h"

using namespace std;


/****************************************************************************************************/
int main(int argc, char**argv){
	myTimer timer;

	/****************************************************************************************************/
	// Command line parser:
	// All command line parameteres (including default values) are set in "opt."
	/****************************************************************************************************/
	command_line_options opt(argc, argv);

	// Alias to command line parameters
	int& frame_length = opt.frame_length;
	int& frame_shift = opt.frame_shift;

	/****************************************************************************************************/
	// input file
	/****************************************************************************************************/
	const int maximal_buffer_length = frame_length;
	wavistream *input = new wavistream( opt.input_file_name.c_str(), maximal_buffer_length);

	// Alias to input file parameters
	const unsigned int sampling_rate = input->header.sampling_rate;
	const short unsigned int I = input->header.n_channel;
	if( I != 1 && I != 2){
	 cerr << "input file is not monaural audio file! " << endl;
	 exit(1);
	}
	const int total_samples = 8 * (input->header.filesize-36) 	/ (input->header.n_channel * input->header.bits_per_sample);
	const int n_frame = (total_samples + frame_length) / frame_shift + 3;
	const int n_freq = frame_length/2 + 1;


	/****************************************************************************************************/
	// Display the information.
	/****************************************************************************************************/
	if(opt.display_flag){
		cerr << "====================================" << endl;
		cerr << "Harmonic Percussive Sound Separation" << endl;
		cerr << "====================================" << endl;
		cerr << "INFORMATION on the input file" << endl;
		cerr << "Sampling Rate: "       << sampling_rate << " Hz" << endl;
		cerr << "Number of Channels: "  << I << endl;
		cerr << "Length of the Input File: " << (double)total_samples / sampling_rate << " [s] " << endl;
		cerr << "Total Samples: " << total_samples << endl;

		cerr << "===================================" << endl;
		cerr << "INFORMATION on STFT" << endl;
		cerr << "===================================" << endl;
		cerr << "n_frame: " << n_frame << endl;
		cerr << "n_freq: " << n_freq << endl;
		cerr << "frame_length: " << frame_length << endl;
		cerr << "frame_shift: " << frame_shift << endl;

		cerr << "===================================" << endl;
		cerr << "INFORMATION on parameters" << endl;
		cerr << "===================================" << endl;
		cerr << "gamma =  " << opt.gamma << endl;
		cerr << "M (= N' = K') = " << opt.M << endl;
		cerr << "w = " << opt.w << endl;
		cerr << "c = " << opt.c << endl;
		cerr << "iteration: " << opt.iteration_time << "times" << endl;
	}
	/****************************************************************************************************/
	// STFT
	/****************************************************************************************************/
	//allocation
	twoDimArray<double> abs_spec(n_frame, n_freq);
	twoDimArray<std::complex<double> > phase_spec(n_frame, n_freq);

	// exec stft
	STFT stft(frame_length, frame_shift, n_frame, opt.win_func);
	stft.exec(input, &abs_spec, &phase_spec);
	
	// gamma
	exp_spectrum(&abs_spec, opt.gamma);

	/********************************************************************************************
	Initialize HPSS
	*********************************************************************************************/
	HPSS* hpss;

	if(opt.HPSS_type == command_line_options::hm2_1)  hpss = new HPSS_HM2_1(abs_spec);
	if(opt.HPSS_type == command_line_options::hm2_2)  hpss = new HPSS_HM2_2(abs_spec);
	if(opt.HPSS_type == command_line_options::idiv)   hpss = new HPSS_IDIV (abs_spec);
	if(opt.HPSS_type == command_line_options::median) hpss = new HPSS_median (abs_spec);

	/********************************************************************************************
	Initial value and parameters
	*********************************************************************************************/
	hpss->setInitialValue (HPSS::equal);
	hpss -> set_M (opt.M);
	hpss -> set_w (opt.w);
	hpss -> set_c (opt.c);
	
	/********************************************************************************************
	update
	*********************************************************************************************/
	timer.message("start");
	// objective function
	std::vector<double> objective_function;

	//update
	for(int iter = 0; iter < opt.iteration_time; iter++){
		if(opt.display_flag){
			cerr << iter << " ";
			objective_function.push_back(hpss->objective());
		}
		hpss->update();
	}
	objective_function.push_back(hpss->objective());

	timer.message("\nHPSS updating equations Ended: ");
	if(opt.display_flag){
		for(int i = 0; i < objective_function.size(); i++){
			printf("%10.20f\n", objective_function[i]);
		}
	}

	/********************************************************************************************
	invSTFT
	*********************************************************************************************/
	// postfiltering
	HPSS::mask mask = 
		  opt.Wiener_flag == command_line_options::binary ? HPSS::binary
		: opt.Wiener_flag == command_line_options::wiener ? HPSS::wiener
		: opt.Wiener_flag == command_line_options::none   ? HPSS::none
		: HPSS::binary;
	hpss->masking(mask, opt.wiener_factor / opt.gamma);

	// get spectrograms
	twoDimArray<double> H_spec(n_frame, n_freq), P_spec(n_frame, n_freq);
	hpss->get_H(&H_spec);
	hpss->get_P(&P_spec);

	// gamma^{-1}
	exp_spectrum(&H_spec, 1. / opt.gamma);
	exp_spectrum(&P_spec, 1. / opt.gamma);

	// inv STFT
	std::vector<double> sig_H(total_samples + frame_length * 10);
	std::vector<double> sig_P(total_samples + frame_length * 10);
	invSTFT( H_spec, phase_spec, &sig_H, frame_length, frame_shift, opt.win_func);
	invSTFT( P_spec, phase_spec, &sig_P, frame_length, frame_shift, opt.win_func);

	// anti saturation
	double max = 0;
	for(int i = 0; i < total_samples; i++){
		if (max < fabs(sig_H[i]) ){
			max = fabs(sig_H[i]);
		}
		if (max < fabs(sig_P[i]) ){
			max = fabs(sig_P[i]);
		}
	}
	for(int i = 0; i < total_samples; i++){
		sig_H[i] /= max;
		sig_P[i] /= max;
	}

	/********************************************************************************************
	Save
	*********************************************************************************************/
	wavostream *Hout = new wavostream( opt.H_file_name.c_str(), 1, maximal_buffer_length);
	wavostream *Pout = new wavostream( opt.P_file_name.c_str(), 1, maximal_buffer_length);
	Hout->header = input->header;
	Pout->header = input->header;

	int processed_samples = 0;
	for(int n = 0; n < n_frame + 2; n++){
		int len;
		if(processed_samples + frame_shift < total_samples){
			len = frame_shift;
		}else{
			len = total_samples - processed_samples;
		}
		for(int j = 0; j < I; j++){
			Hout->set((&(sig_H[frame_shift * n])), j, len);
			Pout->set((&(sig_P[frame_shift * n])), j, len);
		}
		Hout->write(len);
		Pout->write(len);

		processed_samples += len;
	}
	if(opt.display_flag) cerr << "Processed Samples: " << processed_samples << endl;
	timer.message("Inverse STFT and save Time: ");


	/********************************************************************************************
	End
	*********************************************************************************************/
	if(opt.display_flag) cerr << "==================================" << endl;
	timer.message_total("Total Time: ");
	if(opt.display_flag) cerr << "==================================" << endl << endl;

	delete hpss;
	delete input;
	delete Hout;
	delete Pout;
	return 0;
}

