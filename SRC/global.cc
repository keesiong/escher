#line 225 "global.nw"
#include "global.h"
#include <stdlib.h>
#include <cassert>
using namespace std;

vector<statementType *> grouped_statements;
vector<statementType *> statements;
// vector<vector<term_type> > stat_term_types;
vector<formulaType> formulas;
vector<statementType *> cachedStatements;
vector<editType *> edits;
CEG_node * condEditG = NULL, * instEditG = NULL;
set<int> cacheFuncs;
set<int> strings; // used to record strings in Bach programs

#line 259 "global.nw"
int ltime = 0; int cltime = 0;
int verbose = 0; bool typecheck = true; bool optimise = false;
bool backchain = false; bool outermost = false; bool externalIO = false;
FILE * logcache = NULL;
bool interrupted = false;
bool stepByStep = false;
vector<int> queryModalContext;
#line 287 "global.nw"
const string underscore = "_";
const string alpha = "alpha";
const string Parameter = "Parameter";
const string Tuple = "Tuple";
const string Arrow = "Arrow";
const string gBool = "Bool", gChar = "Char", gString = "String";
const string gInt = "Int", gFloat = "Float";
const string pve = "pve";
#line 302 "global.nw"
const string eqsimpl = "Equalities simplification\n";
const string andsimpl = "And rule simplification\n";
const string and2simpl = "And2 rule simplification\n";
const string ineqsimpl = "Inequalities simplification\n";
const string arsimpl = "Arithmetic simplification\n";
const string exsimpl = "Existential rule simplification\n";
const string uvsimpl = "Universal rule simplification\n";
const string betasimpl = "Beta reduction\n";
const string mathsimpl = "Math library function call\n";
const string itesimpl = "If-then-else rule simplification\n";
const string modalsimpl = "Modal term simplification\n";
#line 325 "global.nw"
const string substitutionRuleId = "=";
const string negationRuleId = "~~";
const string conjunctionRuleId = "&";
const string disjunctionRuleId = "v";
const string reflexiveRuleId = "Id";
const string existentialRuleId = "E";
const string universalRuleId = "U";
const string universalSPImpliesRuleId = "USI";
const string bachRuleId = "Bc";
const string closureRuleId = "C";
const string uclosureRuleId = "UI";
const string diamondRuleId = "<>";
const string boxRuleId = "[]";
const string kRuleId = "K";

#line 355 "global.nw"
const int iNot = 1001;        const string gNot = "not";
const int iAnd = iNot + 1;    const string gAnd = "&&";
const int iOr = iAnd + 1;     const string gOr = "||";
const int iImplies = iOr + 1; const string gImplies = "implies";
const int iIff = iImplies +1; const string gIff = "iff";
const int iPi = iIff + 1;     const string gPi = "pi";
const int iSigma = iPi + 1;   const string gSigma = "sigma";
const int iEqual = iSigma +1; const string gEqual = "=";
const int iIte = iEqual + 1;  const string gIte = "ite";
const int iTrue = iIte + 1;   const string gTrue = "True";
const int iFalse = iTrue + 1; const string gFalse = "False";
const int iHash = iFalse + 1; const string gHash = "#";
const int iEmptyList = iHash + 1;  const string gEmptyList = "[]";
const int iInfinity = iEmptyList + 1; const string gInfinity = "Infinity";
const int iAdd = iInfinity + 1;   const string gAdd = "add";
const int iSub = iAdd + 1;    const string gSub = "sub";
const int iMax = iSub + 1;    const string gMax = "max";
const int iMin = iMax + 1;    const string gMin = "min";
const int iMul = iMin + 1;    const string gMul = "mul";
const int iDiv = iMul + 1;    const string gDiv = "div";
const int iMod = iDiv + 1;    const string gMod = "mod";
const int iAtan2 = iMod + 1;  const string gAtan2 = "atan2";
const int iLT = iAtan2 + 1;   const string gLT = "<";
const int iLTE = iLT + 1;     const string gLTE = "<=";
const int iGT = iLTE + 1;     const string gGT = ">";
const int iGTE = iGT + 1;     const string gGTE = ">=";
const int iNEqual = iGTE +1;  const string gNEqual = "/=";
const int iAssign = iNEqual+1;  const string gAssign = ":=";
const int iTpHelp = iAssign+1;  const string gTpHelp = "tpHelp";
const int iTpTag = iTpHelp +1;  const string gTpTag = "TpTag";
const int iSucceeded = iTpTag+1; const string gSucceeded = "Succeeded";
const int iFailed = iSucceeded+1;  const string gFailed = "Failed";
const int iDontKnow = iFailed+1;   const string gDontKnow = "DontKnow";
const int iSin = iDontKnow+1; const string gSin = "sin";
const int iCos = iSin + 1;    const string gCos = "cos";
const int iSqrt = iCos + 1;   const string gSqrt = "sqrt";
const int iExp = iSqrt + 1;   const string gExp = "exp";
const int iUniform = iExp + 1; const string gUniform = "uniform";
const int iCategorical = iUniform+1; const string gCategorical = "categorical";
const int iGaussian = iCategorical + 1; const string gGaussian = "gaussian";
const int iPoint = iGaussian + 1; const string gPoint = "point";
const int iDGaussian = iPoint + 1; const string gDGaussian = "dgaussian";
const int iWildcard = iDGaussian + 1; const string gWildcard = "_";
#line 401 "global.nw"
vector<string> symbolsMap;
vector<string> charsMap; // characters are encoded using numbers in the range
                         // [2000,2999]
