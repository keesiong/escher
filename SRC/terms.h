#line 4669 "ex-terms.nw"
#ifndef _TERM_H
#define _TERM_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <cassert>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "io.h"
using namespace std;

#define unint unsigned int  // defined in stdlib.h

struct term;
class type;

#line 529 "ex-terms.nw"
#define CVAR 1
#define CCONST 2
#define CEQUAL 3 
#define CNOTEQUAL 4 
#line 536 "ex-terms.nw"
struct condition { int tag; int svname; };

#line 4691 "ex-terms.nw"
typedef vector<int> occurrence;
#line 24 "ex-terms.nw"
enum kind { SV, V, F, D, APP, ABS, PROD, MODAL };

#line 1228 "ex-terms.nw"
struct substitution {
        int first; 
        term * second;
        substitution() { second = NULL; }
        substitution(int v, term * t) { first = v; second = t; }
};

#line 4694 "ex-terms.nw"
extern const string & getString(int code);
extern bool isUVar(term * t);
extern bool isUVar(int cn);

struct term {
        
#line 127 "ex-terms.nw"
bool isfloat, isint;

#line 150 "ex-terms.nw"
bool freeze;
#line 340 "ex-terms.nw"
bool redex;

#line 887 "ex-terms.nw"
bool freevars_computed; 

#line 972 "ex-terms.nw"
bool free;
bool validfree;

#line 1379 "ex-terms.nw"
bool noredex;

#line 3096 "ex-terms.nw"
bool is_data;
#line 3174 "ex-terms.nw"
bool notEnoughArgs;
#line 4700 "ex-terms.nw"
        
#line 27 "ex-terms.nw"
kind tag;
#line 38 "ex-terms.nw"
int cname;
char modality;
type * ptype;

#line 130 "ex-terms.nw"
long long int numi; 
double numf;

#line 178 "ex-terms.nw"
term * spinetip;
int spinelength;
int spine_time;

#line 539 "ex-terms.nw"
condition * cond; // only applies to SV 

#line 765 "ex-terms.nw"
int refcount;

#line 890 "ex-terms.nw"
int time_computed;

#line 3953 "ex-terms.nw"
vector<term *> preFVars;

#line 4701 "ex-terms.nw"
        term * next;
	
#line 93 "ex-terms.nw"
// vector<term *> fields;
term * fields[10];
unint fieldsize;

#line 893 "ex-terms.nw"
int frvars[20];
int frvarsize;

#line 3862 "ex-terms.nw"
vector<term **> parents;
#line 4703 "ex-terms.nw"
        
#line 70 "ex-terms.nw"
bool isF() { return (tag == F); }
bool isF(int code) { return (tag == F && cname == code); }
bool isApp() { return (tag == APP); }
bool isD() { return (tag == D); }
bool isD(int code) { return (tag == D && cname == code); }
bool isVar() { return (tag == V); }
bool isVar(int v) { return (tag == V && cname == v); }
bool isAbs() { return (tag == ABS); }
bool isProd() { return (tag == PROD); }
bool isModal() { return (tag == MODAL); }

#line 105 "ex-terms.nw"
term * lc() { /*assert(tag == APP);*/ return fields[0]; }
term * rc() { /*assert(tag == APP);*/ return fields[1]; }

void insert(term * t) {
    fields[fieldsize] = t; fieldsize++;
    if (fieldsize > 10) assert(false);
}
#line 292 "ex-terms.nw"
void initT2Args(term * t1, term * t2) { 
     lc()->insert(t1); insert(t2); 
}
#line 781 "ex-terms.nw"
term * reuse() { refcount++; return this; }
#line 785 "ex-terms.nw"
bool shared() { return (refcount > 1); }
#line 827 "ex-terms.nw"
void unshare(term * parent, unint id);
#line 1017 "ex-terms.nw"
bool isFree() { assert(tag == V && validfree); return free; }
#line 1395 "ex-terms.nw"
void setNoRedex() {
        noredex = true;
        for (unint i=0; i!=fieldsize; i++)
                fields[i]->setNoRedex();
}
#line 3085 "ex-terms.nw"
bool isData();
#line 3164 "ex-terms.nw"
bool isFuncNotRightArgs();
#line 3247 "ex-terms.nw"
bool reduce(vector<int> mpath, term * parent, unint cid, 
     	    term * root, int & tried);
bool reduce(vector<int> mpath, term * parent, unint cid, 
     	    term * root, int & tried, bool lresort);
bool reduceRpt(int maxstep, int & stepsTaken);
bool reduceRpt() { int x; return reduceRpt(0, x); }
#line 4704 "ex-terms.nw"
	term * clone();
	void freememory();
	void replace(term * t);
	bool equal(term * t);
	bool isFunc2Args();
	bool isFunc2Args(int f);
	term * spineTip();
	term * spineTip(int & x);
	bool isChar();
	bool isString();
	bool isAString();
	bool isRigid();
	void print();
	void printVertical(unint level);
        void getFreeVars();
	void unmarkValidfree();
	void labelStaticBoundVars();
	void labelBound(int x);
	bool occursFree(int var);
	bool occursFreeNaive(int var);
	bool occursFreeNaive(int var, vector<int> boundv);
	bool captured(vector<term *> & bvars, int & captd);
	void rename(int var1, int var2);
	void renameLambdaVar(int var1, int var2);
	void subst(vector<substitution> & subs);
	void subst(substitution & sub);
	void subst2(vector<substitution> & subs, vector<term *> bv,
		    term ** pointer);
	// bool containsQuantifiers();
	bool isNegation();
	bool isNegationOf(term * t2);
	bool isDiamond();
	void stripNegations();
	bool containsFreeVariable();
	void collectFreeVariables(set<int> & fvars);
	term * normalise();
	term * normalise1();
	term * normalise2();
	void collectFunctionNames(set<int> & x);
	bool termReplace(term * s, term * r, 
			 term * parent,int id);
	bool matchReplace(term * s, term * r, 
			  term * parent,int id);
	bool simplifyEquality(term * parent, unint id);
	bool simplifyArithmetic(term * parent, unint id);
	bool simplifyInequalities(term * parent, unint id);
	bool simplifyMath(term * parent, unint id);
	bool betaReduction(term * parent, unint id);
	bool simplifyIte(term * parent, unint id);
	bool simplifyConjunction();
	bool simplifyConjunction2(term * parent, unint id);
	bool simplifyExistential(term * parent, unint id);
	bool simplifyUniversal(term * parent, unint id);
	bool simplifyModalTerms(term * parent, unint id);
	term * findEq(term * root);
	bool isEq(term * root);
	bool isEq(int var);
	term * replaceEq(int var);
	void collectSharedVars();
	void shareLambdaVars(vector<term *> & lvars, bool use);
	void shareVar(term * var, term * parent, unint id);
	void shareHeadLambdaVars(vector<term *> & hlvars);
	void collectFreeVars(term * bodyparent, unint id);
	void collectLambdaVars(multiset<int> & ret);
	bool shareFreeVar(term * v, term * parent, unint id);
	bool precomputeFreeVars();
	// bool simplifyWithTP();
	// bool simplifyWithTP2();
};

#line 576 "ex-terms.nw"
extern void makeHeap();
extern term * new_term(kind k);
extern term * new_term(kind k, int code);
extern term * new_term_int(int x);
extern term * new_term_int(long long int x);
extern term * new_term_float(float x);
extern void mem_report();
#line 4775 "ex-terms.nw"
extern int newPVar();
extern int newUVar();
extern term * newT2Args(kind k, const string & f);
extern term * newT2Args(kind k, int f);

#endif
