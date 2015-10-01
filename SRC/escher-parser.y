%{
#include <iostream>
#include <vector>
#include <stack>
#include "global.h"
#include "io.h"
#include "terms.h"
#include "unification.h"
#include "pattern-match.h"
using namespace std;

#define YYMAXDEPTH 50000

extern int yylex(); extern FILE * yyin; 
extern bool quiet; extern bool interactive;
int mywrap();
int yyparse();
extern int switchBuffer(FILE * in);
void yyerror(const char * s);
void cleanup();
#include <set>
set<string> imported;
vector<term *> temp_fields;
vector<string> vec_constants;

stack<type *> tempTuples;

extern int yylineno; extern char * yytext; extern char linebuf[500]; 
extern int tokenpos;
%}

%union { char * name; int cname;
         float num;
         int numint;
         term * trm;
         type * c_type;
         condition * cond;   }

%token IMPORT QUIT ARROW PROVE KB TYPE
%token EAGER CACHE LASTRESORT PERSISTENT 
%token DATA VAR CONST EQUAL NOTEQUAL BOX EXISTS FORALL TURNSTILE
%token AND OR NOT IMPLIES IFF ADD SUB MAX MIN MUL DIV MOD SIN COS SQRT EXP ATAN2
%token IF THEN ELSE ITE
%token SIGMA PI MYLT MYLTE MYGT MYGTE MYEQ MYNEQ ASSIGN 
%token TRUE FALSE CONS EMPTYLIST
%token BOOL INT FLOAT CHAR STRING LISTRING STRLIST
%token <name> FILENAME
%token <name> VARIABLE
%token <name> DATA_CONSTRUCTOR
%token <numint> DATA_CONSTRUCTOR_INT
%token <num> DATA_CONSTRUCTOR_FLOAT
%token <name> DATA_CONSTRUCTOR_STRING
%token <name> DATA_CONSTRUCTOR_CHAR
%token <name> SYN_VARIABLE
%token <name> IDENTIFIER1
%token <name> IDENTIFIER2
%right ARROW
%left '*'
%type <trm> term_schema
%type <cond> sv_condition
%type <c_type> type
%type <name> dataconstructor
%type <cname> functionsymbol
%type <numint> stmt_ctrl_directive
%%

input : program_statements ;
program_statements : /* empty */ | program_statements program_statement ;
program_statement : import | type_info | statement_schema | query | quit ;

quit : QUIT ';' { cout << "Quiting...\n"; cleanup(); exit(0); } ;
import : IMPORT FILENAME ';'
        { if (imported.find($2) == imported.end()) {
            FILE * in = fopen($2, "r"); 
            if (!in) { 
                  cerr << "Error reading from " << $2 << endl; assert(false); }
            quiet = true;
            switchBuffer(in); cerr << "  Reading " << $2 << "...";
            imported.insert($2);
          } 
          // if (!quiet) cerr << "prompt> ";
        }
       ;
