%{
#include <iostream>
#include <string.h>
#include <stack>
#include "terms.h"
#include "unification.h"
#include "y.tab.h"
using namespace std;

char linebuf[5000];
int tokenpos = 0;
int import_level = 0;
bool quiet;
bool interactive;
%}
%option yylineno
%x CMNT
%%
\{\-       BEGIN CMNT;
<CMNT>.|\n ;
<CMNT>\-\} BEGIN INITIAL;
[\t ]+             { tokenpos += yyleng; }
\-\-.*             { // cout << "--\n"; 
                     tokenpos += yyleng; }
\n.*               { if (!quiet) cerr << "prompt> ";
                     assert(strlen(yytext+1) <= 5000);
                     strcpy(linebuf, yytext+1); yyless(1); tokenpos = 0;}
LastResort         { tokenpos += yyleng; return LASTRESORT; }
Cache              { tokenpos += yyleng; return CACHE; }
Eager              { tokenpos += yyleng; return EAGER; }
Persistent         { tokenpos += yyleng; return PERSISTENT; }
type               { tokenpos += yyleng; return TYPE; }
prove              { tokenpos += yyleng; return PROVE; }
KB                 { tokenpos += yyleng; return KB; }
Bool               { tokenpos += yyleng; return BOOL; }
Int                { tokenpos += yyleng; return INT; }
Float              { tokenpos += yyleng; return FLOAT; }
Char               { tokenpos += yyleng; return CHAR; }
String             { tokenpos += yyleng; return STRING; }
ListString         { tokenpos += yyleng; return LISTRING; }
\-\>               { tokenpos += yyleng; return ARROW; }
import             { tokenpos += yyleng; return IMPORT; }
quit               { tokenpos += yyleng; return QUIT; }
VAR                { tokenpos += yyleng; return VAR; }
CONST              { tokenpos += yyleng; return CONST; }
EQUAL              { tokenpos += yyleng; return EQUAL; }
NOTEQUAL           { tokenpos += yyleng; return NOTEQUAL; }
StrList            { tokenpos += yyleng; return STRLIST; }
add                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return ADD; }
sub                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return SUB; }
max                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MAX; }
min                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MIN; }
mul                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MUL; }
div                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return DIV; }
mod                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MOD; }
sin                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return SIN; }
cos                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return COS; }
sqrt               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return SQRT; }
exp                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return EXP; }
atan2              { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return ATAN2; }
if                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return IF; }
then               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return THEN; }
else               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return ELSE; }
ite                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return ITE; }
\&\&               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return AND; }
\|\|               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return OR; }
not                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return NOT; }
implies            { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return IMPLIES; }
iff                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return IFF; }
sigma              { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return SIGMA; }
pi                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return PI; }
exists             { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return EXISTS; }
forall             { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return FORALL; }
box                { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return BOX; }
\|\-               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return TURNSTILE; }
assign             { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return ASSIGN; }
\=               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                strcpy(yylval.name, yytext);; return MYEQ; }
\/\=               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MYNEQ; }
\<\=               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MYLTE; }
\<                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MYLT; }
\>\=               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MYGTE; }
\>                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return MYGT; }
True               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return TRUE; }
False              { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return FALSE; }
\#                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return CONS;}
\[\]               { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return EMPTYLIST;}
-?[0-9]+           { tokenpos += yyleng; yylval.numint = atoi(yytext); 
                     return DATA_CONSTRUCTOR_INT; }
-?[0-9]+\.[0-9]+   { tokenpos += yyleng; yylval.num = atof(yytext); 
                     return DATA_CONSTRUCTOR_FLOAT; }
\'[^']\'           { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext); 
                     return DATA_CONSTRUCTOR_CHAR; /*'*/}
\"[^"]*\"          { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext); 
                     return DATA_CONSTRUCTOR_STRING; /*"*/ }
[a-zA-Z\/0-9\_\.\-]+\.es  { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                         strcpy(yylval.name, yytext);; return FILENAME; }
\_                 { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return VARIABLE; }
[a-z][0-9\_]*      { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return VARIABLE; }
\:[a-z]+[0-9\_]*   { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return VARIABLE; }
pv(e|t)[0-9]*      { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return VARIABLE; }
[a-zA-Z][0-9]*\_SV { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                  strcpy(yylval.name, yytext);; return SYN_VARIABLE; }
[a-z][a-zA-Z0-9\-\_\']* { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                       strcpy(yylval.name, yytext);; return IDENTIFIER1; }
[A-Z][a-zA-Z0-9\-\_\']* { tokenpos += yyleng; yylval.name = new char[strlen(yytext)+1];
                                       strcpy(yylval.name, yytext);; return IDENTIFIER2; }
.                     { tokenpos += yyleng; return yytext[0]; }

%%

#define YY_NO_UNPUT 1 // suppose to get rid of warning message
stack<YY_BUFFER_STATE> import_stack;

void switchBuffer(FILE * in) {
        YY_BUFFER_STATE current = YY_CURRENT_BUFFER;
        import_stack.push(current); 
        // cout << "Switching to new file.\n";
        YY_BUFFER_STATE newf = yy_create_buffer(in, YY_BUF_SIZE);
        yy_switch_to_buffer(newf);
        import_level++;
}
int yywrap() { 
        import_level--;
        cerr << "done "; if (import_level == 0) cerr << endl; 
        if (import_level == 0 && interactive) quiet = false;
        if (!quiet) cerr << "prompt> "; 
        YY_BUFFER_STATE current = YY_CURRENT_BUFFER;
        yy_delete_buffer(current);
        if (import_stack.size()) {
                yy_switch_to_buffer(import_stack.top());
                import_stack.pop();
                return 0;
        }
        return 1; 
}
int mywrap() { 
        if (interactive) quiet = false;
        if (!quiet) cerr << "prompt> "; 
        // YY_BUFFER_STATE current = YY_CURRENT_BUFFER;
        // yy_delete_buffer(current);
        if (import_stack.size()) {
                yy_switch_to_buffer(import_stack.top());
                import_stack.pop();
                return 0;
        }
        return 1; 
}
