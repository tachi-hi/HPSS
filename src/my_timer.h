#pragma once
#include <cstdio>
#include <ctime>

class myTimer{
public:
  myTimer(){
    start = (double)clock();
    prev_call = start;
  }

  double time_since_previous_call(void){
    prev_prev_call = prev_call;
    prev_call = (double)clock();
    return (prev_call - prev_prev_call)/CLOCKS_PER_SEC;
  }

  double total_time(void){
    double tmp = (double)clock();
    return (tmp - start)/CLOCKS_PER_SEC;
  }

  void message(const char* msg){
		fprintf(stderr, "%s %d [ms]\n", msg, (int)(this->time_since_previous_call() * 1000) );
  }

  void message_total(const char* msg){
		fprintf(stderr, "%s %d [ms]\n", msg, (int)(this->total_time() * 1000 ) );
  }

private:
  double start;
  double prev_call;
  double prev_prev_call;
};

