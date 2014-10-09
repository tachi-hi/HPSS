#pragma once
#include<fftw3.h>
#include<complex>
#include<iostream>
#include<cstdlib>

////////////////////////////////////////////////////////////////////////////////////////
class MyFFT{
public:
  typedef enum{forward,backward} FLAG;
  MyFFT(FLAG flag, int size)
  {
    real = new double[size];
    cmplx = new std::complex<double> [size/2+1];
    if(flag == forward){
      p = fftw_plan_dft_r2c_1d(
                          size,
                          real,
                          reinterpret_cast<fftw_complex*>(cmplx),
                          FFTW_ESTIMATE);
      in = real;
      out = cmplx;
    }else if(flag == backward){
      p = fftw_plan_dft_c2r_1d(
                          size,
                          reinterpret_cast<fftw_complex*>(cmplx),
                          real,
                          FFTW_ESTIMATE);
      in = cmplx;
      out = real;
    }else{
      std::cerr << "FFT error" << std::endl;
      exit(1);
    }
  }
  ~MyFFT(){
    fftw_destroy_plan(p);
    delete[] real;
    delete[] cmplx;
  }
  void exec(){
    fftw_execute(p);
  }
  void *in;
  void *out;
private:
  fftw_plan p;
  double *real;
  std::complex<double> *cmplx;
};