int insert_symbol(const string & symbol) {
	if (symbol[0] == '\'') {
		for (unint i=0; i!=charsMap.size(); i++) 
			if (charsMap[i] == symbol) return 2000+i;
		charsMap.push_back(symbol); 
		int csize = charsMap.size(); assert(csize <= 1000);
		return 2000+csize-1;
	}
	for (unint i=0; i!=symbolsMap.size(); i++) 
		if (symbolsMap[i] == symbol) return i+1;
	symbolsMap.push_back(symbol); 
	int csize = symbolsMap.size(); assert(csize < 1000);
	return csize;
}
#line 425 "global.nw"
const string gError = "Error";
const string & getString(int code) {
	if (0 < code && code <= (int)symbolsMap.size()) 
		return symbolsMap[code-1];
	if (2000 <= code && code < 2000+(int)charsMap.size()) 
		return charsMap[code-2000];
	switch (code) {
	case iNot: return gNot; case iAnd: return gAnd;
	case iOr: return gOr;   case iImplies: return gImplies;
	case iIff: return gIff; case iPi: return gPi;
	case iSigma: return gSigma; case iEqual: return gEqual;
	case iIte: return gIte;     case iTrue: return gTrue;
	case iFalse: return gFalse; case iHash: return gHash;
	case iEmptyList: return gEmptyList; 
	case iInfinity: return gInfinity;
	case iAdd: return gAdd;
	case iSub: return gSub; case iMax: return gMax;
	case iMin: return gMin; case iMul: return gMul;
	case iDiv: return gDiv; case iMod: return gMod;
	case iLT: return gLT;   case iLTE: return gLTE;
	case iGT: return gGT;   case iGTE: return gGTE;
	case iNEqual: return gNEqual; case iAssign: return gAssign;
	case iTpHelp: return gTpHelp; case iTpTag: return gTpTag;
	case iSucceeded: return gSucceeded; case iFailed: return gFailed;
	case iDontKnow: return gDontKnow;
	case iSin: return gSin; 
	case iCos: return gCos;
	case iSqrt: return gSqrt;
	case iExp: return gExp;
	case iAtan2: return gAtan2;
	case iUniform: return gUniform;
	case iCategorical: return gCategorical;
	case iGaussian: return gGaussian;
	case iDGaussian: return gDGaussian;
	case iPoint: return gPoint;
	case iWildcard: return gWildcard;
	}
	cerr << "code = " << code << endl; assert(false);
	return gError;
}
#line 493 "global.nw"
static unsigned int varInt = 5000;
static unsigned int uvarInt = 100000;
int newPVar() { assert(varInt < 100000); return varInt++; }
int newUVar() { return uvarInt++; }
#line 510 "global.nw"
bool typeCheck() {
	if (!typecheck) return true;
	cerr << "Type checking statements...";
	int size = grouped_statements.size();
	for (int i=0; i!=size; i++) {
		if (grouped_statements[i] == NULL) continue;
		statementType * sts = grouped_statements[i];
		while (sts != NULL) {
			if (sts->typechecked) { sts = sts->next; continue; } 
			type * res = wellTyped(sts->stmt);
			if (res) { delete_type(res); 
				   sts->typechecked = true;
			} else return false;
			sts = sts->next;
		}
	}
	
	size = statements.size();
	for (int i=0; i!=size; i++) {
		if (statements[i]->typechecked) continue; 
		type * res = wellTyped(statements[i]->stmt);
		if (res) { delete_type(res); 
			   statements[i]->typechecked = true;
		} else return false;
	}
	cerr << "done.\n";
	// cerr << "Type checking formulas...";
	size = formulas.size();
	for (int i=0; i!=size; i++) {
		if (formulas[i].typechecked) continue;
		type * res = wellTyped(formulas[i].fml);
		if (res) { delete_type(res); formulas[i].typechecked = true; }
		else return false;
	}
	// cerr << "done.\n";
	return true;
}
#line 562 "global.nw"
void cleanup_statements() {
	cerr << "Cleaning up statements...";
	for (unint i=0; i!=grouped_statements.size(); i++) {
		if (grouped_statements[i] == NULL) continue;
		statementType * sts = grouped_statements[i];
		while (sts != NULL) {
			sts->freememory();
			sts = sts->next;
		}
	}
	for (unint i=0; i!=statements.size(); i++) 
		statements[i]->freememory();
	cerr << "Done.\n";
	cerr << "Cleaning up " << cachedStatements.size() << 
		" cached statements...";
	for (unint i=0; i!=cachedStatements.size(); i++)
		cachedStatements[i]->freememory();
	cerr << "Done.\n";
}
#line 584 "global.nw"
void cleanup_formulas() {
	cerr << "Cleaning up formulas...";
	for (unint i=0; i!=formulas.size(); i++) formulas[i].freememory();
	cerr << "Done.\n";
}
#line 604 "global.nw"
struct constant_sig { int name; type * signature; };
vector<constant_sig> constants;

