#line 762 "types.nw"
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include "types.h"
#include "unification.h"
using namespace std;

bool unify_verbose = false; // set this to see the unification process
#line 781 "types.nw"
type * getBinding(vector<pair<string, type *> > & eqns, type * x) {
        assert(x->isParameter()); 
        string vname = x->getName();
        for (unsigned int i=0; i!=eqns.size(); i++)
                if (eqns[i].first == vname) return eqns[i].second;
        return x;
}
#line 796 "types.nw"
type * apply_subst(vector<pair<string, type *> > & eqns, type * t) {
        if (t->isParameter())
                return getBinding(eqns, t)->clone();
        type * ret = t->clone(); 
        for (int i=0; i!=ret->alphaCount(); i++) {
                type * temp = apply_subst(eqns, ret->getAlpha(i));
                delete_type(ret->getAlpha(i));
                ret->setAlpha(temp, i); 
        } 
        return ret;
}
#line 817 "types.nw"
bool extend(vector<pair<string, type *> > & eqns, type * x, type * t) {
        assert(x->isParameter()); 
        
#line 826 "types.nw"
if (t->isParameter())
        if (x->getName() == t->getName()) return true;
#line 820 "types.nw"
        
#line 831 "types.nw"
// case of t not a parameter
set<string> parameters; 
t->getParameters(parameters);
// set<string>::iterator p = parameters.begin();
// cout << "parameters : ";
// while (p != parameters.end()) { cout << *p << " "; p++; }
if (parameters.find(x->getName()) != parameters.end())
        return false;
#line 821 "types.nw"
        
#line 842 "types.nw"
for (unsigned int i=0; i!=eqns.size(); i++) {
        type * temp = eqns[i].second;
        eqns[i].second = apply_subst(eqns, temp);
        delete_type(temp);
} 
pair<string, type *> eqn(x->getName(), t->clone()); 
eqns.push_back(eqn);
return true;
#line 822 "types.nw"
}
#line 858 "types.nw"
type * get_type_from_syn(type * in) {
        type * ret = in; 
        while (ret->isSynonym()) 
                ret = dcast<type_synonym *>(ret)->getActual();
        return ret;
}
#line 876 "types.nw"
bool unify(vector<pair<string,type *> > &eqns, type * tvn, type * t) {
	
#line 938 "types.nw"
if (unify_verbose) 
	cerr << "Unifying " << tvn->getName() << " and " << t->getName() <<endl;
#line 878 "types.nw"
        if (tvn->isSynonym()) tvn = get_type_from_syn(tvn);
        if (t->isSynonym()) t = get_type_from_syn(t);
	
#line 943 "types.nw"
if (unify_verbose) cerr << "After transformation:\n";
#line 938 "types.nw"
if (unify_verbose) 
	cerr << "Unifying " << tvn->getName() << " and " << t->getName() <<endl;

#line 882 "types.nw"
        bool ret = false;
        if (tvn->isParameter()) {
                type * phitvn = getBinding(eqns, tvn)->clone();

                type * phit = apply_subst(eqns, t); 
                // if phitvn == tvn
                if (phitvn->isParameter()) {
                        if (tvn->getName() == phitvn->getName()) {
                                ret = extend(eqns, tvn, phit);
                                delete_type(phit); delete_type(phitvn);
				if (unify_verbose) cerr << ret << endl;
                                return ret;
                        }
                } else { 
                        ret = unify(eqns, phitvn, phit);
                        delete_type(phit); delete_type(phitvn);
			if (unify_verbose) cerr << ret << endl;
                        return ret; 
                }
        }
        // switch place
        if (tvn->isParameter() == false && t->isParameter()) 
                return unify(eqns, t, tvn);

        
#line 912 "types.nw"
if (tvn->isParameter() == false && t->isParameter() == false) {
        if (tvn->getTag() != t->getTag()) return false;
        if (tvn->getTag() == Tuple && t->getTag() == Tuple)
                if (tvn->alphaCount() != t->alphaCount()) {
			if (unify_verbose) cerr << false << endl;
                        return false; 
		}
        // unify each component
        if (tvn->alphaCount() != t->alphaCount()) {
		cerr << "Error in unification. Argument counts don't match.\n";
		cerr << "tvn = " << tvn->getName() << endl;
		cerr << "  t = " << t->getName() << endl;
		assert(false);
	}
        for (int i=0; i!=tvn->alphaCount(); i++) {
                bool r = unify(eqns,tvn->getAlpha(i),t->getAlpha(i));
                if (r == false) return false;
        }
}
#line 907 "types.nw"
        return true;
}
#line 1308 "types.nw"
#include <string>
#include <vector>
#include "global.h"
#include "terms.h"

