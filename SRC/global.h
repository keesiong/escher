#line 4 "global.nw"
#ifndef _ESCHER_GLOBAL_H_
#define _ESCHER_GLOBAL_H_

#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <math.h>
#include <stdlib.h>
#include "terms.h"
#include "types.h"
#include "unification.h"
using namespace std;

#line 48 "global.nw"
// these are the escher statements
struct statementType {
	vector<int> modalContext; // this is used in Bach only
	vector<int> quantifiedVars; // this is used in Bach only
        term * stmt;
        int numargs;
        int anchor;
	bool typechecked;
	bool lastresort;
	bool eager;
	bool persistent;
	bool noredex;
	bool collectstats; int usestats;
	statementType * next;
	statementType() { 
		anchor = -5; typechecked = false; lastresort = false; 
		eager = false; persistent = false; noredex = false;
		collectstats = false; usestats = 0;
		next = NULL;
	}
	void freememory() { stmt->freememory(); }
	void print() { stmt->print(); ioprintln(); if (next) next->print(); }
};
extern vector<statementType *> grouped_statements;
extern vector<statementType *> statements;
extern vector<statementType *> cachedStatements;
#line 77 "global.nw"
struct formulaType {
	term * fml;
	bool globalass;
	bool typechecked;
	formulaType() { globalass = false; typechecked = false; }
	void freememory() { fml->freememory(); }
};
extern vector<formulaType> formulas;
#line 92 "global.nw"
struct BN_node { 
	int vname; term * density; BN_node * next; 
	BN_node() { density = NULL; next = NULL; }
	BN_node * clone() { 
#line 158 "global.nw"
BN_node * ret = new BN_node;
ret->vname = vname; ret->density = density->clone();
if (next) ret->next = next->clone();
return ret;
#line 95 "global.nw"
                                       }
	void freememory() { 
		if (density) density->freememory(); 
		if (next) next->freememory(); }
	void print() { 
#line 146 "global.nw"
ioprint(getString(vname)); ioprint(" ~ "); density->print();
if (next != NULL) { ioprint(", "); next->print(); }
#line 99 "global.nw"
                                  }
	void subst(vector<substitution> & theta) {
		density->subst(theta); if (next) next->subst(theta); }
};
#line 106 "global.nw"
struct editType {
	term * head; 
	term * body;
	type * htype;
	string htype_name;
	BN_node * bnodes;
	editType * next;
	editType() { head = NULL; body = NULL; bnodes = NULL; next = NULL; }
	editType * clone() { 
#line 170 "global.nw"
editType * ret = new editType;
ret->head = head->clone(); ret->body = body->clone();
ret->head->labelStaticBoundVars(); ret->body->labelStaticBoundVars(); 
ret->htype = htype->clone(); 
if (bnodes) ret->bnodes = bnodes->clone();
if (next) ret->next = next->clone();
return ret;
#line 114 "global.nw"
                                         }
	void freememory() { 
#line 180 "global.nw"
if (head) head->freememory();
if (body) body->freememory();
if (htype) delete_type(htype);
if (bnodes) bnodes->freememory();
if (next) next->freememory();
#line 115 "global.nw"
                                             }
	void subst(vector<substitution> & theta) { 
#line 129 "global.nw"
// make sure theta does not bound variables in bnodes
for (unint i=0; i!=theta.size(); i++) {
	BN_node * pt = bnodes;
	while (pt!=NULL) { assert(theta[i].first != pt->vname); pt = pt->next; }
}
// do the substitution
body->subst(theta); 
if (bnodes) bnodes->subst(theta);
if (next) next->subst(theta);
#line 116 "global.nw"
                                                               }
	void print() { 
#line 151 "global.nw"
head->print(); ioprint(" ~> "); body->print(); 
if (bnodes != NULL) { ioprint(" ["); bnodes->print(); ioprint(" ]"); }
ioprint(" of type "); ioprint(htype->getName()); ioprint("\n");
if (next) next->print();
#line 117 "global.nw"
                                   }
};
extern vector<editType *> edits;
#line 192 "global.nw"
struct CEG_node {
	term * cond;
	editType * editg;
	CEG_node * lt, * rt;
	CEG_node() { cond = NULL; editg = NULL; lt = NULL; rt = NULL; }
	CEG_node * clone() {
		CEG_node * ret = new CEG_node;
		if (cond != NULL) ret->cond = cond->clone();
		if (editg != NULL) ret->editg = editg->clone();
		if (lt != NULL) { ret->lt = lt->clone(); ret->rt = rt->clone();}
		return ret;
	}
	void freememory() {
		if (cond) cond->freememory();
		if (editg) editg->freememory();
		if (lt) lt->freememory();
		if (rt) rt->freememory();
	}
	void print() {
		setSelector(STDERR);
		if (cond) {
			ioprint("if "); cond->print(); ioprint(" then \n");
			lt->print(); ioprint("\n");
			ioprint("else\n");
			rt->print(); ioprint("\n");
		} else editg->print();
	}
};
extern CEG_node * condEditG;
extern CEG_node * instEditG;
#line 269 "global.nw"
extern int ltime; extern int cltime;
/* options */
extern int verbose; extern bool typecheck; 
extern bool optimise; extern bool backchain; extern bool outermost;
extern bool externalIO;
extern vector<int> queryModalContext;

