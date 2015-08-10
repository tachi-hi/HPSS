# HPSS (Harmonic/Percussive Sound Separation)

## About

This software separates a music audio signal (monaural PCM (`.wav`) format) to two components, "harmonic" and "percussive."


The codes in this repository were used in the experiments of the following paper.

- Hideyuki Tachibana, Nobutaka Ono, Hirokazu Kameoka, Shigeki Sagayama, “Harmonic Percussive Sound Separation Based on Anisotropic Smoothness of Spectrograms,” IEEE/ACM Trans. Audio, Speech and Language Processing, Vol. 22, No. 12, pp. 2059–2073, Dec. 2014.


## LICENSE

For academic use, please cite the paper above.


## Directory

+ src: source codes
+ scripts: scripts

## How to use (on UNIX-like systems)

Install `libboost_program_options-mt` and `libfftw3`.
then

    cd your_working_directory
    git clone https://github.com/tachi-hi/HPSS
    cd HPSS/src; make; cd -

Then two programs `HPSS_separation` and `HPSS_smoothness_eval` will be generated.

Note that the codes are not refactored sufficiently yet.

+ The help message of `HPSS_smoothness_eval` may not help you.

### Known bugs (to be fixed)

This program accepts only the monaural (1ch) wav files.
If you apply the program to a stereo wav file, it will emit the segmentation fault and halt.

## Other Resources
### Sliding version

Another implementation of HPSS is availabel below.
Note that the algorithm used in the program below is not identical to the one described in the paper above (and the codes in this repository).

[https://github.com/tachi-hi/slidingHPSS](https://github.com/tachi-hi/slidingHPSS)


### Windows GUI version 
GUI version (not coded by me) is available below.
Note that the algorithm used in the program below is not identical to the one described in the paper above (and the codes in this repository).

[http://hil.t.u-tokyo.ac.jp/software/HPSS/](http://hil.t.u-tokyo.ac.jp/software/HPSS/)

### Links to other softwares used in the above paper

+ [OpenBliSSART](http://openblissart.github.io/openBliSSART/)
+ [BSS EVAL](http://bass-db.gforge.inria.fr/bss_eval/)

