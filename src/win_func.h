
#pragma once
#include <cmath>

double sin_window(int i, int N){
  return sin(M_PI * (double)(i) / N);
}
double hanning_window(int i, int N){
  return 0.5 - 0.5 * cos(2 * M_PI * (double)(i) / N);
}