#line 998 "types.nw"
vector<term_type> term_types;
struct var_name { int vname; string pname; };
#line 1264 "types.nw"
void printErrorMsg(term * t) {
	int osel = getSelector();
	setSelector(STDERR); t->print(); 
	ioprintln(" is not well typed."); setSelector(osel);
}
#line 1277 "types.nw"
void print_term_types() {
	int osel = getSelector(); setSelector(STDOUT);
	ioprintln(" *** ");
	for (unint i=0; i!=term_types.size(); i++) {
		term_types[i].first->print();
		ioprint(" : "); ioprintln(term_types[i].second->getName());
	}
	setSelector(osel);
}
#line 1294 "types.nw"
void cleanup_term_types() {
	// print_term_types();
	for (unint i=0; i!=term_types.size(); i++)
		delete_type(term_types[i].second);
	term_types.clear();
}
#line 977 "types.nw"
type * wellTyped2(term * t, vector<var_name> bvars, int scope) {
	type * ret = NULL;
	
#line 1022 "types.nw"
if (t->isF() || t->isD()) {
	if (t->isint) ret = new type(gInt);
	else if (t->isfloat) ret = new type(gFloat);
	else if (t->isChar()) ret = new type(gChar);
	else if (t->isString()) ret = new type(gString);
	else {  
		ret = get_signature(t->cname);
		if (ret) { ret = ret->clone(); ret->renameParameters(); }
		else return NULL;
	}
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1033 "types.nw"
}
#line 980 "types.nw"
	
#line 1071 "types.nw"
if (t->isVar()) {
	if (t->cname == iWildcard) {
		ret = new type_parameter();
		
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1075 "types.nw"
	}
	unint start = 0;
	for (int i=(int)bvars.size()-1; i!=-1; i--) 
		if (t->cname == bvars[i].vname) {
			start = scope;
			
#line 1102 "types.nw"
for (unint j=start; j!=term_types.size(); j++) 
	if (term_types[j].first->isVar()) 
		if (t->cname == term_types[j].first->cname) {
			ret = term_types[j].second->clone();
			
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1107 "types.nw"
		}
#line 1081 "types.nw"
			ret = new type_parameter(bvars[i].pname);
			
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1083 "types.nw"
		}
	
#line 1102 "types.nw"
for (unint j=start; j!=term_types.size(); j++) 
	if (term_types[j].first->isVar()) 
		if (t->cname == term_types[j].first->cname) {
			ret = term_types[j].second->clone();
			
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1107 "types.nw"
		}
#line 1084 "types.nw"
                                               
	ret = new type_parameter();
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1087 "types.nw"
}
if (t->tag == SV) {
	for (unint j=0; j!=term_types.size(); j++) 
		if (term_types[j].first->tag == SV) {
			if (t->cname == term_types[j].first->cname) {
				ret = term_types[j].second->clone();
				
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1094 "types.nw"
			}
		}
	ret = new type_parameter();
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1098 "types.nw"
}
#line 981 "types.nw"
	
#line 1124 "types.nw"
if (t->isApp()) {
	unsigned int vlength = term_types.size();
	type * t1 = wellTyped2(t->lc(), bvars, scope); 
	if (t1->isSynonym()) t1 = get_type_from_syn(t1);
	
#line 1160 "types.nw"
if (!t1) { printErrorMsg(t->lc()); return NULL; }

if (!t1->isAbstract() && t1->isParameter()) {
	type * temp = t1;
	t1 = new type_abs(temp, new type_parameter());
	term_types[term_types.size()-1].second = t1;
}
if (!t1->isAbstract()) {
	int osel = getSelector();
	setSelector(STDERR); ioprint("*** Error: "); 
	t->lc()->print(); ioprint(" : "); ioprintln(t1->getName());
	ioprintln(" does not have function type.");
	setSelector(osel);
	return NULL;
}
#line 1129 "types.nw"
	type * t2 = wellTyped2(t->rc(), bvars, scope);
	if (!t2) { printErrorMsg(t->rc()); return NULL; }

	vector<pair<string, type *> > slns; 
	bool result = unify(slns, t1->getAlpha(0), t2);
	if (!result) { 
#line 1183 "types.nw"
int osel = getSelector();
setSelector(STDERR); t->print(); ioprintln(" is not well typed.");
ioprint(t1->getAlpha(0)->getName()); ioprint(" and ");
ioprint(t2->getName()); ioprintln(" are not unifiable\n");
slns.clear();
unify_verbose = true;
unify(slns, t1->getAlpha(0), t2);
setSelector(osel); unify_verbose = false;
return NULL;
#line 1134 "types.nw"
                                                              }
	ret = apply_subst(slns, t1->getAlpha(1));

	for (unint i=vlength; i!=term_types.size(); i++) {
		type * temp = term_types[i].second;
		term_types[i].second = apply_subst(slns, temp);
		delete_type(temp);
	}
	for (unint j=0; j!=slns.size(); j++) delete_type(slns[j].second);
	slns.clear();
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1145 "types.nw"
}
#line 982 "types.nw"
        
#line 1210 "types.nw"
if (t->isAbs()) {
	unint vlength = term_types.size();

	var_name tmp; tmp.vname = t->fields[0]->cname; 
	tmp.pname = newParameterName();
	bvars.push_back(tmp);

	type * t2 = wellTyped2(t->fields[1], bvars, vlength); 
	if (!t2) { printErrorMsg(t); return NULL; }

	type * vt = NULL;
	for (unint i=vlength; i!=term_types.size(); i++) 
		if (term_types[i].first->isVar(t->fields[0]->cname)) 
		      {	vt = term_types[i].second->clone(); break; }
	if (vt == NULL) { vt = new type_parameter(tmp.pname); } 

	ret = new type_abs(vt, t2->clone()); 
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1228 "types.nw"
}
#line 983 "types.nw"
	
#line 1238 "types.nw"
if (t->isModal()) {
        type * ret = wellTyped2(t->fields[0], bvars, scope);
	if (!ret) { printErrorMsg(t); return NULL; }
	ret = ret->clone();
        
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1243 "types.nw"
}
#line 984 "types.nw"
        
#line 1252 "types.nw"
if (t->isProd()) {
	ret = new type_tuple; 
	for (unsigned int i=0; i!=t->fieldsize; i++) {
		type * ti = wellTyped2(t->fields[i], bvars, scope);
		if (!ti) { printErrorMsg(t); return NULL; }
		ret->addAlpha(ti->clone());
	}
	
#line 1045 "types.nw"
term_type res; res.first = t; res.second = ret; 
term_types.push_back(res);
if (t->ptype) delete_type(t->ptype);
t->ptype = ret->clone();
return ret;
#line 1260 "types.nw"
}
#line 985 "types.nw"
	return ret;
}

#line 1317 "types.nw"
type * wellTyped(term * t) {
	vector<var_name> bvars;
	type * ret = wellTyped2(t, bvars, 0);
	if (!ret) { printErrorMsg(t); return NULL; }
	ret = ret->clone();
	cleanup_term_types();
	return ret;
}
#line 1335 "types.nw"
pair<type *, vector<term_type> > mywellTyped(term * t) {
	pair<type *, vector<term_type> > res;
	vector<var_name> bvars;
	type * ret = wellTyped2(t, bvars, 0);
	if (!ret) { printErrorMsg(t); res.first = NULL; return res; }
	ret = ret->clone();
	res.first = ret; res.second = term_types;
	term_types.clear();
	return res;
}
