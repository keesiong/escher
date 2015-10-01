#line 4784 "ex-terms.nw"
#include "terms.h"
#line 279 "ex-terms.nw"
term * newT2Args(kind k, int f) {
        term * ret = new_term(APP);
        ret->insert(new_term(APP)); ret->lc()->insert(new_term(k, f));
        return ret;
}
#line 586 "ex-terms.nw"
#ifdef DEBUG_MEM
static long int allocated = 0;
static long int freed = 0;
#endif

#define HEAPSIZE 100000
term heap[HEAPSIZE];
term * avail;

void makeHeap() {
    // cout << "Sizeof(term) = " << sizeof(term) << endl;
    // cout << "Sizeof(char) = " << sizeof(char) << endl;
    // cout << "Sizeof(short) = " << sizeof(short) << endl;     
    // cout << "Sizeof(int) = " << sizeof(int) << endl;
    // cout << "Sizeof(bool) = " << sizeof(bool) << endl;
    avail = heap;
    for (int i=0; i!=HEAPSIZE-1; i++) {
	heap[i].next = &(heap[i+1]);
    }
    heap[HEAPSIZE-1].next = NULL;
}

term * myalloc() {
    if (avail == NULL) assert(false);
    term * ret = avail; avail = avail->next; 
    
#line 48 "ex-terms.nw"
ret->cname = -5;
ret->modality = -5;
ret->ptype = NULL;

#line 101 "ex-terms.nw"
ret->fieldsize = 0;

#line 137 "ex-terms.nw"
ret->isfloat = false; ret->isint = false;

#line 157 "ex-terms.nw"
ret->freeze = false;

#line 187 "ex-terms.nw"
ret->spinetip = NULL; ret->spinelength = -1; ret->spine_time = -5;
#line 347 "ex-terms.nw"
ret->redex = false;
#line 545 "ex-terms.nw"
ret->cond = NULL;

#line 772 "ex-terms.nw"
ret->refcount = 1;
#line 903 "ex-terms.nw"
ret->frvarsize = 0;
ret->freevars_computed = false;
ret->time_computed = -5;
#line 980 "ex-terms.nw"
ret->validfree = false;
#line 1385 "ex-terms.nw"
ret->noredex = false;

#line 3104 "ex-terms.nw"
ret->is_data = false;
#line 3182 "ex-terms.nw"
ret->notEnoughArgs = false;
#line 611 "ex-terms.nw"
                       
    return ret;
}
inline void mydealloc(term * p) { p->next = avail; avail = p; }
#line 618 "ex-terms.nw"
term * new_term(kind k) {
     term * ret = myalloc(); ret->tag = k; 
     return ret;
}

term * new_term(kind k, int code) {
     term * ret = myalloc(); 
     ret->tag = k; 
     ret->cname = code;
     return ret;
}

term * new_term_int(int x) {
    term * ret = myalloc();
    ret->tag = D; 
    ret->isint = true; ret->numi = x; return ret;
}

term * new_term_int(long long int x) {
    term * ret = myalloc();
    ret->tag = D; 
    ret->isint = true; ret->numi = x; return ret;
}

term * new_term_float(float x) {
     term * ret = myalloc();
     ret->tag = D;
     ret->isfloat = true; ret->numf = x; return ret;
}
#line 650 "ex-terms.nw"
inline void delete_term(term * x) { mydealloc(x); }

void mem_report() {
#ifdef DEBUG_MEM
        cout << "\n\nReport from Memory Manager:\n";
        cout << "\tAllocated " << allocated << endl;
        cout << "\tFreed " << freed << endl;
        cout << "\tUnaccounted " << allocated - freed << endl << endl;
#endif
} // >>
#line 3367 "ex-terms.nw"
#include "global.h"
#include "pattern-match.h"
static int nestingdepth = 0;
#line 3375 "ex-terms.nw"
bool do_local_search = true;
bool try_match_n_reduce(vector<int> mpath, term * candidate, 
     			term * parent,unint cid, term * root, 
			int & tried, bool lastresort) {
	vector<substitution> theta; /* this cannot be made global because of 
				       eager statements */
        
#line 3689 "ex-terms.nw"
if (verbose == 3) {
        setSelector(STDOUT);
        ioprint("Trying to redex match "); candidate->print(); ioprintln();
}
#line 3382 "ex-terms.nw"
	
#line 3547 "ex-terms.nw"
int msg = -5;
if (candidate->isFunc2Args()) {
        int f = candidate->spineTip()->cname;
        if (f == iEqual) {
                if (candidate->simplifyEquality(parent, cid)) 
			{ msg = 1; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3552 "ex-terms.nw"
                               }
        } else if (f == iAnd) {
                if (candidate->simplifyConjunction()) 
			{ msg = 2; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3555 "ex-terms.nw"
                               }
                if (candidate->simplifyConjunction2(parent, cid)) 
			{ msg = 3; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3557 "ex-terms.nw"
                               }
        }
        if (candidate->simplifyInequalities(parent, cid)) 
                { msg = 4; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3560 "ex-terms.nw"
                                            }

        if (candidate->simplifyArithmetic(parent, cid)) 
		{ msg = 5; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3563 "ex-terms.nw"
                              }
}
if (candidate->isApp()) {
        if (candidate->simplifyExistential(parent, cid)) 
		{ msg = 6; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3567 "ex-terms.nw"
                              }

        if (candidate->simplifyUniversal(parent, cid)) 
                { msg = 7; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3570 "ex-terms.nw"
                                            }

        if (candidate->betaReduction(parent, cid)) 
		{ msg = 8; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3573 "ex-terms.nw"
                              }

	if (candidate->simplifyMath(parent, cid))
		{ msg = 9; 
#line 3587 "ex-terms.nw"
ltime++;
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
	int osel = getSelector(); setSelector(STDOUT);
	ioprint("Time = "); ioprintln(ltime);
	switch (msg) {
	case 1: ioprint(eqsimpl); break;
	case 2: ioprint(andsimpl); break;
	case 3: ioprint(and2simpl); break;
	case 4: ioprint(ineqsimpl); break;
	case 5: ioprint(arsimpl); break;
	case 6: ioprint(exsimpl); break;
	case 7: ioprint(uvsimpl); break;
	case 8: ioprint(betasimpl); break;
	case 9: ioprint(mathsimpl); break;
	}
	candidate->redex = true; 
	ioprint("Answer: "); root->print(); ioprint("\n\n"); 
	candidate->redex = false; setSelector(osel); 
 }
return true;
#line 3576 "ex-terms.nw"
                              }
}
#line 3383 "ex-terms.nw"
        
