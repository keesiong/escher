#line 4790 "ex-terms.nw"
#ifndef _PATTERN_MATCH_H
#define _PATTERN_MATCH_H

#include "terms.h"

#line 4139 "ex-terms.nw"
bool redex_match(term * head, term * body, vector<substitution> & theta);
bool redex_match(term * head, term * body, vector<substitution> & theta, 
		 vector<term *> bindingAbss, term * orig_head);
#line 4527 "ex-terms.nw"
bool freevar_match(term * fml1, term * fml2, vector<substitution> & theta);
bool freevar_match(term * fml1, term * fml2, vector<substitution> & theta, 
		   vector<term *> bindingAbss);
#line 4627 "ex-terms.nw"
void printTheta(vector<substitution> & theta);
#line 4655 "ex-terms.nw"
term * findBinding(int svname, vector<substitution> & theta);

#line 4797 "ex-terms.nw"
#endif