void initialise_constants() {
	constant_sig temp;
	temp.name = iTrue; temp.signature = new type(gBool);
	constants.push_back(temp);
	temp.name = iFalse; temp.signature = new type(gBool);
	constants.push_back(temp);	
	type * a = new type_parameter("a");
	type * lista = new type_alg("List"); lista->addAlpha(a);
	temp.name = iEmptyList; temp.signature = lista;
	constants.push_back(temp);
	temp.name = iHash; 
	temp.signature = new type_abs(a->clone(),
			     new type_abs(lista->clone(), lista->clone()));
	constants.push_back(temp);

	temp.name = iEqual; 
	temp.signature = new type_abs(a->clone(),
				 new type_abs(a->clone(), new type(gBool)));
	constants.push_back(temp);

	temp.name = iInfinity; temp.signature = new type_parameter("number");
	constants.push_back(temp);

	temp.name = iUniform; 
	temp.signature = new type_abs(lista->clone(), 
				    new type_abs(a->clone(), new type(gFloat)));
	constants.push_back(temp);

	temp.name = iCategorical;
	temp.signature = temp.signature->clone();
	constants.push_back(temp);

	temp.name = iGaussian;
	temp.signature = new type_abs(new type(gFloat),
			       new type_abs(new type(gFloat),
					    new type_abs(new type(gFloat),
							 new type(gFloat))));
	constants.push_back(temp);

	temp.name = iDGaussian;
	temp.signature = new type_abs(new type(gFloat),
			       new type_abs(new type(gFloat),
					    new type_abs(new type(gFloat),
							 new type(gFloat))));
	constants.push_back(temp);

	temp.name = iPoint;
	temp.signature = new type_abs(a->clone(), 
				    new type_abs(a->clone(), new type(gFloat)));
	constants.push_back(temp);

	
#line 665 "global.nw"
type * number = new type_parameter("number");
type * number2 = new type_parameter("number2");
type * number3 = new type_parameter("number3");
type * algtype = new type_abs(number, new type_abs(number2,number3));
temp.name = iAdd; temp.signature = algtype; constants.push_back(temp);
temp.name = iSub; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iMax; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iMin; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iMul; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iMod; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iDiv; temp.signature = algtype->clone(); constants.push_back(temp);
temp.name = iAtan2;temp.signature = algtype->clone(); constants.push_back(temp);
type * tempsig = new type_abs(number2->clone(),number3->clone()) ;
temp.name = iSin; temp.signature = tempsig; constants.push_back(temp);
temp.name = iCos; temp.signature = tempsig->clone(); constants.push_back(temp);
temp.name = iSqrt; temp.signature = tempsig->clone(); constants.push_back(temp);
temp.name = iExp; temp.signature = tempsig->clone(); constants.push_back(temp);
#line 659 "global.nw"
	
#line 685 "global.nw"
type * reltype = new type_abs(number->clone(),
			      new type_abs(number2->clone(), new type(gBool)));
temp.name = iGT; temp.signature = reltype; constants.push_back(temp);
temp.name = iGTE; temp.signature = reltype->clone(); constants.push_back(temp);
temp.name = iLT; temp.signature = reltype->clone(); constants.push_back(temp);
temp.name = iLTE; temp.signature = reltype->clone(); constants.push_back(temp);
#line 660 "global.nw"
	
#line 699 "global.nw"
temp.name = iSucceeded; temp.signature = new type("Success");
constants.push_back(temp);
temp.name = iFailed; temp.signature = new type("Success");
constants.push_back(temp);
temp.name = iAssign; 
temp.signature = new type_abs(a->clone(),
			new type_abs(a->clone(), new type("Success")));
constants.push_back(temp);
#line 661 "global.nw"
}
#line 710 "global.nw"
void insert_constant(int name, type * sig) {
	assert(name > 0);
	for (unint i=0; i!=constants.size(); i++) 
		if (constants[i].name == name) 
			{ 
#line 722 "global.nw"
int osel = getSelector(); setSelector(STDERR);
cerr << "The constant "<< getString(name) << 
	" has been defined before with type " << 
	constants[i].signature->getName() << ".\nInstruction ignored.\n"; 
setSelector(osel);
#line 714 "global.nw"
                                       return; }

	constant_sig temp; temp.name = name; temp.signature = sig;
	constants.push_back(temp);
}
#line 730 "global.nw"
type * get_signature(int name) {
	for (unint i=0; i!=constants.size(); i++)
		if (constants[i].name == name) return constants[i].signature;
	cerr << "Unknown constant: " << getString(name) << endl;
	// assert(false);
	return NULL;
}
#line 740 "global.nw"
void cleanup_constants() {
	cerr << "Cleaning up constants...";
	for (unint i=0; i!=constants.size(); i++) 
		delete_type(constants[i].signature);
	cerr << "Done.\n";
}
#line 762 "global.nw"
struct fEntry {
        int name;
        int minEffectArity;
	int maxEffectArity;
	fEntry(int n, int min, int max) {
		name = n; minEffectArity = min; maxEffectArity = max; }
};

