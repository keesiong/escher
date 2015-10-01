#line 4801 "ex-terms.nw"
#include <iostream>
#include <utility>
#include <vector>
#include "io.h"
#include "pattern-match.h"
#include "global.h"

#line 4145 "ex-terms.nw"
bool redex_match(term * head, term * body, vector<substitution> & theta) {
        vector<term *> bindingAbss;
        return redex_match(head, body, theta, bindingAbss, head);
}
#line 4152 "ex-terms.nw"
bool redex_match(term * head, term * body, vector<substitution> & theta, 
		 vector<term *> bindingAbss, term * orig_head) {
        kind head_tag = head->tag;
        kind term_tag = body->tag;

        if (head_tag == SV) { 
#line 4195 "ex-terms.nw"
condition * constraint = head->cond;
if (constraint) {
        int ctag = constraint->tag;

        if (ctag == CVAR && term_tag != V) return false; // problematic?
        else if (ctag == CCONST && term_tag != D && term_tag != F) return false;
        else if (ctag == CEQUAL) {
                // if (term_tag != D && term_tag != V) return false;
                term * bound = findBinding(constraint->svname, theta);
                
#line 4217 "ex-terms.nw"
if (bound == NULL) {
        setSelector(STDERR);
        ioprint("The constraint EQUAL or NOTEQUAL on syntactical "
                "variables is used incorrectly; it appears before "
                "its argument is instantiated.\n");
        assert(false);
}
#line 4205 "ex-terms.nw"
                if (body->equal(bound) == false) return false;
        } else if (ctag == CNOTEQUAL) {
                // if (term_tag != D) return false;
                term * bound = findBinding(constraint->svname, theta);
                
#line 4217 "ex-terms.nw"
if (bound == NULL) {
        setSelector(STDERR);
        ioprint("The constraint EQUAL or NOTEQUAL on syntactical "
                "variables is used incorrectly; it appears before "
                "its argument is instantiated.\n");
        assert(false);
}
#line 4210 "ex-terms.nw"
                if (body->equal(bound) == true) return false;
        }
        // assert(ctag != CVAR); assert(ctag != CNOTEQUAL);
}
#line 4178 "ex-terms.nw"
substitution sub(head->cname, body);
theta.push_back(sub);
return true;
#line 4157 "ex-terms.nw"
                                                           }
        if (head_tag == V) { 
#line 4234 "ex-terms.nw"
if (term_tag == V && head->cname == body->cname) return true;
if (head->isFree() == false) return false;
if (head->cname == iWildcard) return true;
                
#line 4251 "ex-terms.nw"
int captd;
if (body->captured(bindingAbss, captd)) {
        setSelector(STDERR);
        cerr <<" ** Matching Failed: Free variable capture in redex-match.\n"; 
        ioprint("head = "); head->print(); ioprintln();
        ioprint("term = "); body->print(); ioprintln();
	assert(orig_head);
	ioprint("orig head = "); orig_head->print(); ioprintln();
        return false; }

#line 4240 "ex-terms.nw"
substitution sub(head->cname, body);
theta.push_back(sub);
return true;
#line 4158 "ex-terms.nw"
                                                        }
        if (head_tag != term_tag) return false;

        
#line 4267 "ex-terms.nw"
if (head_tag == F) return (head->cname == body->cname);
if (head_tag == D) {
	if (head->isfloat != body->isfloat) return false;
	if (head->isint != body->isint) return false;
	if (head->isfloat && body->isfloat) return (head->numf == body->numf);
	else if (head->isint && body->isint) return (head->numi == body->numi);
	return (head->cname == body->cname);
}
#line 4162 "ex-terms.nw"
        if (head_tag == APP) { 
#line 4284 "ex-terms.nw"
if (!redex_match(head->lc(),body->lc(), theta, bindingAbss, orig_head)) 
	return false;
return redex_match(head->rc(), body->rc(),theta,bindingAbss,orig_head); 
#line 4162 "ex-terms.nw"
                                                            }
        if (head_tag == PROD) { 
#line 4317 "ex-terms.nw"
unint size = head->fieldsize;
if (size != body->fieldsize) return false;

for (unint i=0; i!=size; i++)
	if (!redex_match(head->fields[i],body->fields[i],theta,
			 bindingAbss,orig_head)) 
                return false;
return true;
#line 4163 "ex-terms.nw"
                                                              }
        if (head_tag == ABS) { 
#line 4334 "ex-terms.nw"
if (head->fields[0]->tag == SV) { 
      redex_match(head->fields[0],body->fields[0],theta,bindingAbss,orig_head);
 } else { 
#line 4355 "ex-terms.nw"
int term_var = body->fields[0]->cname;
if (head->fields[0]->cname != term_var) {
        int size = head->preFVars.size();
        for (int i=0; i!=size; i++) 
	        if (term_var == head->preFVars[i]->cname) { 
                        
#line 4368 "ex-terms.nw"
int osel = getSelector(); setSelector(STDOUT);
ioprint(" ** Trouble. Variable "); head->preFVars[i]->print();
ioprint(" captured after lambda variable renaming.\n");
ioprint("head = "); head->print(); ioprintln();
ioprint("term = "); body->print(); ioprintln();
setSelector(osel);
#line 4361 "ex-terms.nw"
			head->preFVars[i]->cname = newPVar();
                }
	head->fields[0]->cname = term_var;
}
#line 4336 "ex-terms.nw"
                                                             }
bindingAbss.push_back(head);
return redex_match(head->fields[1],body->fields[1],theta,bindingAbss,orig_head);
#line 4164 "ex-terms.nw"
                                                            }
        if (head_tag == MODAL) { 
#line 4377 "ex-terms.nw"
if (head->modality != body->modality) return false;
return redex_match(head->fields[0],body->fields[0],theta,bindingAbss,orig_head);
#line 4165 "ex-terms.nw"
                                                                }
        assert(false); return false;
}
#line 4533 "ex-terms.nw"
bool freevar_match(term * fml1, term * fml2,
		   vector<substitution> & theta) {
        vector<term *> bindingAbss;
        return freevar_match(fml1, fml2, theta, bindingAbss);
}
#line 4542 "ex-terms.nw"
bool freevar_match(term * head, term * body, vector<substitution> & theta, 
		   vector<term *> bindingAbss) {
        kind head_tag = head->tag;
        kind term_tag = body->tag;
	assert(head_tag != SV /*&& head_tag != MODAL*/);
	if (head_tag == V) {
		if (isUVar(head->cname) && !body->occursFree(head->cname))
			{ term * orig_head = NULL; 
			  head->validfree = true; head->free = true; 
			  
#line 4234 "ex-terms.nw"
if (term_tag == V && head->cname == body->cname) return true;
if (head->isFree() == false) return false;
if (head->cname == iWildcard) return true;
                
#line 4251 "ex-terms.nw"
int captd;
if (body->captured(bindingAbss, captd)) {
        setSelector(STDERR);
        cerr <<" ** Matching Failed: Free variable capture in redex-match.\n"; 
        ioprint("head = "); head->print(); ioprintln();
        ioprint("term = "); body->print(); ioprintln();
	assert(orig_head);
	ioprint("orig head = "); orig_head->print(); ioprintln();
        return false; }

#line 4240 "ex-terms.nw"
substitution sub(head->cname, body);
theta.push_back(sub);
return true;
#line 4551 "ex-terms.nw"
                                }
	} 
	if (term_tag == V) {
		if (isUVar(body->cname) && !head->occursFree(body->cname))
			{ term * headtemp = head;
			  head = body;
			  body = headtemp;
			  term * orig_head = NULL;
			  head->validfree = true; head->free = true; 
			  
#line 4234 "ex-terms.nw"
if (term_tag == V && head->cname == body->cname) return true;
if (head->isFree() == false) return false;
if (head->cname == iWildcard) return true;
                
#line 4251 "ex-terms.nw"
int captd;
if (body->captured(bindingAbss, captd)) {
        setSelector(STDERR);
        cerr <<" ** Matching Failed: Free variable capture in redex-match.\n"; 
        ioprint("head = "); head->print(); ioprintln();
        ioprint("term = "); body->print(); ioprintln();
	assert(orig_head);
	ioprint("orig head = "); orig_head->print(); ioprintln();
        return false; }

#line 4240 "ex-terms.nw"
substitution sub(head->cname, body);
theta.push_back(sub);
return true;
#line 4561 "ex-terms.nw"
			}
	}
	if (head_tag != term_tag) return false;
	if (head_tag == V && term_tag == V) {
		if (head->cname != body->cname) return false;
		return true;
	}

	
#line 4267 "ex-terms.nw"
if (head_tag == F) return (head->cname == body->cname);
if (head_tag == D) {
	if (head->isfloat != body->isfloat) return false;
	if (head->isint != body->isint) return false;
	if (head->isfloat && body->isfloat) return (head->numf == body->numf);
	else if (head->isint && body->isint) return (head->numi == body->numi);
	return (head->cname == body->cname);
}
#line 4570 "ex-terms.nw"
	if (head_tag == APP) { 
#line 4599 "ex-terms.nw"
if (!freevar_match(head->lc(),body->lc(),theta,bindingAbss)) return false;
return freevar_match(head->rc(), body->rc(), theta, bindingAbss); 
#line 4570 "ex-terms.nw"
                                                       }
        if (head_tag == PROD) { 
#line 4609 "ex-terms.nw"
unint size = head->fieldsize;
if (size != body->fieldsize) return false;

for (unint i=0; i!=size; i++) {
	setSelector(SILENT); 
	ioprint("unifying "); head->fields[i]->print(); 
	ioprint(" and "); body->fields[i]->print(); ioprint(" ");
        if (!freevar_match(head->fields[i],body->fields[i],theta,bindingAbss)){ 
		setSelector(SILENT); ioprint(" false\n"); setSelector(SILENT);
                return false;
	}
	setSelector(SILENT); ioprint(" true\n"); setSelector(SILENT);
}
return true;
#line 4571 "ex-terms.nw"
                                                                }
        if (head_tag == ABS) { 
#line 4589 "ex-terms.nw"
if (head->fields[0]->cname != body->fields[0]->cname) return false;
bindingAbss.push_back(head);
return freevar_match(head->fields[1], body->fields[1], theta, bindingAbss);
#line 4572 "ex-terms.nw"
                                                              }
	if (head_tag == MODAL) { 
#line 4604 "ex-terms.nw"
if (head->modality != body->modality) return false;
return freevar_match(head->fields[0], body->fields[0],theta, bindingAbss);
#line 4573 "ex-terms.nw"
                                                           }
        assert(false); return false;
}
#line 4631 "ex-terms.nw"
void printTheta(vector<substitution> & theta) {
        if (getSelector() == SILENT) return;
        ioprint('{');
        int size = theta.size();
        if (size == 0) { ioprint("}\n"); return; }
        for (int i=0; i!=size-1; i++) {
                ioprint('('); 
		if (theta[i].first >= 5000) { 
			ioprint(pve); ioprint(theta[i].first-5000); }
		else ioprint(getString(theta[i].first)); 
		ioprint('/');
                theta[i].second->print(); ioprint("), ");
        }
        ioprint('(');
	if (theta[size-1].first >= 5000) { 
		ioprint(pve); ioprint(theta[size-1].first-5000); }
	else ioprint(getString(theta[size-1].first)); 
	ioprint('/');
        theta[size-1].second->print(); ioprint(')');
        ioprint("}\n");
}
#line 4659 "ex-terms.nw"
term * findBinding(int svname, vector<substitution> & theta) {
        int size = theta.size();
        for (int i=0; i!=size; i++)
                if (theta[i].first == svname) return theta[i].second;
        return NULL;
}