query : ':' term_schema ';'           { bool program_okay = typeCheck();
                                        type * ret = NULL;
                                        term * answer = NULL;
                                        if (program_okay && typecheck) ret = wellTyped($2); 
                                        if (program_okay) {
                                                if (ret) delete_type(ret);
                                                int osel = getSelector(); setSelector(STDOUT);
                                                ioprint("  Query: "); $2->print(); ioprintln(); /*$*/
                                                setSelector(osel);
                                                $2->reduceRpt();
                                                answer = $2; //$
                                                // cout << "Total candidate redexes tried = " << tried << endl;
                                                int osel2 = getSelector(); setSelector(STDOUT);
                                                ioprint("Steps = "); ioprint(ltime);
                                                if (optimise) { ioprint(" ("); ioprint(cltime); ioprint(" cached step(s))"); }
                                                ioprint("\n  Answer: "); 
                                                answer->print(); ioprintln(" ;");
                                                setSelector(osel2);
                                                // answer->freememory();
                                                ltime = 0; cltime = 0;
                                                cerr << "prompt> ";
                                        } else {
                                                cerr << "  Error: Query not evaluated.\n";
                                                if (!quiet) cerr << "prompt> "; 
                                        } 
                                        if (answer) answer->freememory(); }
      | ':' term_schema '|' CACHE ';' { term * oquery = $2->clone();
                                        bool program_okay = typeCheck();
                                        type * ret = NULL;
                                        term * answer = NULL;
                                        if (program_okay && typecheck) ret = wellTyped($2); 
                                        if (program_okay) {
                                                if (ret) delete_type(ret);
                                                int osel = getSelector(); setSelector(STDOUT);
                                                ioprint("  Query: "); $2->print(); ioprintln(); /*$*/
                                                setSelector(osel);
                                                $2->reduceRpt();
                                                answer = $2; //$
                                                // cout << "Total candidate redexes tried = " << tried << endl;
                                                int osel2 = getSelector(); setSelector(STDOUT);
                                                ioprint("Steps = "); ioprint(ltime);
                                                if (optimise) { ioprint(" ("); ioprint(cltime); ioprint(" cached step(s))"); }
                                                ioprint("\n  Answer: "); 
                                                answer->print(); ioprintln(" ;");
                                                setSelector(osel2);
                                                // answer->freememory();
                                                ltime = 0; cltime = 0;
                                                cerr << "prompt> ";
                                        } else {
                                                cerr << "  Error: Query not evaluated.\n";
                                                if (!quiet) cerr << "prompt> "; 
                                        }
                                        statementType * st = new statementType(); 
                                        term * head = oquery;
                                        term * body = answer;
                                        term * p = newT2Args(F, iEqual); p->initT2Args(head, body);
                                        st->stmt = p;
                                        head->labelStaticBoundVars(); body->labelStaticBoundVars();
                                        st->stmt->collectSharedVars();
                                        head->collectFreeVars(st->stmt, 1);
                                        head->precomputeFreeVars();

                                        term * leftmost = head->spineTip(st->numargs);
                                        if (leftmost->isF()) {
                                                st->anchor = leftmost->cname;
                                                insert_ftable(leftmost->cname, st->numargs); }
                                        cachedStatements.push_back(st); }
      ;
type_info : functionsymbol ':' type ';'  { insert_constant($1, $3); }
          | constructordecl
          | syndecl
          ;
functionsymbol : IDENTIFIER1 { $$ = insert_symbol($1); } 
            | SIGMA        { $$ = iSigma; }
            | PI           { $$ = iPi; }
            | AND          { $$ = iAnd; }
            | OR           { $$ = iOr; }
            | NOT          { $$ = iNot; }
            | IMPLIES      { $$ = iImplies; }
            | ITE          { $$ = iIte; }
            | IFF          { $$ = iIff; }
            | ADD          { $$ = iAdd; }
            | SUB          { $$ = iSub; }
            | MAX          { $$ = iMax; }
            | MIN          { $$ = iMin; }
            | MUL          { $$ = iMul; }
            | DIV          { $$ = iDiv; }
            | MOD          { $$ = iMod; }
            | MYLT         { $$ = iLT; }
            | MYLTE        { $$ = iLTE; }
            | MYGT         { $$ = iGT; }
            | MYGTE        { $$ = iGTE; }
            | MYEQ         { $$ = iEqual; }
            | MYNEQ        { $$ = iNEqual; }
            | ASSIGN       { $$ = iAssign; }
            | TRUE         { $$ = iTrue; }
            | FALSE        { $$ = iFalse; }
            | CONS         { $$ = iHash; }
            | EMPTYLIST    { $$ = iEmptyList; }
            ;
constructordecl : dataconstructors ':' type ';' 
                  { string tname($3->getName());
                    type * t = new type_udefined(tname, vec_constants);
                    if ($3->isUdefined())       
                           insert_type(tname, UDEFINED, t);
                    insert_constant(insert_symbol(vec_constants[0]), $3);
                    for (unint i=1; i!=vec_constants.size(); i++)
                            insert_constant(insert_symbol(vec_constants[i]), 
                                            $3->clone());
                    vec_constants.clear();
                    // if (!quiet) cerr << "prompt> "; 
                  }
                ;