#define TABLESIZE 501
static vector<fEntry> func_info[TABLESIZE];
#line 786 "global.nw"
static int eshash(int name) {
        // int size = name.size();
        // int ret = name[0] * name[size-1] - (name[0] + name[size-1]) + size;
        // ret = ret % TABLESIZE;
        // return ret;
	return name % TABLESIZE;
}
#line 807 "global.nw"
void initFuncTable() {
        insert_ftable(iAdd, 2); insert_ftable(iSub, 2);
        insert_ftable(iMax, 2); insert_ftable(iMin, 2);
        insert_ftable(iMul, 2); insert_ftable(iDiv, 2);
        insert_ftable(iMod, 2); insert_ftable(iAtan2, 2);
	insert_ftable(iSin, 1); insert_ftable(iCos, 1); 
	insert_ftable(iSqrt, 1); insert_ftable(iExp, 1);
        insert_ftable(iLT, 2); insert_ftable(iLTE, 2);
        insert_ftable(iGT, 2); insert_ftable(iGTE, 2);
	insert_ftable(iAssign, 2); insert_ftable(iEqual, 2);
	insert_ftable(iNEqual, 2);
	insert_ftable(iPoint, 1);
}
#line 828 "global.nw"
void insert_ftable(int func, int earity) {
        int index = eshash(func);
        int size = func_info[index].size();
        for (int i=0; i!=size; i++)
                if (func_info[index][i].name == func) {
			if (earity < func_info[index][i].minEffectArity)
				func_info[index][i].minEffectArity = earity;
			else if (earity > func_info[index][i].maxEffectArity)
				func_info[index][i].maxEffectArity = earity;
			return;
		}  
        fEntry f(func, earity, earity);
        func_info[index].push_back(f);
        // print_ftable();
}
#line 846 "global.nw"
pair<int,int> getFuncEArity(int func) { 
	assert(func > 0);
	pair<int,int> ret(-1,-1);
        int index = eshash(func);
        int size = func_info[index].size();
        for (int i=0; i!=size; i++) 
                if (func == func_info[index][i].name) {
			ret.first = func_info[index][i].minEffectArity; 
			ret.second = func_info[index][i].maxEffectArity; 
                        return ret;
		}
        cerr << "Error: Function " << getString(func) << " unknown. "
                "Effective arity could not be determined.\n";
        // assert(false); 
        return ret; 
}
#line 865 "global.nw"
void print_ftable() {
        for (int j=0; j!=TABLESIZE; j++) {
                cout << j << ": ";
                int size = func_info[j].size();
                for (int i=0; i!=size; i++)
                        cout << "(" << func_info[j][i].name << " " 
                             << func_info[j][i].minEffectArity << " "
                             << func_info[j][i].maxEffectArity << ")\t";
                cout << endl;
        }
}
#line 887 "global.nw"
set<int> nonrigid_constants;

