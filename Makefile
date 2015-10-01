# Alkemy Makefile
# Author : Kee Siong NG
# Last Modified : 26 October 2005

# Use
#   brew install noweb
# to install noweb and icon.

# Documentation part
DOCOFILES = escher.tex 

FORCE :

DOCFILES = escher.tex introduction.tex terms.tex types.tex \
	parsing.tex io.tex global.tex interpreter.tex programming.tex \
	sys-modules.tex escher.idx escher.aux escher.bbl escher.blg \
	escher.ilg escher.ind escher.log escher.toc \
	escher.defs introduction.defs terms.defs types.defs \
	parsing.defs io.defs global.defs interpreter.defs \
	programming.defs sys-modules.defs all.defs

NOWOPTS = -delay -indexfrom all.defs -filter ./dpp

doco : glue
	nodefs escher.nw > escher.defs
	nodefs introduction.nw > introduction.defs
	nodefs ex-terms.nw > terms.defs
	nodefs types.nw > types.defs 
	nodefs parsing.nw > parsing.defs
	nodefs io.nw > io.defs
	nodefs global.nw > global.defs
	nodefs interpreter.nw > interpreter.defs
	nodefs programming.nw > programming.defs
	nodefs sys-modules.nw > sys-modules.defs
#	nodefs ../Bach/tp.nw > tp.defs
	sort -u *.defs | cpif all.defs
	noweave $(NOWOPTS) escher.nw > escher.tex
	noweave $(NOWOPTS) introduction.nw > introduction.tex
	noweave $(NOWOPTS) ex-terms.nw > terms.tex
	noweave $(NOWOPTS) types.nw > types.tex
	noweave -delay -index parsing.nw > parsing.tex
	noweave $(NOWOPTS) io.nw > io.tex
	noweave $(NOWOPTS) global.nw > global.tex
	noweave $(NOWOPTS) interpreter.nw > interpreter.tex
#	noweave $(NOWOPTS) ../Bach/tp.nw > tp.tex
	noweave -delay -index programming.nw > programming.tex
	noweave -delay -index sys-modules.nw > sys-modules.tex
	pdflatex escher.tex; bibtex escher; makeindex escher.idx; pdflatex escher.tex; pdflatex escher.tex
	pdflatex escher.tex
	mv $(DOCFILES) TEXFILES
	rm -f ex-terms.nw

clean_doco :
	rm -f escher.aux escher.bbl escher.blg escher.log escher.toc


# source part

CPP = g++ -Wall -ansi -O2

mymacro : mymacro.cc
	g++ -Wall mymacro.cc -o mymacro

source : interpreter.nw glue
	notangle -L -Rterms.h ex-terms.nw | cpif SRC/terms.h
	notangle -L -Rterms.cc ex-terms.nw | cpif SRC/terms.cc
	notangle -L -Rpattern-match.h ex-terms.nw | cpif SRC/pattern-match.h
	notangle -L -Rpattern-match.cc ex-terms.nw | cpif SRC/pattern-match.cc
	notangle -L -Rio.h io.nw | cpif SRC/io.h
	notangle -L -Rio.cc io.nw | cpif SRC/io.cc
	notangle -L -Rglobal.h global.nw | cpif SRC/global.h
	notangle -L -Rglobal.cc global.nw | cpif SRC/global.cc
	notangle -L -Rtypes.h types.nw | cpif SRC/types.h
	notangle -L -Rtypes.cc types.nw | cpif SRC/types.cc
	notangle -L -Runification.h types.nw | cpif SRC/unification.h
	notangle -L -Runification.cc types.nw | cpif SRC/unification.cc
#	notangle -L -Rlabel.h ../Bach/tp.nw | cpif SRC/label.h
#	notangle -L -Rlabel.cc ../Bach/tp.nw | cpif SRC/label.cc
#	notangle -L -RaccessibilityRelation.h ../Bach/tp.nw | cpif SRC/accessibilityRelation.h
#	notangle -L -RaccessibilityRelation.cc ../Bach/tp.nw | cpif SRC/accessibilityRelation.cc
#	notangle -L -Rtableau.h ../Bach/tp.nw | cpif SRC/tableau.h
#	notangle -L -Rtableau.cc ../Bach/tp.nw | cpif SRC/tableau.cc
#	notangle -L -Rtableaux.h ../Bach/tp.nw | cpif SRC/tableaux.h
#	notangle -L -Rtableaux.cc ../Bach/tp.nw | cpif SRC/tableaux.cc
#	notangle -L -Rconstraints-solver.h ../Bach/tp.nw | cpif SRC/constraints-solver.h
#	notangle -L -Rconstraints-solver.cc ../Bach/tp.nw | cpif SRC/constraints-solver.cc
	notangle -Rescher-parser.y parsing.nw | cpif SRC/escher-parser.y
	notangle -Rescher-scan.l parsing.nw | cpif SRC/escher-scan.l
	notangle -Rbooleans.es sys-modules.nw | cpif booleans.es
	notangle -Rlists.es sys-modules.nw | cpif lists.es
	notangle -Rsets.es sys-modules.nw | cpif sets.es
	notangle -Rnumbers.es sys-modules.nw | cpif numbers.es
	notangle -Rdata.es programming.nw | cpif data.es
	notangle -Rqueries.es programming.nw | cpif queries.es
	rm -f ex-terms.nw

glue : terms.nw redexes.nw
	./mymacro < terms.nw > ex-terms.nw

escher : FORCE
	cd SRC; make lex.yy.c; make y.tab.c; g++ -MM -I. *.cc *.c > .depend; make escher




