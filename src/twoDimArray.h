#pragma once
#include<cassert>

#include<iostream>
#include<string>
using namespace std;

const int _twodimarray_margin_ = 20;

template <typename T> 
class twoDimArray{
public:
	twoDimArray<T>(int t, int f) : margin(_twodimarray_margin_) {
		n_frame = t;
		n_freq = f; 
		data = new T[(n_frame + 2 * margin) * (n_freq + 2 * margin)];
		for(int i = 0; i < (n_frame + 2 * margin) * (n_freq + 2 * margin); i++){
			data[i] = 0;
		}
	}

	twoDimArray<T>(const twoDimArray<T> &r) : margin(_twodimarray_margin_) {
		n_frame = r.get_n_frame();
		n_freq  = r.get_n_freq(); 
		data = new T[(n_frame + 2 * margin) * (n_freq + 2 * margin)];
		for(int i = 0; i < (n_frame + 2 * margin) * (n_freq + 2 * margin); i++){
			data[i] = r.data[i];
		}
	}

	~twoDimArray(){
		delete[] data;
		data = NULL;
	}

	T* operator[](int n){
		return &(data[(n_freq + 2 * margin) * (margin + n) + margin]);
	}
	const T* operator[](int n) const {
		return &(data[(n_freq + 2 * margin) * (margin + n) + margin]);
	}

	const int get_n_frame(void) const {return n_frame;}
	const int get_n_freq(void)  const {return n_freq;}

private:
	const twoDimArray<T>& operator=(const twoDimArray<T> &r){};


protected:
	T* data;
	int n_freq;
	int n_frame;
	const int margin;
};

void exp_spectrum(twoDimArray<double>* spec, double gamma);



// debug
template <typename T> 
class twoDimArray_ : public twoDimArray<T>{
public:
	twoDimArray_<T>(int t, int f, const char* ch) : twoDimArray<T>(t,f){
		str = ch;
	}
	twoDimArray_<T>(const twoDimArray_<T> &r){
		str = r.str + "_";
	}
	~twoDimArray_(){
		cerr << str << endl;
	}
	std::string str;
};