dataconstructors : dataconstructor  { vec_constants.push_back($1); }
                 | dataconstructors ',' dataconstructor 
                   { vec_constants.push_back($3); }
                 ;
dataconstructor : IDENTIFIER2 { $$ = $1; }
                | DATA_CONSTRUCTOR { $$ = $1; }
                ;
syndecl : TYPE IDENTIFIER2 MYEQ type ';' 
           { string t($2); insert_type(t,SYNONYM,$4); }
        ;
type : VARIABLE { string tname($1); $$ = new type_parameter(tname); }
     | IDENTIFIER1 { string tname($1); $$ = new type_parameter(tname); }
     | BOOL     { $$ = new type("Bool"); }
     | INT      { $$ = new type("Int"); }
     | FLOAT    { $$ = new type("Float"); } 
     | CHAR     { $$ = new type("Char"); }
     | STRING   { $$ = new type("String"); }
     | LISTRING { $$ = new type_alg("List"); $$->addAlpha(new type("Char")); }
     | IDENTIFIER2 
        { string tname($1);
          pair<int,type *> p = get_type(tname);
          if (p.second == NULL) $$ = new type_udefined(tname);
          else {  if (p.first == UDEFINED) $$ = p.second->clone();
                  else $$ = new type_synonym(tname, p.second->clone()); }
        }      
     | '(' IDENTIFIER2 types ')' 
        { string tname($2); 
          type_tuple * rem = dcast<type_tuple *>(tempTuples.top());
          tempTuples.pop(); 
          $$ = new type_alg(tname, rem);
          delete_type(rem); 
        }
     | '(' products ')' { $$ = tempTuples.top(); tempTuples.pop(); }
     | type ARROW type { $$ = new type_abs($1, $3); }
     | '(' type ')' { $$ = $2; }
     ;

products : products '*' type { tempTuples.top()->addAlpha($3); }
         | type '*' type     { tempTuples.push(new type_tuple);
                               tempTuples.top()->addAlpha($1);
                               tempTuples.top()->addAlpha($3);  }
         ;
types : type 
         { tempTuples.push(new type_tuple); tempTuples.top()->addAlpha($1); }
      | types type 
         { tempTuples.top()->addAlpha($2); }
      ;