#line 3453 "ex-terms.nw"
int cs_size = cachedStatements.size();
for (int j=0; j!=cs_size; j++) {
	term * head = cachedStatements[j]->stmt->lc()->rc();
	term * body = cachedStatements[j]->stmt->rc();
	theta.clear(); // vector<substitution> theta;
	if (candidate->equal(head) || redex_match(head, candidate, theta)) {
		// cerr << "Using cached computation." << endl;
		term * temp = body->clone();
		if (theta.size()) { temp->subst(theta); }
		ltime++; cltime++; 
		
#line 3477 "ex-terms.nw"
if (parent) { parent->fields[cid] = temp; candidate->freememory(); }
else { candidate->replace(temp); temp->freememory(); }
#line 3464 "ex-terms.nw"
		return true;
	} 
}
#line 3384 "ex-terms.nw"
			
        candidate->spineTip();
	if (!candidate->spinetip->isF()) return false;
	int anchor = candidate->spinetip->cname;
	if (anchor >= (int)grouped_statements.size()) return false;
	statementType * sts = grouped_statements[anchor];
	if (sts == NULL) return false;
	while (sts != NULL) {
		if (sts->lastresort != lastresort) { sts = sts->next; continue;}
		if ((candidate->spinelength -1) != sts->numargs) 
			{ sts = sts->next; continue; }
		theta.clear();
		term * head = sts->stmt->lc()->rc();
		term * body = sts->stmt->rc();
		
#line 3696 "ex-terms.nw"
if (verbose == 3) { ioprint("\tand "); head->print(); ioprint(" ... "); }
#line 3399 "ex-terms.nw"
		if (redex_match(head, candidate, theta)) {
                        
#line 3620 "ex-terms.nw"
if (sts->eager && candidate->isApp()) {
	/* try reduce the arguments first, return
	   true if any one can be reduced */
	for (int i=candidate->spinelength-1; i!=0; i--) {
		/* go to argument spinelength - i argument */
		term * arg = candidate;
		for (int j=1; j!=i; j++) arg = arg->lc();
		if (arg->rc()->reduce(mpath,arg,1,root, tried)) 
			return true;
	}
}
#line 3401 "ex-terms.nw"
                        ltime++;
                        
#line 3674 "ex-terms.nw"
#ifdef MAIN_DEBUG1

term * head1 = head->clone();
head1->subst(theta);
head1->applySubst();
assert(head1->equal(candidate));

#endif
#line 3700 "ex-terms.nw"
if (verbose == 3) ioprint("\t[succeed]\n");
#line 3646 "ex-terms.nw"
	      //if (verbose && ltime % 100 == 0) {
if (verbose) {
        int osel = getSelector(); setSelector(STDOUT);
        ioprint("Time = "); ioprintln(ltime);
        ioprint("Matched "); head->print(); ioprintln(); // ioprint(" and ");
        // // candidate->print();
        // // ioprint("\nReplacing with "); body->print(); ioprint(' '); 
        // // printTheta(theta);

        candidate->redex = true;
        ioprint("Query:  "); root->print(); ioprintln();
        candidate->redex = false; setSelector(osel);
} 
#line 3403 "ex-terms.nw"
			term * temp = NULL;
			if (sts->noredex) temp = body->reuse();
			else {
				temp = body->clone();
				temp->subst(theta);
				
#line 3496 "ex-terms.nw"
#ifdef ESCHER
// do_local_search = false;
if (!outermost && do_local_search && !lastresort && temp->tag != D) {
	// cerr << "reduce temp to simplest form\n";
	do_local_search = false;	
	term * temp3 = NULL; // term * temp2 = NULL; 
	if (optimise) { // temp2 = head->clone(); temp2->subst(theta); 
		        // temp2->unshare(NULL, 1); 
		        temp3 = temp->clone(); }
        nestingdepth++;
	int time_old = ltime;
        temp->unshare(NULL, 1); // we should not need to do this operation
	bool reduced = true;
	int interval = 0; // this makes sure the local operation doesn't go 
	                  // too deep, which can happen if we do this too early
	while (reduced) {
                reduced = temp->reduce(mpath,NULL,0, temp,tried);
		if (temp->tag == D) break;
		if (interval++ > 500) break;
	}
        nestingdepth--;
	if (optimise) { 
#line 3524 "ex-terms.nw"
if (ltime - time_old > 30 && head->isApp() && 
    cacheFuncs.find(head->lc()->cname) != cacheFuncs.end()) {
	// int osel = getSelector(); setSelector(STDERR);
	//   temp3->print(); ioprint(" = "); temp->print();
	//   ioprint("; -- "); ioprint(ltime-time_old); ioprintln();
	//   ioprintln(); setSelector(osel);
	statementType * st = new statementType();
	st->stmt = newT2Args(F, iEqual); 
	st->stmt->initT2Args(temp3, temp->clone());
	temp3->labelStaticBoundVars(); // temp->labelStaticBoundVars();
	st->stmt->collectSharedVars();
	cachedStatements.push_back(st);
} else temp3->freememory();
#line 3517 "ex-terms.nw"
                                                  }
	do_local_search = true;
}
#endif
#line 3409 "ex-terms.nw"
			}
			
#line 3477 "ex-terms.nw"
if (parent) { parent->fields[cid] = temp; candidate->freememory(); }
else { candidate->replace(temp); temp->freememory(); }
#line 3411 "ex-terms.nw"
			
#line 3662 "ex-terms.nw"
//if (verbose && ltime % 100 == 0) { 
if (verbose) {
        int osel = getSelector(); setSelector(STDOUT);     
        ioprint("Answer: "); root->print(); ioprint("\n\n"); setSelector(osel);}
#line 3412 "ex-terms.nw"
                        return true;
		}
		
#line 3704 "ex-terms.nw"
if (verbose == 3) ioprint("\t[failed]\n");
#line 3415 "ex-terms.nw"
		sts = sts->next;
	}
	 
	/* int bm_size = statements.size(); 
        for (int j=0; j<bm_size; j++) {
		if (statements[j].lastresort != lastresort) continue;
                <try match::find special cases where no matching is required>
                theta.clear();
                term * head = statements[j].stmt->lc()->rc();
                term * body = statements[j].stmt->rc();
                <debug matching 2>
		if (redex_match(head, candidate, theta)) {
                        <try match::eager statements>
			ltime++;
                        <try match::unimportant things>
                        term * temp = body->clone();
                        temp->subst(theta);
			<try match::reduce temp to simplest form>
			<try match::put reduct in place>
			<try match::output answer>
                        return true;
		}
                <debug matching 4>
	} */
        return false;
}
#line 3712 "ex-terms.nw"
/*
bool try_disruptive(vector<int> mpath, term * candidate, 
     		    term * parent, unint cid, term * root, 
		    int & tried) {
	int osel = getSelector(); setSelector(SILENT);
	if (candidate->isFunc2Args(iAssign)) {
		term * arg1 = candidate->lc()->rc();
		// reduce arg2
		ioprint("simplifying "); candidate->rc()->print(); ioprintln();
		int tried2 = 0;
                bool reduced = true;
                while (reduced) { 
                        reduced = candidate->rc()->reduce(mpath,NULL, 
				  	       0, root, tried2);
                        if (candidate->rc()->tag == D) break;
                };
		ioprint("result = "); candidate->rc()->print(); ioprintln();
		// update statement 
		for (unint i=0; i!=statements.size(); i++) {
		    if (statements[i].anchor == arg1->cname) {
		       	    assert(statements[i].persistent);
		            statements[i].stmt->rc()->freememory();
			    statements[i].stmt->fields[1] = candidate->rc();

			    statements[i].stmt->print(); ioprintln();
			    setSelector(osel);
			    break;
		    }
		}
		// candidate = Succeed
		candidate->fields[1] = NULL;
		term * temp = new_term(D, iSucceeded);
		if (parent) { parent->fields[cid] = temp;
		              candidate->freememory(); }
		else { candidate->replace(temp); temp->freememory(); }
		return true;
	}
	return false;
}
*/
#line 199 "ex-terms.nw"
term * term::spineTip() {
	if (spinetip && spinelength > -1 && spine_time ==ltime) return spinetip;
	spine_time = ltime; 
        if (tag != APP) { spinetip = this; spinelength = 1; return spinetip; }
        spinelength = 2; spinetip = fields[0]; 
        while (spinetip->isApp())  
		{ spinetip = spinetip->fields[0]; spinelength++; } 
        return spinetip;
}
term * term::spineTip(int & numarg) {
        if (tag != APP) { numarg = 0; return this; }
        numarg = 1; term * p = fields[0];
        while (p->isApp()) { p = p->fields[0]; numarg++; }
        return p;
}
#line 224 "ex-terms.nw"
bool term::isFunc2Args() {
	if (spinetip && spinelength == 3 && spinetip->isF()) return true;
        return (isApp() && lc()->isApp() && lc()->lc()->isF());
}
bool term::isFunc2Args(int f) {
	if (spinetip && spinelength == 3 && spinetip->isF(f)) return true;
        return (isApp() && lc()->isApp() && lc()->lc()->isF(f));
}
#line 239 "ex-terms.nw"
bool term::isAString() {
	return (isApp() && lc()->isApp() && lc()->lc()->isD(iHash) 
                && lc()->rc()->isChar());
}
bool term::isChar() {
	if (isfloat || isint) return false;
	return (tag == D && cname >= 2000 && cname < 3000);
}
bool term::isString() {
	if (isfloat || isint) return false;
	return (tag == D && strings.find(cname) != strings.end());
}
#line 259 "ex-terms.nw"
bool term::isRigid() {
	if (tag == V || tag == D) return true;
	if (tag == F) return is_rigid_constant(cname);
	if (tag == ABS) return fields[1]->isRigid();
	if (tag == MODAL) return fields[0]->isRigid();
	assert(tag == PROD || tag == APP);
	for (unint i=0; i!=fieldsize; i++) 
		if (!fields[i]->isRigid()) return false;
	return true;
}
#line 303 "ex-terms.nw"
bool term::equal(term * t) {
        if (tag != t->tag) return false;
        if (cname != t->cname) return false;
        if (modality != t->modality) return false;
        
#line 329 "ex-terms.nw"
if (isint && t->isint && numi != t->numi) return false;
if (isint && t->isfloat && (double)numi != t->numf) return false;
if (isfloat && t->isint && numf != (double)t->numi) return false;
if (isfloat && t->isfloat && numf != t->numf) return false;
#line 308 "ex-terms.nw"
	// unint size1 = fieldsize;
        // unint size2 = t->fieldsize;
        if (fieldsize != t->fieldsize) return false;
        for (unint i=0; i!=fieldsize; i++)
                if (fields[i]->equal(t->fields[i]) == false)
                        return false;
        return true;
}
#line 361 "ex-terms.nw"
extern const string pve;
void term::print() {
        if (getSelector() == SILENT) return;
	
#line 410 "ex-terms.nw"
if (isAString()) {
	string temp = ""; temp += getString(lc()->rc()->cname)[1];
	term * arg2 = rc();
	while (!arg2->isD(iEmptyList)) {
		assert(arg2->lc()->rc()->isChar());
		temp += getString(arg2->lc()->rc()->cname)[1];
		arg2 = arg2->rc();
	}
	ioprint("\""); ioprint(temp); ioprint("\""); return;
}
#line 365 "ex-terms.nw"
	
#line 427 "ex-terms.nw"
if (isApp() && lc()->isApp() && lc()->lc()->isD(iHash)) {
	ioprint("["); lc()->rc()->print();
	term * arg2 = rc();
	while (arg2->isD(iEmptyList) == false) {
		ioprint(", ");
		if (arg2->isApp() && arg2->lc()->isApp() && 
		    arg2->lc()->lc()->isD(iHash)) 
			{ arg2->lc()->rc()->print(); arg2 = arg2->rc(); }
		else { arg2->print(); break; }
	}
	ioprint("]");
	return;
}
#line 366 "ex-terms.nw"
        if (redex) ioprint(" [[[ ");
        
#line 447 "ex-terms.nw"
if (isApp() && lc()->cname == iIte) { 
	ioprint("if "); rc()->fields[0]->print();
	ioprint(" then "); rc()->fields[1]->print();
	/* ioprint("\n\t");*/ ioprint(" else "); rc()->fields[2]->print();
	return;
}
#line 368 "ex-terms.nw"
        // if (refcount > 1) ioprint("_s_");
	if (cname >= 5000) { ioprint(pve); ioprint(cname - 5000); }
	else if (cname > 0) ioprint(getString(cname));
        else if (isfloat) ioprint(numf);
	else if (isint) ioprint(numi);
	else if (isFunc2Args()) {
		ioprint("("); lc()->lc()->print(); ioprint(" ");
		lc()->rc()->print(); ioprint(" "); rc()->print(); ioprint(")");
	} else if (tag == APP && (lc()->isF(iSigma) || lc()->isF(iPi))) {
		if (lc()->isF(iSigma)) ioprint("\\exists ");
		else ioprint("\\forall ");
		rc()->fields[0]->print(); ioprint(".");
		rc()->fields[1]->print(); 
        } else if (tag == APP) {
		ioprint("("); fields[0]->print(); ioprint(" ");
                fields[1]->print(); ioprint(")"); 
        } else if (tag == ABS) {
                ioprint("\\"); fields[0]->print(); 
		ioprint("."); fields[1]->print();
        } else if (tag == PROD) {
                int size = fieldsize;
                if (size == 0) { ioprint("()"); return; }
                ioprint("(");
                for (int i=0; i!=size-1; i++) 
                      { fields[i]->print(); ioprint(","); }
                fields[size-1]->print(); ioprint(")");
        } else if (tag == MODAL) {
                ioprint("["); ioprint(modality); ioprint("] ");
		fields[0]->print(); 
        } else { 
#line 456 "ex-terms.nw"
cerr << "Printing untagged term.\ttag = " << tag << endl;
assert(false);
#line 397 "ex-terms.nw"
                                          }
        if (redex) ioprint(" ]]] ");
}
#line 469 "ex-terms.nw"
void term::printVertical(unint level) {
        if (getSelector() == SILENT) return;
        
#line 502 "ex-terms.nw"
for (unint i=0; i!=level; i++) ioprint(" ");

#line 472 "ex-terms.nw"
	if (cname >= 5000) { ioprint(pve); ioprint(cname-5000); }
        else if (cname > 0) 
                { ioprint(getString(cname)); 
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 474 "ex-terms.nw"
                                                                         }
        else if (isfloat) { ioprint(numf);  
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 475 "ex-terms.nw"
                                                                        }
	else if (isint) { ioprint(numi);  
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 476 "ex-terms.nw"
                                                               }
        else if (tag == APP) {
                ioprint("("); 
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 479 "ex-terms.nw"
                fields[0]->printVertical(level+1);
                fields[1]->printVertical(level+1);
                
#line 502 "ex-terms.nw"
for (unint i=0; i!=level; i++) ioprint(" ");

#line 481 "ex-terms.nw"
                                       ioprint(")\n");
        } else if (tag == ABS) {
                ioprint("\\"); fields[0]->print(); ioprint("."); 
                
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 485 "ex-terms.nw"
                fields[1]->printVertical(level+1);
        } else if (tag == PROD) {
                int size = fieldsize;
                if (size == 0) 
                      { ioprint("()"); 
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 489 "ex-terms.nw"
                                                                   return; }
                ioprint("("); 
#line 505 "ex-terms.nw"
ioprint("\t\t");
if (refcount > 1) { ioprint("shared"); ioprint(refcount); }
ioprintln();
#line 491 "ex-terms.nw"
                for (int i=0; i!=size-1; i++) {
                        fields[i]->printVertical(level+1); ioprint(",\n"); }
                fields[size-1]->printVertical(level+1);
                
#line 502 "ex-terms.nw"
for (unint i=0; i!=level; i++) ioprint(" ");

#line 494 "ex-terms.nw"
                                       ioprint(")\n");
        } else if (tag == MODAL) {
                assert(false);
        } else { 
#line 456 "ex-terms.nw"
cerr << "Printing untagged term.\ttag = " << tag << endl;
assert(false);
#line 497 "ex-terms.nw"
                                          }
}
#line 668 "ex-terms.nw"
term * term::clone() {
        term * ret;
        if (isfloat) ret = new_term_float(numf);
        else if (isint) ret = new_term_int(numi);
        else if (tag >= SV && tag <= D) ret = new_term(tag, cname);
        else ret = new_term(tag);
        
#line 53 "ex-terms.nw"
ret->cname = cname;
if (tag == MODAL) ret->modality = modality;
// if (ptype) ret->ptype = ptype->clone();

#line 548 "ex-terms.nw"
if (cond) { assert(tag == SV); 
            ret->cond = new condition;
            ret->cond->svname = cond->svname; ret->cond->tag = cond->tag; }

#line 998 "ex-terms.nw"
ret->validfree = false;
#line 1388 "ex-terms.nw"
ret->noredex = noredex;

#line 3108 "ex-terms.nw"
ret->is_data = is_data;
#line 3186 "ex-terms.nw"
ret->notEnoughArgs = notEnoughArgs;
#line 675 "ex-terms.nw"
                  
        int size = fieldsize;
        for (int i=0; i!=size; i++) ret->insert(fields[i]->clone());
        return ret;
}
#line 689 "ex-terms.nw"
/*
void term::freememory() {
        refcount--;
        
#line 722 "ex-terms.nw"
// if (refcount < 0) { setSelector(STDERR); print(); ioprintln();
//	            ioprint("refcount = "); ioprintln(refcount); }
assert(refcount >= 0);
#line 693 "ex-terms.nw"
        if (refcount != 0) return;
	if (ptype) { delete_type(ptype); }
        if (cond) delete cond; 
        int size = fieldsize;
        for (int i=0; i!=size; i++) if (fields[i]) fields[i]->freememory();
	fieldsize = 0;
        delete_term(this);
}
*/
void term::freememory() {
        refcount--;
        
#line 722 "ex-terms.nw"
// if (refcount < 0) { setSelector(STDERR); print(); ioprintln();
//	            ioprint("refcount = "); ioprintln(refcount); }
assert(refcount >= 0);
#line 705 "ex-terms.nw"
        if (refcount != 0) return;
	
	term * p = this;
	delete_term(this);

	if (p->ptype) delete_type(p->ptype);
        if (p->cond) delete p->cond; 

        int size = p->fieldsize;
        for (int i=0; i!=size; i++) 
	    if (p->fields[i]) p->fields[i]->freememory();
	p->fieldsize = 0;
}

