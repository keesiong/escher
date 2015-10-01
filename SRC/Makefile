# Alkemy Makefile
# Author : Kee Siong NG
# Last Modified : 9 June 2006

# Documentation part
DOCOFILES = escher.tex 

FORCE :

CPP = g++ -Wall -ansi -O2
# This next one is for profiling.
# CPP = g++ -Wall -ansi -g -pg 

OBJFILES = io.o terms.o types.o unification.o pattern-match.o global.o \
	lex.yy.o y.tab.o

SRCFILES = io.cc terms.cc types.cc unification.cc pattern-match.cc global.cc \
	lex.yy.c y.tab.c

lex.yy.c : escher-scan.l
	flex escher-scan.l

y.tab.c : escher-parser.y
	yacc -d escher-parser.y

lex.yy.o : lex.yy.c
	g++ -ansi -Os -c lex.yy.c

y.tab.o : y.tab.c terms.h terms.cc
	$(CPP) -c y.tab.c

include .depend

.cc.o:
	$(CPP) -c $<

escher : $(OBJFILES)
	rm -f terms.o; $(CPP) -DESCHER -c terms.cc
	$(CPP) -o escher $(OBJFILES); mv escher ..

hal : $(OBJFILES)
	rm -f terms.o; $(CPP) -DHAL -c terms.cc
	$(CPP) -o hal $(OBJFILES); mv hal ..

bach : $(OBJFILES)
	rm -f terms.o; $(CPP) -DBACH -c terms.cc
	$(CPP) -o bach $(OBJFILES); mv bach ..

leak-escher : $(SRCFILES)
	g++ -Wall -ansi -g -o escher $(SRCFILES) LeakTracer.o

typeChecker : tc.tab.o tc.lex.yy.o terms.o pattern-match.o io.o global.o
	$(CPP) -o typeChecker tc.tab.o tc.lex.yy.o terms.o pattern-match.o io.o global.o

clean :
	rm -f *.o escher lex.yy.c y.tab.c tc.tab.c tc.lex.yy.c