statement_schema : 
         term_schema MYEQ term_schema ';'
           { statementType * st = new statementType(); 
             term * head = $1; term * body = $3;
             term * leftmost = head->spineTip(st->numargs); //$
             assert(leftmost->isF());
             // make sure all free variables in body appears in the head
             // head->labelVariables(0); body->labelVariables(0);
             head->getFreeVars(); body->getFreeVars();

             for (int i=0; i!=body->frvarsize; i++) {
                     if (body->frvars[i] == -5) continue;
                     bool done = false;
                     for (int j=0; j!=$1->frvarsize; j++)
                             if (body->frvars[i] == head->frvars[j]) { 
                                     done = true; break; }
                     if (!done) {
                             setSelector(STDERR);
                             cerr << " *** Error parsing statement: "; 
                             head->print(); cerr << " = "; body->print(); cerr << endl;
                             cerr << "Variable " << getString(body->frvars[i]) << 
                                     " free in body but not free in head.\n";
                             assert(false);
                     }
              }
             // make sure every free variable occurs only once in the head
             for (int i=0; i!=head->frvarsize; i++) {
                     if (head->frvars[i] == -5) continue;
                     if (head->frvars[i] == iWildcard) continue;
                     for (int j=i+1; j!=head->frvarsize; j++)
                             if (head->frvars[i] == head->frvars[j]) {
                                     setSelector(STDERR);
                                     cerr << " *** Error parsing statement: "; 
                                     head->print(); cerr << " = "; body->print(); cerr<<endl;
                                     cerr << "Variable " << head->frvars[i] << 
                                             " occurs multiple times in head.\n";
                                     assert(false);
                             }
              }

             // make sure lambda variables in the head are unique
             multiset<int> lvars; head->collectLambdaVars(lvars);
             multiset<int>::iterator p = lvars.begin();
             while (p != lvars.end()) {
                     if (lvars.count(*p) > 1) {
                             setSelector(STDERR);
                             cerr << " *** Error parsing statement: ";
                             head->print(); cerr << " = "; body->print(); cerr<<endl;
                             cerr << "Lambda variable " << getString(*p) << 
                                     " occurs multiple times in head.\n";
                             assert(false);
                     }
                     p++;
             }
             // everything is okay now
             st->anchor = leftmost->cname;
             insert_ftable(leftmost->cname, st->numargs);
             st->stmt = newT2Args(F, iEqual); st->stmt->initT2Args(head, body);
             head->labelStaticBoundVars(); body->labelStaticBoundVars();
             st->stmt->collectSharedVars();
             head->collectFreeVars(st->stmt, 1);
             head->precomputeFreeVars();
             insert_statement(st); }
       | term_schema MYEQ term_schema ';' stmt_ctrl_directive ';'
           { statementType * st = new statementType(); 
             term * head = $1; term * body = $3;
             term * leftmost = head->spineTip(st->numargs); //$
             assert(leftmost->isF());
             // make sure all free variables in body appears in the head
             // head->labelVariables(0); body->labelVariables(0);
             head->getFreeVars(); body->getFreeVars();

             for (int i=0; i!=body->frvarsize; i++) {
                     if (body->frvars[i] == -5) continue;
                     bool done = false;
                     for (int j=0; j!=$1->frvarsize; j++)
                             if (body->frvars[i] == head->frvars[j]) { 
                                     done = true; break; }
                     if (!done) {
                             setSelector(STDERR);
                             cerr << " *** Error parsing statement: "; 
                             head->print(); cerr << " = "; body->print(); cerr << endl;
                             cerr << "Variable " << getString(body->frvars[i]) << 
                                     " free in body but not free in head.\n";
                             assert(false);
                     }
              }
             // make sure every free variable occurs only once in the head
             for (int i=0; i!=head->frvarsize; i++) {
                     if (head->frvars[i] == -5) continue;
                     if (head->frvars[i] == iWildcard) continue;
                     for (int j=i+1; j!=head->frvarsize; j++)
                             if (head->frvars[i] == head->frvars[j]) {
                                     setSelector(STDERR);
                                     cerr << " *** Error parsing statement: "; 
                                     head->print(); cerr << " = "; body->print(); cerr<<endl;
                                     cerr << "Variable " << head->frvars[i] << 
                                             " occurs multiple times in head.\n";
                                     assert(false);
                             }
              }

             // make sure lambda variables in the head are unique
             multiset<int> lvars; head->collectLambdaVars(lvars);
             multiset<int>::iterator p = lvars.begin();
             while (p != lvars.end()) {
                     if (lvars.count(*p) > 1) {
                             setSelector(STDERR);
                             cerr << " *** Error parsing statement: ";
                             head->print(); cerr << " = "; body->print(); cerr<<endl;
                             cerr << "Lambda variable " << getString(*p) << 
                                     " occurs multiple times in head.\n";
                             assert(false);
                     }
                     p++;
             }
             // everything is okay now
             st->anchor = leftmost->cname;
             insert_ftable(leftmost->cname, st->numargs);
             st->stmt = newT2Args(F, iEqual); st->stmt->initT2Args(head, body);
             head->labelStaticBoundVars(); body->labelStaticBoundVars();
             st->stmt->collectSharedVars();
             head->collectFreeVars(st->stmt, 1);
             head->precomputeFreeVars();
             insert_statement(st);
             if ($5 == EAGER) st->eager = true;
             if ($5 == LASTRESORT) st->lastresort = true;
             if ($5 == PERSISTENT) st->persistent = true;
             if ($5 == CACHE) cacheFuncs.insert(leftmost->cname);
           }
       ;