#line 742 "ex-terms.nw"
void term::replace(term * t) {
        tag = t->tag;
        
#line 58 "ex-terms.nw"
cname = t->cname;
if (t->tag == MODAL) modality = t->modality; 
// if (t->ptype) ptype = t->ptype->clone();
#line 140 "ex-terms.nw"
if (t->tag == D) { isfloat = t->isfloat; isint = t->isint; 
                   numi = t->numi; numf = t->numf;  }
#line 160 "ex-terms.nw"
freeze = t->freeze;
#line 195 "ex-terms.nw"
spinetip = NULL; spinelength = -1; spine_time = -5;
#line 553 "ex-terms.nw"
if (cond) delete cond;
cond = t->cond;
#line 913 "ex-terms.nw"
frvarsize = 0;
freevars_computed = false;
time_computed = -5;

#line 1014 "ex-terms.nw"
validfree = false;

#line 1391 "ex-terms.nw"
noredex = t->noredex;
#line 3117 "ex-terms.nw"
is_data = false;
#line 3194 "ex-terms.nw"
notEnoughArgs = false;
#line 745 "ex-terms.nw"
        int tsize = t->fieldsize;
        for (int i=0; i!=tsize; i++) t->fields[i]->reuse();
        
        int size = fieldsize;
        for (int i=0; i!=size; i++) if (fields[i]) fields[i]->freememory();
        // fields.resize(tsize);
	// copy(t->fields.begin(), t->fields.end(), fields.begin());
	fieldsize = t->fieldsize;
	for (int i=0; i!=tsize; i++)
	    fields[i] = t->fields[i];
}
#line 836 "ex-terms.nw"
void term::unshare(term * parent, unint id) {
        if (refcount > 1) {
                assert(parent); term * temp = clone();
                parent->fields[id]->freememory(); parent->fields[id] = temp;
                return;  }
        int size = fieldsize;
        for (int i=0; i!=size; i++) fields[i]->unshare(this, i);
}
#line 918 "ex-terms.nw"
void term::getFreeVars() {
        if (freevars_computed && time_computed == ltime) return;

	frvarsize = 0;
	freevars_computed = true; time_computed = ltime;

	if (tag == D || tag == F) return;
	if (tag == V) { frvars[0] = cname; frvarsize = 1; return; }
	if (tag == ABS) {
		fields[1]->getFreeVars();

		for (int i=0; i!=fields[1]->frvarsize; i++) {
		    if (fields[1]->frvars[i] == fields[0]->cname) continue;
		    frvars[frvarsize] = fields[1]->frvars[i];
		    frvarsize++;
		}
		assert(frvarsize <= 20);
		return;
	}
        for (unint i=0; i!=fieldsize; i++) {
                fields[i]->getFreeVars();
		for (int j=0; j!=fields[i]->frvarsize; j++) {
		    if (j > 0 && fields[i]->frvars[j] == fields[i]->frvars[j-1])
			continue;
		    frvars[frvarsize] = fields[i]->frvars[j];
		    frvarsize++; 
		}
		assert(frvarsize <= 20);
	}
	return;
}
#line 1027 "ex-terms.nw"
void term::unmarkValidfree() {
	validfree = false;
	for (unint i=0; i!=fieldsize; i++) fields[i]->unmarkValidfree();
}
void term::labelStaticBoundVars() {
        if (tag == F || tag == D || tag == SV) return;
        if (tag == V) { if (!validfree) { validfree = true; free = true; }
                        return; }
        if (tag == ABS) {
                fields[0]->validfree = true; fields[0]->free = false;
                fields[1]->labelBound(fields[0]->cname);
                fields[1]->labelStaticBoundVars();
                return;
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++) fields[i]->labelStaticBoundVars();
}
#line 1047 "ex-terms.nw"
void term::labelBound(int x) {
        if (tag == F || tag == D || tag == SV) return;
        if (tag == V) { if (cname == x) { validfree = true; free = false; }
                        return; }
        if (tag == ABS) { fields[1]->labelBound(x); return; }
        int size = fieldsize;
        for (int i=0; i!=size; i++) fields[i]->labelBound(x);
}
#line 1078 "ex-terms.nw"
bool term::occursFree(int var) {
        getFreeVars();
        for (int i=0; i!=frvarsize; i++) if (frvars[i] == var) return true;
        return false;
}
#line 1086 "ex-terms.nw"
bool term::occursFreeNaive(int var) {
        vector<int> boundv; return occursFreeNaive(var, boundv);
}
#line 1092 "ex-terms.nw"
bool term::occursFreeNaive(int var, vector<int> boundv) {
        if (tag == F || tag == D) return false;
        if (tag == V) { 
		if (freeze) return false;
                if (cname == var && inVector(cname,boundv)== false) return true;
                return false; }
        if (tag == ABS) {
                boundv.push_back(fields[0]->cname);
                return fields[1]->occursFreeNaive(var, boundv);
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++) 
                if (fields[i]->occursFreeNaive(var, boundv)) return true;
        return false;
}
#line 1128 "ex-terms.nw"
bool term::captured(vector<term *> & bvars, int & captd) {
        if (bvars.empty()) return false;
        getFreeVars();

        int bsize = bvars.size();
        for (int i=0; i!=frvarsize; i++) 
                for (int j=0; j!=bsize; j++) 
                        if (frvars[i] == bvars[j]->fields[0]->cname) {
                               captd = j; return true; }
        return false;
}
#line 1153 "ex-terms.nw"
void term::collectLambdaVars(multiset<int> & ret) {
	if (tag >= SV && tag <= D) return;
	if (tag == ABS) {
		ret.insert(fields[0]->cname);
		fields[1]->collectLambdaVars(ret); return;
        }
	for (unint i=0; i!=fieldsize; i++)
		fields[i]->collectLambdaVars(ret);
}
#line 1180 "ex-terms.nw"
void term::rename(int var1, int var2) {
        if (tag == SV || tag == F || tag == D) return;
        if (tag == V) { if (cname == var1) cname = var2; return; }
        int size = fieldsize;
        for (int i=0; i!=size; i++) fields[i]->rename(var1, var2);
}
#line 1195 "ex-terms.nw"
void term::renameLambdaVar(int var1, int var2) {
        freevars_computed = false;
        if (tag == SV || tag == V || tag == F || tag == D) return;
        if (tag == ABS) {
                if (fields[0]->cname == var1) {
			fields[0]->cname = var2;
                        fields[1]->rename(var1, var2);
                }
                // if lambda variables are distinct, this is not needed
                fields[1]->renameLambdaVar(var1, var2);
                return;
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++)
                fields[i]->renameLambdaVar(var1, var2);
}
#line 1305 "ex-terms.nw"
void term::subst(vector<substitution> & subs) {
        if (tag == V || tag == SV) {
		if (freeze) return;
                int size = subs.size();
                for (int i=0; i!=size; i++)
                        if (cname == subs[i].first) {
                                this->replace(subs[i].second); return; }
                return;
        }
        labelStaticBoundVars();
        vector<term *> bindingAbss;
        subst2(subs, bindingAbss, NULL);
	unmarkValidfree();
}
#line 1332 "ex-terms.nw"
void term::subst2(vector<substitution> & subs, vector<term *> bindingAbss,
		  term ** pointer) {
        if (tag == SV) { if (freeze) return; 
#line 1357 "ex-terms.nw"
int size = subs.size();
for (int i=0; i!=size; i++) 
        if (cname == subs[i].first) { 
#line 1371 "ex-terms.nw"
assert(pointer);
this->freememory();
if (!subs[i].second->noredex && subs[i].second->shared()) 
	*pointer = subs[i].second->clone();
else *pointer = subs[i].second->reuse();
#line 1359 "ex-terms.nw"
                                                                return; }
return;
#line 1334 "ex-terms.nw"
                                                                    }
        if (tag == V) { if (freeze) return; 
#line 1423 "ex-terms.nw"
if (isFree() == false) return;
int size = subs.size();
for (int i=0; i!=size; i++) {
        if (cname != subs[i].first) continue;
        
#line 1435 "ex-terms.nw"
int k;
while (subs[i].second->captured(bindingAbss,k))
    bindingAbss[k]->renameLambdaVar(bindingAbss[k]->fields[0]->cname,newPVar());
#line 1428 "ex-terms.nw"
        
#line 1371 "ex-terms.nw"
assert(pointer);
this->freememory();
if (!subs[i].second->noredex && subs[i].second->shared()) 
	*pointer = subs[i].second->clone();
else *pointer = subs[i].second->reuse();
#line 1429 "ex-terms.nw"
        return;
}
return;
#line 1335 "ex-terms.nw"
                                                                  }
        if (tag == F || tag == D) return;
        if (tag == ABS) {
		if (fields[0]->tag == SV)
			fields[0]->subst2(subs, bindingAbss, &fields[0]);
                bindingAbss.push_back(this);
                fields[1]->subst2(subs, bindingAbss, &fields[1]);
                return;
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++) 
                fields[i]->subst2(subs, bindingAbss, &fields[i]);
}
#line 1475 "ex-terms.nw"
void term::subst(substitution & sub) {
        if (tag == V || tag == SV) {
                if (cname == sub.first) this->replace(sub.second); return; }
        labelStaticBoundVars();
        vector<term *> bindingAbs;
        vector<substitution> subs; subs.push_back(sub);
        subst2(subs, bindingAbs, NULL);
	unmarkValidfree();
}
#line 1526 "ex-terms.nw"
bool isUVar(term * t) { return (t->tag == V && t->cname >= 100000); }
bool isUVar(int cn) { return (cn >= 100000); }

