#line 744 "types.nw"
#ifndef _UNIFICATION_H_
#define _UNIFICATION_H_

#include "terms.h"
#include "types.h"
#include <vector>
#include <utility>
struct term_type { term * first; type * second; };
extern bool unify(vector<pair<string,type *> > &eqns,type *tvn,type *t);
extern type * apply_subst(vector<pair<string, type *> > & eqns, type * x);
extern type * wellTyped(term * t);
extern pair<type *, vector<term_type> > mywellTyped(term * t);
extern type * get_type_from_syn(type * in);

#endif