void insert_nonrigid_constant(int name) { nonrigid_constants.insert(name); }
bool is_rigid_constant(int name) {
	return (nonrigid_constants.find(name) == nonrigid_constants.end());
}
#line 906 "global.nw"
#include <map>
static map<string, pair<int, type *> > type_fac;

void insert_type(const string & tname, int x, type * tp) {
        assert(type_fac.find(tname) == type_fac.end());
        pair<int, type *> temp(x, tp);
        type_fac[tname] = temp;
}

pair<int, type *> get_type(const string & tname) {
        map<string, pair<int,type *> >::iterator p = type_fac.find(tname);
        if (p == type_fac.end()) { pair<int,type *> ret(-5,NULL); return ret; }
        return p->second;
}

void cleanup_synonyms() {
	cerr << "Cleaning up type synonyms...";
	map<string, pair<int, type *> >::iterator p = type_fac.begin();
	while (p != type_fac.end()) { delete_type(p->second.second); p++; }
	cerr << "Done.\n";
}
#line 946 "global.nw"
void insert_statement(statementType * st) {
	assert(st->anchor >= 0);
	int gsize = grouped_statements.size();
	/* grow vector if it is not big enough */
	if (st->anchor > gsize-1)
		for (int i=0; i!=st->anchor+1; i++)
			grouped_statements.push_back(NULL);
	assert(st->anchor < (int)grouped_statements.size());
	/* insert statement */
	if (grouped_statements[st->anchor] == NULL) {
		grouped_statements[st->anchor] = st;
		return;
	}
	statementType * p = grouped_statements[st->anchor];
	while (p->next != NULL) p = p->next;
	p->next = st;
}
#line 966 "global.nw"
void print_grouped_statements() {
	setSelector(STDOUT);
	for (int i=0; i!=(int)grouped_statements.size(); i++) {
		if (grouped_statements[i] == NULL) continue;
		ioprint("*****\n");
		grouped_statements[i]->print();
		ioprint("-----\n");
	}
}
#line 987 "global.nw"
bool inVector(int x, vector<int> & v) {
	vector<int>::iterator p = find(v.begin(), v.end(), x);
	return (p != v.end());
}
bool subset(vector<int> v1, vector<int> v2) {
	int size = v1.size();
	for (int i=0; i!=size; i++) 
		if (!inVector(v1[i], v2)) return false;
	return true;
}
#line 1013 "global.nw"
vector<term *> usamplingset;
pair<term *, float> uniformSampling(term * items) {
	usamplingset.clear();
	assert(items->isApp() && items->lc()->lc()->isD(iHash));
	while (!items->isD(iEmptyList)) {
		usamplingset.push_back(items->lc()->rc());
		items = items->rc();
	} 
	int ssize = usamplingset.size();
	int i = random() % ssize;
	pair<term *, float> ret(usamplingset[i], 1.0 / ssize);
	return ret;
}
#line 1036 "global.nw"
vector<pair<term *,float> > msamplingset;
pair<term *,float> categoricalSampling(term * arg) {
	msamplingset.clear();
	float counter = 0;
	assert(arg->isApp() && arg->lc()->lc()->isD(iHash));
	while (!arg->isD(iEmptyList)) {
		pair<term *,float> element;
		element.first = arg->lc()->rc()->fields[0];
		counter += arg->lc()->rc()->fields[1]->numf;
		element.second = counter;
		msamplingset.push_back(element);
		arg = arg->rc();
	}
	assert(msamplingset.size() > 0); assert(counter == 1);
	int r = random() % 100;
	unint i = 0;
	while (r/100.0 > msamplingset[i].second) { i++; }
	float prob;
	if (i == 0) prob = msamplingset[i].second;
	else prob = msamplingset[i].second - msamplingset[i-1].second;
	pair<term *,float> ret(msamplingset[i].first,prob); 
	return ret;
}
#line 1067 "global.nw"
float gaussDens(float mu, float sigma, float x) {
	assert(sigma > 0);
	float s1 = 1.0 / (sigma * sqrt( 2 * 3.14159)); 
	float s2 = (x - mu) * (x - mu) / (2 * sigma * sigma);
	float ret = s1 * exp ( -1.0 * s2); 
	return ret;
}