bool term::containsFreeVariable() {
	if (tag == V) return isUVar(cname);
	for (unint i=0; i!=fieldsize; i++)
		if (fields[i]->containsFreeVariable()) return true;
	return false;
}

void term::collectFreeVariables(set<int> & fvars) {
	if (tag == V && isUVar(cname)) fvars.insert(cname);
	for (unint i=0; i!=fieldsize; i++)
		fields[i]->collectFreeVariables(fvars);
	return;
}
#line 1550 "ex-terms.nw"
bool term::isNegation() { return (isApp() && lc()->isF(iNot)); }
#line 1559 "ex-terms.nw"
bool term::isDiamond() {
	return (isNegation() && rc()->isModal() &&
		rc()->fields[0]->isNegation());
}
#line 1572 "ex-terms.nw"
bool term::isNegationOf(term * t2) {
	term * t1 = this;
	if ((t1->isD(iTrue) && t2->isD(iFalse)) ||
	    (t1->isD(iFalse) && t2->isD(iTrue)))
		return true;
	if (t2->isApp() && t2->lc()->isF(iNot) && t1->equal(t2->rc()))
		return true;
	if (t1->isApp() && t1->lc()->isF(iNot) && t2->equal(t1->rc()))
		return true;
	return false;
}
#line 1590 "ex-terms.nw"
void term::stripNegations() {
	if (isNegation() && rc()->isNegation()) {
		term * term = rc()->rc();
		rc()->fields[1] = NULL;
		this->replace(term);
	}
}
#line 1607 "ex-terms.nw"
bool term::termReplace(term * s, term * r, term * parent, int id) {
	if (equal(s)) {
		if (parent == NULL) this->replace(r->clone());
		else { parent->fields[id]->freememory();
		       parent->fields[id] = r->clone(); }
		return true;
	}
	bool ret = false;
	int size = fieldsize;
	for (int i=0; i!=size; i++)
		ret = (ret || fields[i]->termReplace(s, r, this, i));
	return ret;
}
#line 1634 "ex-terms.nw"
extern bool redex_match(term * head, term * body, vector<substitution> & theta);

