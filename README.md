# HPSS (Harmonic/Percussive Sound Separation)

[![](https://github.com/tachi-hi/HPSS/workflows/build/badge.svg)](https://github.com/tachi-hi/HPSS/actions?query=workflow%3Abuild)
[![](https://github.com/tachi-hi/HPSS/workflows/LaTeXplot/badge.svg)](https://github.com/tachi-hi/HPSS/actions?query=workflow%3ALaTeXplot)
[![](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)


## About

This software separates a music audio signal (monaural PCM (`.wav`) format) to two components, "harmonic" and "percussive."


The codes in this repository were used in the experiments of the following paper. [[IEEE Xplore](https://doi.org/10.1109/TASLP.2014.2351131)]

```bibtex
@article{tachibana2014hpss,
  author={Hideyuki Tachibana and Nobutaka Ono and Hirokazu Kameoka and Shigeki Sagayama},
  journal={IEEE/ACM Transactions on Audio, Speech, and Language Processing},
  title={Harmonic/Percussive Sound Separation Based on Anisotropic Smoothness of Spectrograms},
  year={2014},
  volume={22},
  number={12},
  pages={2059-2073},
}
```

## LICENSE

- [MIT](LICENSE)
- For academic use, please cite the paper above.

## Directory

+ src: source codes
+ scripts: scripts

## How to use (on UNIX-like systems)

### Quick Build (recommended)

    git clone https://github.com/tachi-hi/HPSS
    cd HPSS
    ./build.sh

This script automatically handles everything: installing CMake (if needed), fetching [vcpkg](https://github.com/microsoft/vcpkg), resolving dependencies (`boost`, `fftw3`), and building the project.

Two executables will be generated in `build/`:
- `build/HPSS_separation`
- `build/HPSS_smoothness_eval`

Other build commands:

    ./build.sh debug     # Debug build
    ./build.sh clean     # Remove build artifacts

### Manual Build (with system libraries)

If you prefer to install dependencies manually:

    # Debian/Ubuntu
    sudo apt-get install cmake libboost-program-options-dev libfftw3-dev

    # macOS (Homebrew)
    brew install cmake boost fftw

Then build with CMake:

    cmake -B build
    cmake --build build

### Legacy Build (Makefile)

The original Makefile is still available in `src/`:

    sudo apt-get install libboost-dev libfftw3-dev
    cd HPSS/src; make; cd -

### Notes

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

- ~~http://hil.t.u-tokyo.ac.jp/pages/downloads.html~~ ... link expired

#### Audacity
see http://ismir2015.uma.es/LBD/LBD16.pdf

#### librosa

[librosa](http://librosa.github.io/librosa/), a audio processing module for python, also contains a module named [hpss](http://librosa.github.io/librosa/generated/librosa.decompose.hpss.html), though the algorithm is different from the one described in the above paper.

### Links to other softwares used in the above paper

+ [OpenBliSSART](http://openblissart.github.io/openBliSSART/)
+ [BSS EVAL](http://bass-db.gforge.inria.fr/bss_eval/)