stmt_ctrl_directive : EAGER { $$ = EAGER ; }
                    | LASTRESORT { $$ = LASTRESORT; }
                    | CACHE { $$ = CACHE; }
                    | PERSISTENT { $$ = PERSISTENT; }
                    ;
term_schema : SYN_VARIABLE { $$ = new_term(SV, insert_symbol($1)); }
            | SYN_VARIABLE sv_condition 
                           { $$=new_term(SV, insert_symbol($1)); $$->cond = $2;}
            | VARIABLE     { if ($1[0] == '_') 
                                  $$ = new_term(V, iWildcard); 
                             else $$ = new_term(V, insert_symbol($1)); }
            | SIGMA        { $$ = new_term(F, iSigma); }
            | PI           { $$ = new_term(F, iPi); }
            | AND          { $$ = new_term(F, iAnd); }
            | OR           { $$ = new_term(F, iOr); }
            | NOT          { $$ = new_term(F, iNot); }
            | IMPLIES      { $$ = new_term(F, iImplies); }
            | ITE          { $$ = new_term(F, iIte); }
            | IFF          { $$ = new_term(F, iIff); }
            | ADD          { $$ = new_term(F, iAdd); }
            | SUB          { $$ = new_term(F, iSub); }
            | MAX          { $$ = new_term(F, iMax); }
            | MIN          { $$ = new_term(F, iMin); }
            | MUL          { $$ = new_term(F, iMul); }
            | DIV          { $$ = new_term(F, iDiv); }
            | MOD          { $$ = new_term(F, iMod); }
            | SIN          { $$ = new_term(F, iSin); }
            | COS          { $$ = new_term(F, iCos); }
            | SQRT         { $$ = new_term(F, iSqrt); }
            | EXP          { $$ = new_term(F, iExp); }
            | ATAN2        { $$ = new_term(F, iAtan2); }
            | MYLT         { $$ = new_term(F, iLT); }
            | MYLTE        { $$ = new_term(F, iLTE); }
            | MYGT         { $$ = new_term(F, iGT); }
            | MYGTE        { $$ = new_term(F, iGTE); }
            | MYEQ         { $$ = new_term(F, iEqual); }
            | MYNEQ        { $$ = new_term(F, iNEqual); }
            | ASSIGN       { $$ = new_term(F, iAssign); }
            | TRUE         { $$ = new_term(D, iTrue); }
            | FALSE        { $$ = new_term(D, iFalse); }
            | CONS         { $$ = new_term(D, iHash); }
            | EMPTYLIST    { $$ = new_term(D, iEmptyList); }
            | DATA_CONSTRUCTOR_INT    { $$ = new_term_int($1); }
            | DATA_CONSTRUCTOR_FLOAT  { $$ = new_term_float($1); }
            | DATA_CONSTRUCTOR_CHAR   
               { int code = insert_symbol($1); $$ = new_term(D, code); }
            | DATA_CONSTRUCTOR_STRING 
              { int code = insert_symbol($1); strings.insert(code);
                $$ = new_term(D, code); }
              | STRLIST DATA_CONSTRUCTOR_STRING  
                 { string x($2); int size = x.size();
                   term * elist = new_term(D, iEmptyList);
                   for (int i=size-2; i!=0; i--) {
                           term * temp = newT2Args(D, iHash);
                           string character("'"); character += x[i]; character += "'";
                           int code = insert_symbol(character); 
                           temp->initT2Args(new_term(D, code), elist);
                           elist = temp;
                   }
                   $$ = elist; /*$*/ }
            | IDENTIFIER1  { $$ = new_term(F, insert_symbol($1)); }
            | IDENTIFIER2  { $$ = new_term(D, insert_symbol($1)); }
            | '\\' VARIABLE '.' term_schema
                  { $$ = new_term(ABS); 
                    // $$->lc = new_term(V,insert_symbol($2));
                    // $$->rc = $4;
                    $$->insert(new_term(V,insert_symbol($2))); 
                    $$->insert($4);
                  }
            | '\\' SYN_VARIABLE '.' term_schema
                  { $$ = new_term(ABS); 
                    // $$->lc = new_term(SV,insert_symbol($2));
                    // $$->rc = $4;
                    $$->insert(new_term(SV,insert_symbol($2))); 
                    $$->insert($4);
                  }
            | IF term_schema THEN term_schema ELSE term_schema 
              { $$ = new_term(APP);
                // $$->lc = new_term(F, iIte);
                $$->insert(new_term(F, iIte));
                term * temp = new_term(PROD);
                /* temp->tuple = $2;
                temp->tuple->next = $4;
                temp->tuple->next->next = $6;
                temp->tuple->next->next->next = NULL; */
                temp->insert($2); temp->insert($4); temp->insert($6);
                // $$->rc = temp;
                $$->insert(temp);
              } 
            | '\\' EXISTS VARIABLE '.' term_schema 
                 { $$ = new_term(APP); 
                   // $$->lc = new_term(F, iSigma);
                   $$->insert(new_term(F, iSigma));
                   term * abs = new_term(ABS);
                   // abs->lc = new_term(V, insert_symbol($3)); abs->rc = $5;
                   abs->insert(new_term(V, insert_symbol($3))); abs->insert($5);
                   // $$->rc = abs;
                   $$->insert(abs);   
                 }
            | '\\' FORALL VARIABLE '.' term_schema 
                 { $$ = new_term(APP); 
                   // $$->lc = new_term(F, iPi);
                   $$->insert(new_term(F, iPi));
                   term * abs = new_term(ABS);
                   // abs->lc = new_term(V, insert_symbol($3)); abs->rc = $5;
                   abs->insert(new_term(V, insert_symbol($3))); abs->insert($5);
                   // $$->rc = abs;
                   $$->insert(abs);   
                 }
            | BOX DATA_CONSTRUCTOR_INT term_schema 
                   { $$ = new_term(MODAL); $$->modality = $2; 
                     /* $$->lc = $3; */ $$->insert($3); }
            | '(' term_schema term_schema ')'
                   { $$ = new_term(APP); 
                     // $$->lc = $2; $$->rc = $3;
                     $$->insert($2); $$->insert($3); 
                   }
            | '(' term_schema term_schema term_schemas ')'
                      { $$ = new_term(APP);
                        // $$->lc = $2; $$->rc = $3; 
                        $$->insert($2); $$->insert($3);

                        int size = temp_fields.size(); int psize = 0;
                        while (temp_fields[size-1-psize] != NULL) psize++;

                        term * temp;
                        for (int i=size-psize; i!=size; i++) {
                                temp = new_term(APP);
                                // temp->lc = $$; temp->rc = temp_fields[i];
                                temp->insert($$);
                                temp->insert(temp_fields[i]);
                                $$ = temp;
                        }
                        while (psize+1) { temp_fields.pop_back(); psize--; }
                      }
            | '(' ')'   { $$ = new_term(PROD); }
            | '(' term_schemas_product ')'
                      { $$ = new_term(PROD);
                        int size = temp_fields.size(); int psize = 0;
                        while (temp_fields[size-1-psize] != NULL) psize++;

                        for (int i=size-psize; i!=size; i++) {
                            $$->insert(temp_fields[i]);
                        }      
                        while (psize+1) { temp_fields.pop_back(); psize--; }
                      } 
            | '{' '}' 
                   {  $$ = new_term(ABS); 
                      // $$->lc = new_term(V, newPVar());
                      // $$->rc = new_term(D, iFalse);
                      $$->insert(new_term(V, newPVar()));
                      $$->insert(new_term(D, iFalse)); 
                   }
              | '{' term_schemas_product '}'
                   {  int pv = newPVar();
                      $$ = new_term(ABS); $$->insert(new_term(V, pv));

                      term * arg2 = new_term(D, iFalse);
                      int i = temp_fields.size()-1;
                      while (temp_fields[i] != NULL) {
                              term * ite = new_term(APP);
                              ite->insert(new_term(F, iIte));
                              ite->insert(new_term(PROD));

                              term * eq = newT2Args(F, iEqual);
                              eq->initT2Args(new_term(V, pv), temp_fields[i]);

                              ite->fields[1]->insert(eq);
                              ite->fields[1]->insert(new_term(D, iTrue));
                              ite->fields[1]->insert(arg2); 

                              arg2 = ite;
                              i--;
                      }
                      $$->insert(arg2); // $
                      // setSelector(STDOUT); $$->print(); setSelector(SILENT);

                      int size = temp_fields.size(); int psize = 0;
                      while (temp_fields[size-1-psize] != NULL) psize++;      
                      while (psize+1) { temp_fields.pop_back(); psize--; }
                    } 
            | '[' ']' {}
              | '[' term_schemas_product ']' 
                  {   int tsize = temp_fields.size();
                      term * tail = newT2Args(D, iHash); 
                      tail->initT2Args(temp_fields[tsize-1], new_term(D, iEmptyList));
                      int i = tsize - 2;
                      while (temp_fields[i] != NULL) {
                              term * current = newT2Args(D, iHash);
                              current->initT2Args(temp_fields[i], tail);
                              tail = current;
                              i--;
                      }
                      $$ = tail;
                      int size = temp_fields.size(); int psize = 0;
                      while (temp_fields[size-1-psize] != NULL) psize++;      
                      while (psize+1) { temp_fields.pop_back(); psize--; }
                  }           
            ;

