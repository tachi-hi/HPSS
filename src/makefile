
## commands and flags
CXX = g++
CXXFLAGS = -O3 #-march=native #-g -pg #-Wall #-persist?
RM = rm -f

## Libraries
L_PTHREAD       = -lpthread
L_BOOST_OPTIONS = -lboost_program_options-mt
L_FFTW3         = -lfftw3
LIBS = $(L_PHREAD) $(L_BOOST_OPTIONS) $(L_FFTW3) $(L_PORTAUDIO) -lm


## exe files
EXE_sep  = HPSS_separation 
EXE_eval = HPSS_smoothness_eval
EXE = $(EXE_sep) $(EXE_eval)


## object files
OBJS_shared = HPSS.o STFT.o invSTFT.o twoDimArray.o objFunction.o waviostream.o HPSS_specific.o
OBJS_sep    = HPSS_separation.o 
OBJS_eval   = HPSS_smoothness_eval.o
OBJS        = $(OBJS_sep) $(OBJS_eval) $(OBJS_shared)


## all
.PHONY: all
all: $(OBJS) depend
	$(CXX) $(OMP) $(CXXFLAGS) -o $(EXE_sep) $(OBJS_sep) $(OBJS_shared) $(LIBS)
	$(CXX) $(OMP) $(CXXFLAGS) -o $(EXE_eval) $(OBJS_eval) $(OBJS_shared) $(LIBS)


## Suffix Rule
.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<


## clean
.PHONY: clean
clean: 
	$(RM) $(OBJS) $(EXE) $(ZIPFILE) depend.inc
	$(RM) *~


## header
.PHONY: depend
depend: $(OBJS:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do\
		cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc;\
	done

-include depend.inc	


