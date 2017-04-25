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

### Libraries
Install `boost` and `fftw3` as follows,

    # apt
    sudo apt-get install libboost-dev libfftw3-dev
    
    # yum
    sudo yum install boost-devel fftw3-devel
    
### Build

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
### Other HPSS Implementation
#### Sliding version

Another implementation of HPSS is availabel below.
Note that the algorithm used in the program below is not identical to the one described in the paper above (and the codes in this repository).

[https://github.com/tachi-hi/slidingHPSS](https://github.com/tachi-hi/slidingHPSS)


#### Windows GUI version (not available now)
GUI version (not coded by me) ~~is~~ <font color="Red">was</font> available below.
Note that the algorithm used in the program below is not identical to the one described in the paper above (and the codes in this repository).

- ~~http://hil.t.u-tokyo.ac.jp/downloads~~ ... link expired

#### Audacity 
see http://ismir2015.uma.es/LBD/LBD16.pdf

#### librosa

[librosa](http://librosa.github.io/librosa/), a audio processing module for python, also contains a module named [hpss](http://librosa.github.io/librosa/generated/librosa.decompose.hpss.html), though the algorithm is different from the one described in the above paper.

### Links to other softwares used in the above paper

+ [OpenBliSSART](http://openblissart.github.io/openBliSSART/)
+ [BSS EVAL](http://bass-db.gforge.inria.fr/bss_eval/)