term_schemas : term_schema
               { temp_fields.push_back(NULL); // start a new mult app
                 temp_fields.push_back($1); }
             | term_schemas term_schema
               { temp_fields.push_back($2); }
             ;
term_schemas_product : term_schema
                     { temp_fields.push_back(NULL); // start a new product
                       temp_fields.push_back($1); }
                     | term_schemas_product ',' term_schema
                       { temp_fields.push_back($3); }
                     ;
sv_condition : '/' VAR '/' { $$ = new condition; $$->tag = CVAR; }
             | '/' CONST '/' { $$ = new condition; $$->tag = CCONST; }
             | '/' EQUAL ',' SYN_VARIABLE '/'
               { $$ = new condition; $$->tag = CEQUAL; 
                 $$->svname = insert_symbol($4); }
             | '/' NOTEQUAL ',' SYN_VARIABLE '/'
               { $$ = new condition; $$->tag = CNOTEQUAL; 
                 $$->svname = insert_symbol($4); }
             ;

%%
void yyerror(const char * s) {        
        cerr << yylineno << ": " << s << " at " << yytext << " in this line\n";
        cerr << linebuf << endl; 
        for (int i=0; i!=tokenpos-1; i++) cerr << " ";
        cerr << "^" << endl;
        if (!quiet) cerr << "prompt> ";
}
#ifndef __APPLE__
#ifndef __sun
#include <getopt.h>
#endif
#endif

#include <unistd.h>
#include <signal.h>

static void handle_signal(int sig) {
        cout << "Interrupted....\n"; 
        if (interrupted) { cleanup(); exit(1); }
        interrupted = true; 
}

int main(int argc, char ** argv) {
        interactive = false; quiet = true;
        char c; 
        while ((c = getopt(argc, argv, "vitobds")) != EOF) {
                switch (c) { 
                case 'v': verbose++; break;   
                case 'i': interactive = true; break;
                case 't': typecheck = false; break;
                case 'o': optimise = true; break; 
                case 'b': backchain = true; break;
                case 'd': outermost = true; break;
                case 's': stepByStep = true; break;
                }
        }
        if (verbose) setSelector(STDOUT); else setSelector(SILENT);
        makeHeap();
        initFuncTable();
        initialise_constants();

        signal(SIGINT, handle_signal);
  
        logcache = fopen("log.cache", "r+"); assert(logcache);

        if (interactive) cerr << "prompt> ";
        do { yyparse(); } while (!feof(yyin));

        fclose(logcache);

        cleanup();
        return 0;
}
void cleanup() { 
        cleanup_statements(); cleanup_formulas();
        cleanup_constants(); cleanup_synonyms(); 
        mem_report(); 
}                                                    