bool term::matchReplace(term * s, term * r, term * parent, int id) {
	vector<substitution> theta;
	if (redex_match(s, this, theta)) {
		term * r2 = r->clone();
		r2->subst(theta);
		if (parent == NULL) this->replace(r2);
		else {  parent->fields[id]->freememory();
			parent->fields[id] = r2; }
		return true;
	}
	bool ret = false;
	int size = fieldsize;
	for (int i=0; i!=size; i++)
		ret = (ret || fields[i]->matchReplace(s, r, this, i));
	return ret;
}
#line 1664 "ex-terms.nw"
term * term::normalise() 
      { return (this->normalise1())->normalise2(); }
#line 1677 "ex-terms.nw"
term * term::normalise1() {
	for (unint i=0; i!=fieldsize; i++) 
		fields[i] = fields[i]->normalise();
	term * ret;
	if (isD(iTrue)) {
	   	// ret = new_term(APP); ret->lc = new_term(F, iNot);
		// ret->rc = new_term(D, iFalse);

		ret = new_term(APP); ret->insert(new_term(F, iNot));
		ret->insert(new_term(D, iFalse));
		this->replace(ret); return this;
	} 
	if (isFunc2Args(iImplies)) {
		lc()->lc()->cname = iOr; 
		ret = new_term(APP); 
		// ret->lc = new_term(F, iNot);
		// ret->rc = lc->rc;
		ret->insert(new_term(F, iNot));
		ret->insert(lc()->rc());
		lc()->fields[1] = ret;
		return this;
	}
	if (isFunc2Args(iAnd)) { 
#line 1721 "ex-terms.nw"
ret = new_term(APP); 
// ret->lc = new_term(F, iNot);
ret->insert(new_term(F, iNot));
term * arg2 = newT2Args(F, iOr);
term * arg21 = new_term(APP); 
// arg21->lc = new_term(F, iNot); arg21->rc = lc->rc->clone();
arg21->insert(new_term(F, iNot)); arg21->insert(lc()->rc()->clone());
term * arg22 = new_term(APP);
// arg22->lc = new_term(F, iNot); arg22->rc = rc->clone();
arg22->insert(new_term(F, iNot)); arg22->insert(rc()->clone());
arg2->initT2Args(arg21, arg22);
// ret->rc = arg2;
ret->insert(arg2);
this->replace(ret); return this;
#line 1699 "ex-terms.nw"
                                              }
	if (isFunc2Args(iIff)) { 
#line 1744 "ex-terms.nw"
ret = newT2Args(F, iAnd);
term * arg1 = newT2Args(F, iOr);
term * arg1a = new_term(APP);
// arg1a->lc = new_term(F, iNot); arg1a->rc = lc->rc->clone();
arg1a->insert(new_term(F, iNot)); arg1a->insert(lc()->rc()->clone());
arg1->initT2Args(arg1a, rc()->clone());
term * arg2 = newT2Args(F, iOr);
term * arg2a = new_term(APP);
// arg2a->lc = new_term(F, iNot); arg2a->rc = rc->clone();
arg2a->insert(new_term(F, iNot)); arg2a->insert(rc()->clone());
arg2->initT2Args(arg2a, lc()->rc()->clone());
ret->initT2Args(arg1, arg2);
ret = ret->normalise1();
this->replace(ret); return this;
#line 1700 "ex-terms.nw"
                                              }
	if (isApp() && lc()->isF(iPi)) {
		lc()->cname = iSigma;
		ret = new_term(APP); 
		ret->insert(new_term(F, iNot));
		ret->insert(rc()->fields[1]);
		rc()->fields[1] = ret;
		ret = new_term(APP); ret->insert(new_term(F, iNot));
		ret->insert(this);
		return ret;
	} 
	return this;
}
#line 1761 "ex-terms.nw"
term * term::normalise2() {
	if (isApp() && rc()->isApp() && lc()->isF(iNot) &&
	    rc()->lc()->isF(iNot)) {
		term * ret = rc()->rc();
		rc()->fields[1] = NULL;
		freememory();
		ret = ret->normalise2();
		return ret;
	}
	for (unint i=0; i!=fieldsize; i++)
	 	fields[i] = fields[i]->normalise2();
	return this;
}
#line 1782 "ex-terms.nw"
void term::collectFunctionNames(set<int> & x) {
	if (tag == F) { x.insert(cname); return; }
	for (unint i=0; i!=fieldsize; i++)
		fields[i]->collectFunctionNames(x);
}
#line 1821 "ex-terms.nw"
bool term::simplifyEquality(term * parent, unint id) {
        bool changed = false;
        term * ret = this;
        term * t1 = lc()->rc(), * t2 = rc();
        
#line 1968 "ex-terms.nw"
int t1_arity = 0, t2_arity = 0;

#line 1827 "ex-terms.nw"
        
#line 1861 "ex-terms.nw"
if ((t1->isVar() && t2->isVar()) || (t1->isF() && t2->isF()))
	if (t1->cname == t2->cname) {
		changed = true; ret = new_term(D, iTrue); 
		goto simplifyEquality_cleanup;
	}
#line 1827 "ex-terms.nw"
                                                                        
        
#line 1875 "ex-terms.nw"
if (t1->isVar() || t2->isVar()) return false;
if (t1->isAbs()) return false;
#line 1829 "ex-terms.nw"
	
#line 1890 "ex-terms.nw"
if (t1->isAString() && t2->isAString()) {
	changed = true;
	term * p1 = t1, * p2 = t2;
	while (true) {
		if (p1->isD(iEmptyList) && p2->isD(iEmptyList)) break;
		if (p1->tag != p2->tag ||
		    p1->lc()->rc()->cname!=p2->lc()->rc()->cname)
		      { ret=new_term(D,iFalse); goto simplifyEquality_cleanup;}
		p1 = p1->rc();
		p2 = p2->rc();
	}
	ret = new_term(D, iTrue);
	goto simplifyEquality_cleanup;
} 
#line 1830 "ex-terms.nw"
        
#line 1919 "ex-terms.nw"
if (t1->isProd() && t2->isProd()) {
        changed = true; unint t1_args = t1->fieldsize;

        
#line 1950 "ex-terms.nw"
if (t1_args == 0) { ret = new_term(D, iTrue); goto simplifyEquality_cleanup; }
#line 1923 "ex-terms.nw"
        
#line 1960 "ex-terms.nw"
if (t1_args != t2->fieldsize || t1_args < 2) {
        setSelector(STDERR); ioprint("Error in simplifyEquality:products\n");
        t1->print(); ioprintln(); t2->print(); ioprintln(); 
}
assert(t1_args == t2->fieldsize && t1_args >= 2);

#line 1925 "ex-terms.nw"
        term * eq1 = newT2Args(F, iEqual);
        eq1->initT2Args(t1->fields[0]->reuse(), t2->fields[0]->reuse());
        term * eq2 = newT2Args(F, iEqual);
        eq2->initT2Args(t1->fields[1]->reuse(), t2->fields[1]->reuse());

        ret = newT2Args(F, iAnd); ret->initT2Args(eq1, eq2);
        for (unint i=0; i!=t1_args-2; i++) {
                term * eqi = newT2Args(F, iEqual);
                eqi->initT2Args(t1->fields[i+2]->reuse(), 
				t2->fields[i+2]->reuse());
                term * temp = newT2Args(F, iAnd);
		temp->initT2Args(ret, eqi);
                ret = temp;
        }
        goto simplifyEquality_cleanup;
}
#line 1831 "ex-terms.nw"
        
#line 2016 "ex-terms.nw"
if (!t1->spineTip(t1_arity)->isD()) return false;
if (!t2->spineTip(t2_arity)->isD()) return false;
#line 1973 "ex-terms.nw"
changed = true;

if (t1_arity == 0 && t2_arity == 0) {
        if (t1->spineTip()->isfloat && t2->spineTip()->isfloat) {
                if (t1->spineTip()->numf == t2->spineTip()->numf) 
                        ret = new_term(D, iTrue);
                else ret = new_term(D, iFalse);
                goto simplifyEquality_cleanup; 
        } else if (t1->spineTip()->isint && t2->spineTip()->isint) {
                if (t1->spineTip()->numi == t2->spineTip()->numi) 
                        ret = new_term(D, iTrue);
                else ret = new_term(D, iFalse);
                goto simplifyEquality_cleanup; 
        }
        if (t1->spineTip()->cname == t2->spineTip()->cname) 
		ret= new_term(D,iTrue); 
        else ret = new_term(D, iFalse);
        goto simplifyEquality_cleanup; 
}
if (t1_arity != t2_arity || t1->spineTip()->cname != t2->spineTip()->cname) 
	{ ret = new_term(D, iFalse); goto simplifyEquality_cleanup; }

ret = newT2Args(F, iEqual);
ret->initT2Args(t1->fields[1]->reuse(), t2->fields[1]->reuse());
t1_arity--;
while (t1_arity != 0) {
        t1 = t1->fields[0]; t2 = t2->fields[0];
        term * temp = newT2Args(F, iEqual);
        temp->initT2Args(t1->fields[1]->reuse(), t2->fields[1]->reuse());
        term * temp2 = newT2Args(F, iAnd); temp2->initT2Args(temp, ret);
        ret = temp2;
        t1_arity--;
}

#line 1833 "ex-terms.nw"
simplifyEquality_cleanup:
        if (changed) { 
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 1834 "ex-terms.nw"
                                                    }
        return changed;
}
#line 2027 "ex-terms.nw"
bool term::simplifyArithmetic(term * parent, unint id) {
        if (!(rc()->isD() && lc()->rc()->isD())) return false;
	
        int op = fields[0]->lc()->cname; 
	if (!(op >= iAdd && op <= iAtan2)) return false;

        term * t1 = lc()->rc(), * t2 = rc();
	if (t1->isD(iInfinity) || t2->isD(iInfinity)) return false;

        term * ret = NULL;
        
#line 2078 "ex-terms.nw"
if (op == iAdd) { 
        if (t1->isfloat && t2->isfloat)
                ret = new_term_float(t1->numf + t2->numf);
        else if (t1->isfloat && t2->isint)
                ret = new_term_float(t1->numf + t2->numi);
        else if (t1->isint && t2->isfloat)
                ret = new_term_float(t1->numi + t2->numf);
        else if (t1->isint && t2->isint)
                ret = new_term_int(t1->numi + t2->numi);
        else return false;
} else if (op == iSub) { 
        if (t1->isfloat && t2->isfloat)
                ret = new_term_float(t1->numf - t2->numf);
        else if (t1->isfloat && t2->isint)
                ret = new_term_float(t1->numf - t2->numi);
        else if (t1->isint && t2->isfloat)
                ret = new_term_float(t1->numi - t2->numf);
        else if (t1->isint && t2->isint)
                ret = new_term_int(t1->numi - t2->numi);
        else return false;
} else if (op == iMul) { 
        if (t1->isfloat && t2->isfloat)
                ret = new_term_float(t1->numf * t2->numf);
        else if (t1->isfloat && t2->isint)
                ret = new_term_float(t1->numf * t2->numi);
        else if (t1->isint && t2->isfloat)
                ret = new_term_float(t1->numi * t2->numf);
        else if (t1->isint && t2->isint)
                ret = new_term_int(t1->numi * t2->numi);
        else return false;
} else if (op == iDiv) { 
        if (t1->isfloat && t2->isfloat)
                ret = new_term_float(t1->numf / t2->numf);
        else if (t1->isfloat && t2->isint)
                ret = new_term_float(t1->numf / t2->numi);
        else if (t1->isint && t2->isfloat)
                ret = new_term_float(t1->numi / t2->numf);
        else if (t1->isint && t2->isint) {
		double res = (double)t1->numi / (double)t2->numi;
		if (res == floor(res)) ret = new_term_int(t1->numi / t2->numi);
                else ret = new_term_float(res);
        } else return false;
}
#line 2038 "ex-terms.nw"
        else if (op == iMax) {
                if (t1->isfloat && t2->isfloat) {
                        if (t1->numf >= t2->numf) ret =new_term_float(t1->numf);
                        else ret = new_term_float(t2->numf) ;
                } else if (t1->isint && t2->isint) {
                        if (t1->numi >= t2->numi) ret = new_term_int(t1->numi);
                        else ret = new_term_int(t2->numi) ;
                } else return false;
        } else if (op == iMin) {
                if (t1->isfloat && t2->isfloat) {
                        if (t1->numf <= t2->numf) ret =new_term_float(t1->numf);
                        else ret = new_term_float(t2->numf) ;
                } else if (t1->isint && t2->isint) {
                        if (t1->numi <= t2->numi) ret = new_term_int(t1->numi);
                        else ret = new_term_int(t2->numi) ;
                } else return false;
        } else if (op == iMod) {
                assert(t1->isint && t2-isint);
                ret = new_term_int(int(t1->numi % t2->numi)); 
        } else if (op == iAtan2) {
		assert(t1->isfloat && t2->isfloat);
		ret = new_term_float(atan2(t1->numf, t2->numf));
	}
        
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2062 "ex-terms.nw"
        return true;
}
#line 2129 "ex-terms.nw"
bool term::simplifyInequalities(term * parent, unint id) {
        if (!(rc()->isD() && lc()->rc()->isD())) return false;
        int rel = lc()->lc()->cname; 

	if (!(rel >= iLT && rel <=iGTE)) return false;

        term * t1 = lc()->rc() ;
	term * t2 = rc() ;

	if (t1->isD(iInfinity) || t2->isD(iInfinity)) return false;

        term * ret = NULL;
        if (rel == iLT) { 
                if (t1->isint && t2->isint) {
                        if (t1->numi < t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isfloat) {
                        if (t1->numf < t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isint && t2->isfloat) {
                        if (t1->numi < t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
		} else if (t1->isfloat && t2->isint) {
                        if (t1->numf < t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else return false;
        } else if (rel == iLTE) {
                if (t1->isint && t2->isint) {
                        if (t1->numi <= t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isfloat) {
                        if (t1->numf <= t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isint && t2->isfloat) {
                        if (t1->numi <= t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isint) {
                        if (t1->numf <= t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else return false;
        } else if (rel == iGT) {
                if (t1->isint && t2->isint) {
                        if (t1->numi > t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isfloat) {
                        if (t1->numf > t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isint && t2->isfloat) {
                        if (t1->numi > t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isint) {
                        if (t1->numf > t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else return false;
        } else if (rel == iGTE) {
                if (t1->isint && t2->isint) {
                        if (t1->numi >= t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isfloat) {
                        if (t1->numf >= t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isint && t2->isfloat) {
                        if (t1->numi >= t2->numf) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else if (t1->isfloat && t2->isint) {
                        if (t1->numf >= t2->numi) ret = new_term(D,iTrue); 
                        else ret = new_term(D,iFalse);
                } else return false;
        }
        
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2199 "ex-terms.nw"
        return true;
}
#line 2208 "ex-terms.nw"
bool term::simplifyMath(term * parent, unint id) {
	if (!(lc()->isF() && rc()->isD())) return false;
	int op = lc()->cname;
	if (!(op >= iSin && op <= iExp)) return false;

	term * ret = NULL;
	if (op == iSin) {
		assert(rc()->isfloat);
		ret = new_term_float(sin(rc()->numf));
	} else if (op == iCos) {
		assert(rc()->isfloat);
		ret = new_term_float(cos(rc()->numf));
	} else if (op == iSqrt) {
		assert(rc()->isfloat);
		ret = new_term_float(sqrt(rc()->numf));
	} else if (op == iExp) {
		assert(rc()->isfloat);
		ret = new_term_float(exp(rc()->numf));
	}
	
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2228 "ex-terms.nw"
        return true;
}
#line 2262 "ex-terms.nw"
bool term::betaReduction(term * parent, unint id) {
        if (lc()->isAbs() == false) return false;

        substitution bind(lc()->fields[0]->cname, rc());
        lc()->fields[1]->subst(bind);
        term * ret = lc()->fields[1]->reuse();
	
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2269 "ex-terms.nw"
        return true;
}
#line 2285 "ex-terms.nw"
bool term::simplifyIte(term * parent, unint id) {
	if (!lc()->isF(iIte)) return false;
	term * cond = rc()->fields[0];
	if (!cond->isFunc2Args(iEqual)) return false;
	if (!cond->lc()->rc()->isVar()) return false;
	int vname = cond->lc()->rc()->cname;
	if (!rc()->fields[1]->occursFree(vname)) return false;
	substitution bind(vname, cond->rc());
	rc()->fields[1]->subst(bind);
	return true;
}
#line 2405 "ex-terms.nw"
bool term::simplifyConjunction() {
        term * p = findEq(this); if (p == NULL) return false;

        term * varp = p->lc()->rc();
	varp->freeze = true;

        substitution bind(varp->cname, p->fields[1]->reuse());
        subst(bind); p->fields[1]->refcount--;
	varp->freeze = false;

        return true;
}
#line 2432 "ex-terms.nw"
term * term::findEq(term * root) {
        term * p = NULL;
        term * t1 = lc()->rc(); 
        if (t1->isEq(root)) return t1;
        if (t1->isFunc2Args(iAnd)) { p = t1->findEq(root); if (p) return p; }

        term * t2 = rc(); 
        if (t2->isEq(root)) return t2;
        if (t2->isFunc2Args(iAnd)) { p = t2->findEq(root); if (p) return p; }
        return NULL;
}
#line 2470 "ex-terms.nw"
bool term::isEq(term * root) {
        if (isFunc2Args(iEqual) == false) return false;
        term * t1 = lc()->rc(), * t2 = rc();
        if (t1->isVar() && t2->isVar() == false) {
                if (t2->occursFree(t1->cname) == false) {
			t1->freeze = true;
			if (root->occursFreeNaive(t1->cname)) {
				t1->freeze = false; return true; }
			t1->freeze = false;
                }}
        if (t2->isVar() && t1->isVar() == false) {
                if (t1->occursFree(t2->cname) == false) {
			t2->freeze = true;
			bool ret = root->occursFreeNaive(t2->cname);
			t2->freeze = false;
                        if (ret) { 
#line 2507 "ex-terms.nw"
term * temp = t1; lc()->fields[1] = t2; fields[1] = temp;
#line 2485 "ex-terms.nw"
                                                              }
                        return ret;
                }}
        return false;
}
#line 2554 "ex-terms.nw"
bool term::simplifyConjunction2(term * parent, unint id) {
        term * t1 = lc()->rc(), * t2 = rc();
        term * sigma, * other;
        if (t2->isApp() && t2->lc()->isF(iSigma)) {
                sigma = t2; other = t1;
        } else if (t1->isApp() && t1->lc()->isF(iSigma)) {
                sigma = t1; other = t2;
        } else return false;

        int var = sigma->rc()->fields[0]->cname;
        if (other->occursFree(var)) return false;

        
#line 2577 "ex-terms.nw"
term * con = newT2Args(F, iAnd);
if (sigma == t2) 
	con->initT2Args(other->reuse(), sigma->rc()->fields[1]->reuse());
else con->initT2Args(sigma->rc()->fields[1]->reuse(), other->reuse()); 
term * abs = new_term(ABS);
// abs->lc = new_term(V, var); abs->rc = con;
abs->insert(new_term(V, var)); abs->insert(con);
term * ret = new_term(APP);
// ret->lc = new_term(F, iSigma); ret->rc = abs;
ret->insert(new_term(F, iSigma));  ret->insert(abs);
#line 2567 "ex-terms.nw"
        
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2568 "ex-terms.nw"
        return true;
}
#line 2673 "ex-terms.nw"
bool term::simplifyExistential(term * parent, unint id) {
        if (fields[0]->isF(iSigma) == false) return false;

        term * ret = NULL; term * p = NULL;
        int var = rc()->fields[0]->cname;
        substitution bind;

        
#line 2699 "ex-terms.nw"
term * body = fields[1]->fields[1]; 
while (body->isApp() && body->lc()->isF(iSigma)) {
        body = body->rc()->fields[1]; 
}

#line 2682 "ex-terms.nw"
        
#line 2717 "ex-terms.nw"
if (body->isD(iTrue) || body->isD(iFalse)) {
        ret = body->reuse(); goto simplifyExistential_cleanup; }
#line 2683 "ex-terms.nw"
        
#line 2763 "ex-terms.nw"
if (body->isEq(var)) { ret = new_term(D, iTrue);
                       goto simplifyExistential_cleanup; }
#line 2778 "ex-terms.nw"
if (body->isFunc2Args(iAnd) == false) return false;
p = body->replaceEq(var); if (p == NULL) return false;
        
bind.first = p->lc()->rc()->cname; bind.second = p->fields[1];
body->subst(bind);
p->freememory();

ret = rc()->fields[1]->reuse();

#line 2685 "ex-terms.nw"
simplifyExistential_cleanup:
        
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2687 "ex-terms.nw"
        return true;
}
#line 2803 "ex-terms.nw"
term * term::replaceEq(int var) {
        term * p = NULL;
        term * t1 = lc()->rc();
        if (t1->isEq(var)) { 
                lc()->fields[1] = new_term(D, iTrue); return t1; }
        if (t1->isFunc2Args(iAnd)) { p = t1->replaceEq(var); if (p) return p; }

        term * t2 = rc();
        if (t2->isEq(var)) { fields[1] = new_term(D, iTrue); return t2; } 
        if (t2->isFunc2Args(iAnd)) { p = t2->replaceEq(var); if (p) return p; }
        return NULL;
}
#line 2828 "ex-terms.nw"
bool term::isEq(int x) {
        if (isFunc2Args(iEqual) == false) return false;
        term * t1 = lc()->rc(), * t2 = rc();

        if (t1->isVar(x) && t2->occursFree(x) == false) return true;
        if (t2->isVar(x) && t1->occursFree(x) == false) {
                
#line 2507 "ex-terms.nw"
term * temp = t1; lc()->fields[1] = t2; fields[1] = temp;
#line 2834 "ex-terms.nw"
                                           
                return true; }
        return false;
}
#line 2874 "ex-terms.nw"
bool term::simplifyUniversal(term * parent, unint id) {
        if (lc()->isF(iPi) == false) return false;
        
        int var = rc()->fields[0]->cname;
        
#line 2892 "ex-terms.nw"
term * body = rc()->fields[1];
while (body->isApp() && body->lc()->isF(iPi)) 
        body = body->rc()->fields[1];
if (body->isFunc2Args(iImplies) == false) return false;
term * t1 = body->lc()->rc();
#line 2879 "ex-terms.nw"
        
#line 2904 "ex-terms.nw"
if (t1->isD(iFalse)) { term * ret = new_term(D, iTrue);
                        
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2906 "ex-terms.nw"
                        return true; }
#line 2880 "ex-terms.nw"
        
#line 2917 "ex-terms.nw"
if (t1->isEq(var)) {
        term * t2 = body->rc();
        substitution bind(t1->lc()->rc()->cname, t1->rc());
        t2->subst(bind);
        body->replace(t2->reuse());
        t2->freememory();
        
#line 2932 "ex-terms.nw"
term * ret = rc()->fields[1]->reuse();
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2934 "ex-terms.nw"
return true;
#line 2924 "ex-terms.nw"
}
#line 2881 "ex-terms.nw"
        
#line 2947 "ex-terms.nw"
if (t1->isFunc2Args(iAnd) == false) return false;
term * p = t1->replaceEq(var); if (p == NULL) return false;

substitution bind(p->lc()->rc()->cname, p->fields[1]);
body->subst(bind);
p->freememory();

#line 2932 "ex-terms.nw"
term * ret = rc()->fields[1]->reuse();
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2934 "ex-terms.nw"
return true;
#line 2882 "ex-terms.nw"
}
#line 2984 "ex-terms.nw"
bool term::simplifyModalTerms(term * parent, unint id) {
	if (isModal()) {
		if (!isRigid()) return false;
		term * ret = fields[0]->reuse();
		
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 2989 "ex-terms.nw"
		return true;
	}
	if (isApp() && lc()->isModal() && lc()->fields[0]->isF()) {
		if (!rc()->isRigid()) return false;
		term * ret = new_term(MODAL);
		ret->modality = lc()->modality;
		term * temp = new_term(APP);
		/* temp->lc = lc->lc->reuse();
		temp->rc = rc->reuse();
		ret->lc = temp; */
		temp->insert(lc()->fields[0]->reuse());
		temp->insert(rc()->reuse());
		ret->insert(temp);
		
#line 1854 "ex-terms.nw"
assert(ret);
if (parent == NULL) { 
	this->replace(ret); ret->freememory();
} else { parent->fields[id]->freememory(); parent->fields[id] = ret; }
#line 3003 "ex-terms.nw"
		return true;
	}
	return false;
}
#line 3126 "ex-terms.nw"
bool term::isData() {
        if (tag != APP) return false;
        if (is_data) { fields[0]->is_data = true; return true; }
        if (spineTip()->isD()) {
                is_data = true; fields[0]->is_data = true; return true; }
        return false;
}
#line 3204 "ex-terms.nw"
bool term::isFuncNotRightArgs() {
        if (tag != APP) return false;
        if (notEnoughArgs) { fields[0]->notEnoughArgs = true; return true; }
	spineTip(); // can use spineTip(int & x) here
        int numargs = spinelength-1;
        if (spinetip->isF() == false) return false;
        pair<int,int> arity = getFuncEArity(spinetip->cname);
        
#line 3228 "ex-terms.nw"
if (arity.first == -1) return true;
#line 3212 "ex-terms.nw"
	// if (arity > numargs) { 
	if (numargs < arity.first) {
                notEnoughArgs = true; fields[0]->notEnoughArgs = true; 
                return true; 
        }
        // return (arity < numargs);
	return (numargs > arity.second);
}
#line 3256 "ex-terms.nw"
bool term::reduce(vector<int> mpath, term * parent, unint cid,
			 term * root, int & tried) {
	if (reduce(mpath,parent,cid,root,tried,false)) return true;
	// cerr << "Trying last resort rules" << endl;// setSelector(osel);
	return reduce(mpath,parent,cid,root,tried,true);
}
#line 3265 "ex-terms.nw"
bool term::reduce(vector<int> mpath, term * parent, unint cid,
		      term * root, int & tried, bool lr) {
	
#line 3073 "ex-terms.nw"
if (isAString()) return false;
if (tag == V || tag == D) return false;
if (tag == ABS || tag == PROD || tag == MODAL || isData()) goto not_a_redex;
#line 3268 "ex-terms.nw"
#ifdef ESCHER
        
#line 3150 "ex-terms.nw"
if (tag == F && getFuncEArity(cname).first != 0) return false; 
if (isFuncNotRightArgs()) goto not_a_redex;
#line 3270 "ex-terms.nw"
#endif
        tried++;
	// if (try_disruptive(mpath, this, parent, cid, root, tried))
	//	return true; 
        if (try_match_n_reduce(mpath,this,parent,cid,root,tried,lr)) 
                return true;

not_a_redex:
        if (tag == ABS) 
	   	return fields[1]->reduce(mpath,this,1, root, tried,lr);
        if (tag == MODAL) {
                // return false; // to begin with, Escher does not handle this.
		mpath.push_back(modality);
                return fields[0]->reduce(mpath,this,0,root, tried, lr);
        }
        if (tag == APP) {
                
#line 3315 "ex-terms.nw"
#ifdef ESCHER
if (lc()->isF() && notEnoughArgs) 
        return rc()->reduce(mpath, this, 1, root, tried, lr);
#endif
#line 3287 "ex-terms.nw"
		if (lc()->reduce(mpath, this,0, root, tried, lr)) 
		        return true;
                return rc()->reduce(mpath,this,1, root, tried, lr);
        } 
        if (tag == PROD) {
                unint dimension = fieldsize;
                for (unint i=0; i!=dimension; i++)
                        if (fields[i]->reduce(mpath, this, i, root, tried, lr))
                                return true;
                return false;
        }
	if (tag == F) return false;
#ifdef ESCHER
        setSelector(STDERR); cerr << "term = "; print(); ioprintln();
        cerr << "tag = " << tag << endl; assert(false); 
#endif
        return false;
}
#line 3334 "ex-terms.nw"
bool term::reduceRpt(int maxstep, int & stepsTaken) {
	int tried = 0; vector<int> modalPath;
	bool reduced = true; bool rewritten = false;
	int starttime = ltime;
	while (reduced) { 
		reduced = reduce(modalPath, NULL, 0, this, tried);
		if (reduced) rewritten = true;
		if (tag == D) break;
		if ((maxstep > 0) && (ltime - starttime) > maxstep) break;
		if (interrupted) break;
	};
	stepsTaken = ltime - starttime;
	return rewritten;
}
#line 3788 "ex-terms.nw"
void term::collectSharedVars() {
        term * head = fields[0]->fields[1];
        term * body = fields[1];
        vector<term *> headlvars;
        head->shareLambdaVars(headlvars, true);
        body->shareLambdaVars(headlvars, false);
        body->shareHeadLambdaVars(headlvars);
}
#line 3810 "ex-terms.nw"
void term::shareLambdaVars(vector<term *> & lvars, bool use) {
        if (tag == ABS) {
                if (use) lvars.push_back(fields[0]);
                fields[1]->shareVar(fields[0], this, 1);
                fields[1]->shareLambdaVars(lvars, use);
                return;
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++)
                fields[i]->shareLambdaVars(lvars, use);
}
#line 3834 "ex-terms.nw"
void term::shareVar(term * var, term * parent, unint id) {
        if (tag == SV || tag == D || tag == F) return;
        if (tag == ABS) { if (var->cname == fields[0]->cname) return;
                          fields[1]->shareVar(var, this, 1); return; }
        if (tag == V) {
                if (cname == var->cname) {
                        parent->fields[id] = var->reuse();
                        var->parents.push_back(&parent->fields[id]);
                        this->freememory(); }
                return;
        }
        unint size = fieldsize;
        for (unint i=0; i!=size; i++) fields[i]->shareVar(var, this, i);
}
#line 3873 "ex-terms.nw"
void term::shareHeadLambdaVars(vector<term *> & hlvars) {
        if (hlvars.empty()) return;
        if (tag == ABS) {
                int size = hlvars.size();
                for (int i=0; i!=size; i++) {
                        if (fields[0]->cname != hlvars[i]->cname) continue;
                        int psize = fields[0]->parents.size();
                        for (int j=0; j!=psize; j++) {
                                *(fields[0]->parents[j]) = hlvars[i]->reuse();
                                fields[0]->freememory();
                        }
                        fields[0]->freememory();
                        fields[0] = hlvars[i]->reuse();
                        break;
                }
                fields[1]->shareHeadLambdaVars(hlvars);
                return;
        }
        int size = fieldsize;
        for (int i=0; i!=size; i++)
                fields[i]->shareHeadLambdaVars(hlvars);
}
#line 3914 "ex-terms.nw"
void term::collectFreeVars(term * bodyparent, unint id) {
        if (tag == V && isFree()) 
                bodyparent->fields[id]->shareFreeVar(this, bodyparent, id);
        if (tag == SV || tag == D || tag == F) return;
        if (tag == ABS) fields[1]->collectFreeVars(bodyparent, id);
        int size = fieldsize;
        for (int i=0; i!=size; i++)
                fields[i]->collectFreeVars(bodyparent, id);
}
#line 3931 "ex-terms.nw"
bool term::shareFreeVar(term * v, term * parent, unint id){
        if (tag == V && isFree() && cname == v->cname) {
                freememory(); parent->fields[id] = v->reuse(); return true; }

        if (tag == SV || tag == D || tag == F) return false;
        if (tag == ABS) return fields[1]->shareFreeVar(v, this, 1);
        bool ret = false;
        int size = fieldsize;
        for (int i=0; i!=size; i++)
                if (fields[i]->shareFreeVar(v, this, i)) ret = true;
        return ret;
}
#line 3956 "ex-terms.nw"
bool term::precomputeFreeVars() {
        if (tag == SV || tag == D || tag == F) return false;
        if (tag == V && isFree()) {
	    preFVars.push_back(this);
		return true;
	}
        if (tag == ABS) {
               bool res = fields[1]->precomputeFreeVars(); 
	       if (res) 
		   preFVars = fields[1]->preFVars; 
	       return res;
	}
        int size = fieldsize;
        for (int i=0; i!=size; i++) {
                bool res = fields[i]->precomputeFreeVars();
		if (!res) continue;
		int size2 = fields[i]->preFVars.size();
		for (int j=0; j!=size2; j++)
		    preFVars.push_back(fields[i]->preFVars[j]);
        }
	return !preFVars.empty();
}