extern bool interrupted; 
extern bool stepByStep;
/* log files */
extern FILE * logcache;
#line 316 "global.nw"
extern const string eqsimpl, andsimpl, and2simpl, ineqsimpl, arsimpl,
	exsimpl, uvsimpl, betasimpl, mathsimpl, itesimpl, modalsimpl;
#line 342 "global.nw"
extern const string substitutionRuleId, negationRuleId, conjunctionRuleId,
	disjunctionRuleId, reflexiveRuleId, kRuleId,
	existentialRuleId, universalRuleId, universalSPImpliesRuleId, 
	bachRuleId, closureRuleId, uclosureRuleId, diamondRuleId, boxRuleId;
#line 476 "global.nw"
/* symbol table */
extern int insert_symbol(const string & symbol);
extern const string & getString(int code);
#line 550 "global.nw"
extern bool typeCheck();
#line 594 "global.nw"
extern void cleanup_statements();
extern void cleanup_formulas();
#line 749 "global.nw"
extern void initialise_constants();
extern void insert_constant(int name, type * sig);
extern type * get_signature(int name);
extern void cleanup_constants();
#line 879 "global.nw"
/* function symbol table */
extern void initFuncTable();
extern void insert_ftable(int func, int earity);
extern pair<int,int> getFuncEArity(int func);
extern void print_ftable();
#line 896 "global.nw"
extern void insert_nonrigid_constant(int name);
extern bool is_rigid_constant(int name);
#line 932 "global.nw"
extern void insert_type(const string & tname, int x, type * tp);
extern pair<int, type *> get_type(const string & tname);
extern void cleanup_synonyms();
#line 978 "global.nw"
extern void insert_statement(statementType * st);
extern void print_grouped_statements();
#line 1000 "global.nw"
bool inVector(int x, vector<int> & v);
bool subset(vector<int> v1, vector<int> v2);
#line 1183 "global.nw"
pair<term *, float> sample(term * density);
#line 1192 "global.nw"
#include <sstream>
inline string numtostr(const int i) { stringstream s; s << i; return s.str(); }
inline string numtostr(const double i) { stringstream s; s << i; return s.str(); }

#line 22 "global.nw"
// extern vector<vector<term_type> > stat_term_types;
extern set<int> cacheFuncs;
extern set<int> strings;
extern const string pve;

#line 468 "global.nw"
extern const int iNot, iAnd, iOr, iImplies, iIff, iPi, iSigma, iEqual, iIte,
   iTrue, iFalse, iHash, iEmptyList, iInfinity, iAdd, iSub, iMax, iMin, iMul, 
   iDiv, iMod, iLT, iLTE, iGT, iGTE, iNEqual, iAssign, iTpHelp, iTpTag, 
   iSucceeded, iFailed, iDontKnow, iSin, iCos, iSqrt, iExp, iAtan2, 
   iUniform, iCategorical, iGaussian, iDGaussian, iPoint, iWildcard;

#line 29 "global.nw"
#define UDEFINED 0
#define SYNONYM  1

#endif
