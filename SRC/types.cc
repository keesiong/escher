#line 281 "types.nw"
#include "types.h"
#include <stdlib.h>


#line 345 "types.nw"
void delete_type(type * x) {
        // if (x->isComposite() || x->isParameter()) assert(x->count == 0);
        if (x->count == 0) delete x; else x->deccount(); 
}
#line 516 "types.nw"
static int counter = 0;
void inc_counter() { counter++; }
int get_counter_value() { return counter; }
#line 378 "types.nw"
type_composite::~type_composite() {
        for (unsigned int i=0; i!=alpha.size(); i++) delete_type(alpha[i]);
}

#line 383 "types.nw"
void type_composite::deccount() {
        count--;
        for (unsigned int i=0; i!=alpha.size(); i++) alpha[i]->deccount();
}

#line 389 "types.nw"
void type_composite::setAlpha(type * x, unsigned int y) 
      { assert(y < alpha.size()); alpha[y] = x; }

type * type_composite::getAlpha(unsigned int x) 
      { assert(x < alpha.size()); return alpha[x]; }

string type_composite::getName() { assert(false); return ""; }
#line 407 "types.nw"
void type_composite::getParameters(set<string> & ret) {
        for (unsigned int i=0; i!=alpha.size(); i++)
                alpha[i]->getParameters(ret);
}
#line 414 "types.nw"
void type_composite::renameParameters() {
        set<string> ps; 
	getParameters(ps);
        set<string>::iterator p = ps.begin();
        while (p != ps.end()) { renameParameter(*p); inc_counter(); p++; }
}
void type_composite::renameParameter(string name) {
        for (unsigned int i=0; i!=alpha.size(); i++)
                alpha[i]->renameParameter(name);
}
#line 454 "types.nw"
#include "global.h"
static int parameterCount = 0;
type_parameter::type_parameter() {
	tag = Parameter; vname = newParameterName(); }
#line 468 "types.nw"
string newParameterName() {
 	string vname = alpha + numtostr(parameterCount++);
 	return vname;
}

#line 474 "types.nw"
void type_parameter::getParameters(set<string> & ret) {
        string temp = tag + underscore + vname; 
        ret.insert(temp); 
}

#line 480 "types.nw"
void type_parameter::renameParameters() 
      { string temp = tag+underscore+vname; renameParameter(temp); inc_counter(); }
#line 493 "types.nw"
void type_parameter::renameParameter(string name) {
        string tname = tag + underscore + vname;
        if (tname != name) return;
        char temp[10]; sprintf(temp, "_%d", get_counter_value());

	unint i = vname.rfind(underscore);
	if (i >= 0 && i < vname.size()) vname.erase(i, vname.size()-i);

        string temp2(temp); vname = vname + temp2;
}
#line 570 "types.nw"
type * type_tuple::clone() {
        type_tuple * ret = new type_tuple;
        for (int i=0; i!=alphaCount(); i++)
                ret->addAlpha(alpha[i]->clone());
        return ret;
}
#line 579 "types.nw"
string type_tuple::getName() {
        string ret = "( ";
        for (unsigned int i=0; i!=alpha.size()-1; i++)
                ret = ret + alpha[i]->getName() + " * ";
	ret = ret + alpha[alpha.size()-1]->getName() + ")";
        return ret;
}
#line 727 "types.nw"
string type_alg::getName() {
        string ret = "(" + tag;
        for (unint i=0; i!=alpha.size()-1; i++) 
                ret = ret + " " + alpha[i]->getName();
        ret = ret + " " + alpha[alpha.size()-1]->getName() + ")";
        return ret;
}
#line 622 "types.nw"
type * type_abs::clone() {
        type_abs * ret = new type_abs(alpha[0]->clone(), alpha[1]->clone());
        ret->rank = rank; 
        return ret;
}
#line 630 "types.nw"
string type_abs::getName() {
        string ret;
        if (alpha[0]->isComposite()) 
                ret = "(" + alpha[0]->getName() + ") -> ";
        else ret = alpha[0]->getName() + " -> ";
        if (alpha[1]->isComposite()) 
                ret = ret + "(" + alpha[1]->getName() + ")";
        else ret = ret + alpha[1]->getName();
        return ret;
}
#line 643 "types.nw"
type * type_abs::getArg(int n) {
        assert(n < rank);
        type * p = this;
        int temp = 0;
        while (temp != n) { p = p->getAlpha(1); temp++; }
        return p->getAlpha(0);
}
#line 653 "types.nw"
type * type_abs::getSource() {
        assert(rank != -5); 
        type * p = this; 
        for (int i=0; i!=rank; i++) p = p->getAlpha(1); 
        assert(p->getAlpha(0)); return p->getAlpha(0);
}
#line 662 "types.nw"
type * type_abs::getTarget() {
        assert(rank != -5);
        type * p = this;
        for (int i=0; i!=rank; i++) p = p->getAlpha(1);
        return p->getAlpha(1);
}
#line 677 "types.nw"
int type_abs::compRank() {
        if (alpha[1]->isAbstract() && alpha[0]->isAbstract() && 
            alpha[0]->getAlpha(1)->getTag() == gBool) {
                type_abs * t = dcast<type_abs *>(alpha[1]);
                return 1 + t->compRank();
        }
        return 0;
}