pair<term *,float> ret;
pair<term *,float> gaussianSampling(term * m, term * s) {
	assert(m->isfloat && s->isfloat);
	float u1, u2, v1, v2, S;
	do {
		u1 = (random() % 100) / 100.0;
		u2 = (random() % 100) / 100.0;
		v1 = 2*u1 - 1;
		v2 = 2*u2 - 1;
		S = u1*u1 + u2*u2;
	} while (S >= 1.0);
	float x1; // float x2;
	if (S == 0) { x1 = 0; /* x2 = 0; */ } 
	else { x1 = v1 * sqrt( -2*log(S)/S );
		/* x2 = v2 * sqrt( -2*log(S)/S ); */ }

	ret.first = new_term_float(m->numf + s->numf * x1);
	ret.second = gaussDens(m->numf, s->numf, ret.first->numf);

	return ret;
}
#line 1100 "global.nw"
/* vector<pair<term *,float> > dgsamplingset;
pair<term *,float> dgaussianSampling(term * m, term * s) {
	assert(m->isfloat && s->isfloat);
	dgsamplingset.clear();

	float mu = m->numf; float sigma = s->numf;
	pair<term *,float> cent;
	cent.first = new_term_float(mu);
	cent.second = gaussDens(mu,sigma,mu);

	float total = 0;
	for (int i=0; i!=6; i++) {
		pair<term *,float> cent;
		cent.first = new_term_float(mu + i*sigma);
		cent.second = gaussDens(mu,sigma,mu + i*sigma);
		dgsamplingset.push_back(cent);
		total += cent.second;

		if (i==0) continue;
		
		cent.first = new_term_float(mu - i*sigma);
		cent.second = gaussDens(mu,sigma,mu + i*sigma);
		dgsamplingset.push_back(cent);
		total += cent.second;
	}

	float offset = 0;
	for (unint j=0; j!=dgsamplingset.size(); j++) {
		dgsamplingset[j].second = 
			offset + dgsamplingset[j].second / total;
		offset = dgsamplingset[j].second;
	}

	for (unint i=0; i!=dgsamplingset.size(); i++) {
		setSelector(STDOUT);
		dgsamplingset[i].first->print(); 
		cout << "," << dgsamplingset[i].second << " ";
		// revertSelector();
	} cout << endl; 


	int r = random() % 1000;
	unint i = 0;
	while (r/1000.0 > dgsamplingset[i].second) { i++; }
	float prob;
	if (i == 0) prob = dgsamplingset[i].second;
	else prob = dgsamplingset[i].second - dgsamplingset[i-1].second;
	pair<term *,float> ret(dgsamplingset[i].first, prob);
	return ret;
} */
#line 1157 "global.nw"
pair<term *, float> sample(term * density) {
	assert(density->isApp());
	pair<term *, float> ret(NULL,0.0);
	term * distr = density->spineTip();
	if (distr->isF(iUniform)) 
		ret = uniformSampling(density->rc());
	else if (distr->isF(iCategorical))
		ret = categoricalSampling(density->rc());
	else if (distr->isF(iGaussian)) {
		float m = density->lc()->rc()->numf;
		float s = density->rc()->numf;
		while (true) {
		     ret = gaussianSampling(density->lc()->rc(), density->rc());
		     if (fabs(ret.first->numf - m) <= 2*s) break;
		     ret.first->freememory();
		}
	}
	// else if (distr->isF(iDGaussian))
	//	ret = dgaussianSampling(density->lc()->rc(), density->rc());
	else if (distr->isF(iPoint)) {
		ret.first = density->rc()->clone(); ret.second = 1.0; }
	return ret;
}

