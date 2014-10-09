#include"trapfpe.c"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <complex>
#include <algorithm>

#include "waviostream.hpp"
#include "HPSS_smoothness_eval.h"

#include "my_timer.h"

#include "STFT.h"
#include "invSTFT.h"

#include "objFunction.h"

int main(int argc, char **argv){
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
	const int n_frame = total_samples / frame_shift + 3;
	const int n_freq = frame_length/2 + 1;


	/****************************************************************************************************/
	// Display the information.
	/****************************************************************************************************/
	if(opt.display_flag){
		cerr << "====================================" << endl;
		cerr << "SMOOTHNESS EVALUATION" << endl;
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

	cerr << "temporal  Smoothness function (4): " << smoothness_time(abs_spec, n_frame, n_freq, opt.M) << endl;
	cerr << "frequency Smoothness function (5): " << smoothness_freq(abs_spec, n_frame, n_freq, opt.M) << endl;
	cerr << "sum of energy: " << simple_sum(abs_spec, n_frame, n_freq) << endl;
	cerr << "Notes:" << endl;
	cerr << "\tThe values are small, the spectrogram is smooth." << endl;
	cerr << "\tThe scale of the values depends on not only the smoothness, but also the volume and the length of the input signal" << endl;
	cerr << "\tThe program can be used just for comparing temporal and frequency smoothness of a signal." << endl;
	cout << smoothness_time(abs_spec, n_frame, n_freq, opt.M) / simple_sum(abs_spec, n_frame, n_freq) << endl;
	cout << smoothness_freq(abs_spec, n_frame, n_freq, opt.M) / simple_sum(abs_spec, n_frame, n_freq) << endl;
	delete input;
	return 0;
}

