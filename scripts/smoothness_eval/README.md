# Smoothness Eval

## Experiment

You first need to do following.

+ Build the HPSS in this repository, and place the obtained executable code in the suitable directory.
+ Install `sox`
+ Place some `.wav` files in the directory `./experiment/wav`

Then, move to the directory `./experiment`, run the `./script.sh`, and a `.csv` file will be created.
You have to edit this file manually suitably.
An example of the edited `.csv` file is `./plot/result.csv`

## Plot

You need to install following.

+ `R`, and `ggplot2`
+ `latex`, `dvi2ps`, `psfrag`

Then run `script.sh`.
The obtained `convertfig.eps` is the figure we needed.



