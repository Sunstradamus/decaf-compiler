%{
#include <iostream>
#include <ostream>
#include <string>
#include <cstdlib>
#include "decafast-defs.h"

int yylex(void);
int yyerror(char *); 

// print AST?
bool printAST = true;

#include "decafast.cc"

using namespace std;

%}

%union{
    class decafAST *ast;
    std::string *sval;
 }

%token T_AND
%token T_ASSIGN
%token T_COMMA
%token T_DIV
%token T_DOT
%token T_EQ
%token T_GEQ
%token T_GT
%token T_LCB
%token T_LEFTSHIFT
%token T_LEQ
%token T_LPAREN
%token T_LSB
%token T_LT
%token T_MINUS
%token T_MOD
%token T_MULT
%token T_NEQ
%token T_NOT
%token T_OR
%token T_PLUS
%token T_RCB
%token T_RIGHTSHIFT
%token T_RPAREN
%token T_RSB
%token T_SEMICOLON
%token T_BOOLTYPE
%token T_BREAK
%token T_CONTINUE
%token T_ELSE
%token T_EXTERN
%token T_FALSE
%token T_FOR
%token T_FUNC
%token T_IF
%token T_INTTYPE
%token T_NULL
%token T_PACKAGE
%token T_RETURN
%token T_STRINGTYPE
%token T_TRUE
%token T_VAR
%token T_VOID
%token T_WHILE
%token <sval> T_CHARCONSTANT
%token <sval> T_INTCONSTANT
%token <sval> T_STRINGCONSTANT
%token T_WHITESPACE
%token <sval> T_ID
%token T_COMMENT


 //%type <ast> extern_list decafpackage

%%

start: program

program: extern_list decafpackage
    { 
      //        ProgramAST *prog = new ProgramAST((decafStmtList *)$1, (PackageAST *)$2); 
      //		if (printAST) {
      //			cout << getString(prog) << endl;
      //		}
      //        delete prog;
      cout << "hello World" << endl;
    }

extern_list: T_EXTERN T_FUNC T_ID T_LPAREN externtype_lists T_RPAREN methodtype T_SEMICOLON extern_list
           | /* empty string */
    {
      //      decafStmtList *slist = new decafStmtList();
      //      $$ = slist;
    };

decafpackage: T_PACKAGE T_ID T_LCB field_decl method_decl T_RCB
    { 
      //      $$ = new PackageAST(*$3, new decafStmtList(), new decafStmtList());
      //      delete $3;
    };

field_decl: T_VAR T_ID decaftype T_SEMICOLON field_decl
          | T_VAR T_ID T_COMMA id_list decaftype T_SEMICOLON field_decl
          | T_VAR id_list arraytype T_SEMICOLON field_decl
          | T_VAR T_ID decaftype T_EQ constant T_SEMICOLON field_decl
          | /* empty string */
    {};

method_decl: T_FUNC T_ID T_LPAREN idtype_lists T_RPAREN methodtype decafblock method_decl
           | /* empty string */
           {};

decafblock: T_LCB T_RCB {};

id_list: T_ID T_COMMA id_list
       | T_ID
    {};

externtype_lists: externtype_list
                | /* empty string */
    {};

externtype_list: externtype T_COMMA externtype_list
               | externtype
    {};

idtype_lists: idtype_list | /* empty string */ {};

idtype_list: T_ID decaftype T_COMMA idtype_list
           | T_ID decaftype
    {};

// Basic definitions
decaftype: T_INTTYPE | T_BOOLTYPE {};

methodtype: T_VOID | decaftype {};

externtype: T_STRINGTYPE | methodtype {};

arraytype: T_LSB T_INTCONSTANT T_RSB decaftype {};

boolconstant: T_TRUE | T_FALSE {};

constant: T_INTCONSTANT | T_CHARCONSTANT | boolconstant {};

%%

int main() {
  // parse the input and create the abstract syntax tree
  int retval = yyparse();
  return(retval >= 1 ? EXIT_FAILURE : EXIT_SUCCESS);
}

