#include <iostream>
#include <csignal>
using namespace std;

#include <boost/program_options.hpp>
using namespace boost::program_options;
#include<cstdlib>
#include<cmath>
#include"win_func.h"
#include"twoDimArray.h"

/////////////////////////////////////////////////////////////////////////////////
class command_line_options{
public:
	explicit command_line_options(int argc, char **argv){
		options_description opt(
			"============================================================================\n"
			"* Harmonic/Percussive Sound Separation \n"
			"*\n"
			"* Coded by Hideyuki TACHIBANA (2012) \n"
			"* License: See http://github.com/tachi-hi\n"
			"============================================================================\n"
			"Options");
		opt.add_options()
			("help,h","Display this page")
			("input,i",   value<string>()->default_value("test.wav"), "input file")
			("Hfile,H",   value<string>()->default_value("_H.wav"), "output H file")
			("Pfile,P",   value<string>()->default_value("_P.wav"), "output P file")
			("frame,f",   value<signed int>()->default_value(512), "Frame Length of STFT")
			("shift,s",   value<signed int>()->default_value(256), "Frame Shift of STFT")
			("gamma,g",   value<double>()->default_value(0.5),   "gamma")
			("M,M",       value<int>()->default_value(10),   "M.")
			("w,w",       value<double>()->default_value(1),   "w.")
			("c,c",       value<double>()->default_value(0.01),   "c.")
			("iteration,t", value<signed int>()->default_value(30), "Number of Iterations")
			("version,v", value<string>()->default_value("idiv"), "Version of HPSS. hm2_1, hm2_2, idiv, median")
			("postprocess,p",  value<double>()->default_value(0), "postprocessing: binary mask = 0, do nothing = -1, Wiener mask : factor (> 0)")
			("display,d",  value<int>()->default_value(0), "display information: 0, do not display information: other")
			("window,W", value<string>()->default_value("sine"), "Window function: sine, hann")
			;
		variables_map vm;
		store(parse_command_line(argc, argv, opt), vm);
		notify(vm);

		if(vm.count("help") ){
			cout << opt << endl;
			exit(1);

		}else{
			iteration_time = vm["iteration"].as<int>();
			input_file_name = vm["input"].as<string>();
			H_file_name = vm["Hfile"].as<string>();
			P_file_name = vm["Pfile"].as<string>();
			frame_length = vm["frame"].as<int>();
			frame_shift = vm["shift"].as<int>();
//			frame_shift = frame_length / 2;
			gamma =  vm["gamma"].as<double>();
			M =  vm["M"].as<int>();
			if (M > _twodimarray_margin_){ 
				cerr << "Error: M is too big! It should be M << " 
					<< _twodimarray_margin_ 
					<< ", or edit _twodimarray_margin_ in twoDimArray.h" 
					<< endl;
					assert(M < _twodimarray_margin_);
			}
			c =  vm["c"].as<double>();
			w =  vm["w"].as<double>();
			Wiener_flag = 
				  vm["postprocess"].as<double> () == 0 ? binary
				: vm["postprocess"].as<double> () < 0 ? none
				: vm["postprocess"].as<double> () > 0 ? wiener
				: binary;
			wiener_factor = vm["postprocess"].as<double> ();
			HPSS_type = 
				  vm["version"].as<string>() == "hm2_1" ? hm2_1
				: vm["version"].as<string>() == "hm2_2" ? hm2_2
				: vm["version"].as<string>() == "idiv"  ? idiv
				: vm["version"].as<string>() == "median"  ? median
				: idiv ; /*default*/
			display_flag =  vm["display"].as<int>() == 0;
			win_func = 
				  vm["window"].as<string>() == "sine" ? sin_window
				: vm["window"].as<string>() == "hann" ? hanning_window
				: sin_window;
		}
	}

	enum {hm2_1, hm2_2, idiv, median} HPSS_type;
	enum {wiener, none, binary} Wiener_flag;
	int iteration_time;
	string input_file_name, H_file_name, P_file_name;
	int frame_length;
	int frame_shift;
	double (*win_func)(int, int);
	double gamma;
	int M;
	double w, c;
	double wiener_factor;
	bool display_flag;
};


