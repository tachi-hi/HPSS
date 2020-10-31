all:
	R --vanilla --slave < smoothness.R
	latex convertfig
	dvipdf convertfig
	mv convertfig.pdf smoothness.pdf

clean:
	rm -f *.dvi *.aux *.fdb_latexmk *.fls *.log *.eps *.pdf
