#line 253 "types.nw"
#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#include <set>
#include <vector>
#include <string>
#include <assert.h>
#include <iostream>
using namespace std;
#define dcast dynamic_cast
#define unint unsigned int

extern const string underscore, alpha, Parameter, Tuple, Arrow, 
		    gBool, gInt, gFloat, gChar, gString;

#line 511 "types.nw"
void inc_counter();
int get_counter_value();
#line 304 "types.nw"
class type {
public:
        int count;
        type() { count = 0; }
        type(string n) : tag(n) { count = 0; }
        virtual ~type() {}
        virtual void setAlpha(type * x, unsigned int y) {}
        virtual void addAlpha(type * x) {}
        virtual type * getAlpha(unsigned int x) { return NULL; }
        virtual int alphaCount() { return 0; }
        virtual bool isComposite() { return false; }
        virtual bool isTuple() { return false; }
        virtual bool isAbstract() { return false; }
        virtual bool isParameter() { return false; }
        virtual bool isSynonym() { return false; }
        virtual bool isUdefined() { return false; }
        virtual string getName() { return tag; }
        virtual string & getTag() { return tag; }
        virtual type * clone() { count++; return this; }
        virtual void deccount() { count--; }
        virtual void getParameters(set<string> & ret) {}
        virtual void renameParameters() {}
        virtual void renameParameter(string name) {}
protected:      
        string tag;
};
#line 341 "types.nw"
void delete_type(type * x);
#line 358 "types.nw"
class type_composite : public type {
protected:
        vector<type *> alpha;
public:
        virtual ~type_composite(); 
        bool isComposite() { return true; }
        virtual void deccount();
        virtual void setAlpha(type * x, unsigned int y); 
        virtual void addAlpha(type * x) { alpha.push_back(x); }
        virtual type * getAlpha(unsigned int x);  
        virtual int alphaCount() { return alpha.size(); }
        virtual string getName();
        virtual type * clone() { assert(false); }
        virtual void getParameters(set<string> & x);
        virtual void renameParameters();
        virtual void renameParameter(string name);
};
#line 431 "types.nw"
class type_parameter : public type {
public: type_parameter();
	type_parameter(string x) { tag = Parameter; vname = x; }
        type * clone() { return new type_parameter(vname); }
        bool isParameter() { return true; }
        string getName() { return tag + underscore + vname; }
        void getParameters(set<string> & ret); 
        void renameParameters();
        void renameParameter(string name);
private:
        string vname;
};

extern string newParameterName();
#line 560 "types.nw"
class type_tuple : public type_composite {
public:
        type_tuple() { tag = Tuple; }
        type * clone();
        bool isTuple() { return true; }
        string getName();
};
#line 695 "types.nw"
class type_udefined : public type {
        const vector<string> values;
public:
        type_udefined(string & tname, const vector<string> &vals) 
                : type(tname), values(vals) {}
        type_udefined(string & tname) : type(tname) {}
	bool isUdefined() { return true; }
        // type * clone() { count++; return this; }
        const vector<string> & getValues() { return values; }
};
#line 708 "types.nw"
class type_alg : public type_composite {
public:
        type_alg(string tid) { tag = tid; }
        type_alg(string tid, vector<type *> x) {
                tag = tid; 
                for (unsigned int i=0; i!=x.size(); i++) 
                        addAlpha(x[i]->clone()); 
        }
        type_alg(string tid, type_tuple * x) {
                tag = tid;
                for (int i=0; i!=x->alphaCount(); i++)
                        addAlpha(x->getAlpha(i)->clone());
        }
        type * clone() { return new type_alg(tag, alpha); }
        string getName();
};
#line 603 "types.nw"
class type_abs : public type_composite {
public:
        int rank;
        type_abs() { tag = Arrow; rank = -5; }
	type_abs(type * source, type * target) {
		tag = Arrow; rank = -5;
		addAlpha(source); addAlpha(target);
	}
        bool isAbstract() { return true; }
        type * clone();
        type * getArg(int n);
        type * getSource();
        type * getTarget();
        string getName();
        int compRank();
};
#line 530 "types.nw"
class type_synonym : public type {
public:  
        type_synonym(string name, type * ac)  
              { tag = name; tname = name; actual = ac; }
        ~type_synonym() { delete_type(actual); }
        type * clone() { 
		// assert(actual); count++; actual->count++; return this; } 
		assert(actual);return new type_synonym(tname,actual->clone());}
        void deccount() { assert(false); }
        bool isSynonym() { return true; }
        type * getActual() { return actual; }
	string getName() { return actual->getName(); }
private: 
        type * actual;
        string tname;
};

#line 277 "types.nw"
#endif
