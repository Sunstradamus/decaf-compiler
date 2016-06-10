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

/*  op = option (zero or one)
    st = star (zero or more)
    pl = plus (one or more)
    cs = comma separated (1 or more separated by commas)
*/


start: program

program: st_extern decafpackage
    { 
      //        ProgramAST *prog = new ProgramAST((decafStmtList *)$1, (PackageAST *)$2); 
      //        if (printAST) {
      //            cout << getString(prog) << endl;
      //        }
      //        delete prog;
      cout << "hello World" << endl;
    }

st_extern: T_EXTERN T_FUNC T_ID T_LPAREN op_cs_externtype T_RPAREN methodtype T_SEMICOLON st_extern
           | /* empty string */
    {
      //      decafStmtList *slist = new decafStmtList();
      //      $$ = slist;
    };

decafpackage: T_PACKAGE T_ID T_LCB field_decl method_decl T_RCB
    { 
      //      $$ = new PackageAST(*$2, new decafStmtList(), new decafStmtList());
      //      delete $2;
    };

field_decl: T_VAR T_ID decaftype T_SEMICOLON field_decl
          | T_VAR T_ID T_COMMA cs_id decaftype T_SEMICOLON field_decl
          | T_VAR cs_id arraytype T_SEMICOLON field_decl
          | T_VAR T_ID decaftype T_ASSIGN constant T_SEMICOLON field_decl
          | /* empty string */
    {};

method_decl: T_FUNC T_ID T_LPAREN op_cs_idtype T_RPAREN methodtype decafblock method_decl
           | /* empty string */
    {};

decafblock: T_LCB var_decls statements T_RCB {};

var_decls: T_VAR cs_id decaftype T_SEMICOLON var_decls
         | /* empty string */
    {};

statements: statement statements
          | /* empty string */
    {};

statement: decafblock
         | assign T_SEMICOLON
         | methodcall T_SEMICOLON
         | T_IF T_LPAREN decafexpr T_RPAREN decafblock op_else
         | T_WHILE T_LPAREN decafexpr T_RPAREN decafblock
         | T_FOR T_LPAREN cs_assign T_SEMICOLON decafexpr T_SEMICOLON cs_assign T_RPAREN decafblock
         | T_RETURN op_returnexpr T_SEMICOLON
         | T_BREAK T_SEMICOLON
         | T_CONTINUE T_SEMICOLON
    {};

decafexpr: expr binaryop decafexpr
         | expr {};

expr: T_ID
    | methodcall
    | constant
    | T_LPAREN decafexpr T_RPAREN
    | unaryop expr
    | T_ID T_LSB decafexpr T_RSB

unaryop: T_NOT | T_MINUS {};

binaryop: arithmeticop
        | booleanop
    {};

arithmeticop: T_PLUS | T_MINUS | T_MULT | T_DIV | T_LEFTSHIFT | T_RIGHTSHIFT | T_MOD {};

booleanop: T_EQ | T_NEQ | T_LT | T_LEQ | T_GT | T_GEQ | T_AND | T_OR {};

op_returnexpr: T_LPAREN op_decafexpr T_RPAREN
             | /* empty string */
    {};

op_decafexpr: decafexpr | /* empty string */ {};

op_else: T_ELSE decafblock
       | /* empty string */
    {};

cs_assign: assign T_COMMA cs_assign
         | assign
    {};

assign: lvalue T_ASSIGN decafexpr {};

lvalue: T_ID
      | T_ID T_LSB decafexpr T_RSB
    {};

methodcall: T_ID T_LPAREN op_cs_methodarg T_RPAREN

op_cs_methodarg: cs_methodarg
               | /* empty string */
    {};

cs_methodarg: methodarg T_COMMA cs_methodarg
            | methodarg
    {};

methodarg: decafexpr
         | T_STRINGCONSTANT
    {};

cs_id: T_ID T_COMMA cs_id
     | T_ID
    {};

op_cs_externtype: cs_externtype
                | /* empty string */
    {};

cs_externtype: externtype T_COMMA cs_externtype
             | externtype
    {};

op_cs_idtype: cs_idtype | /* empty string */ {};

cs_idtype: T_ID decaftype T_COMMA cs_idtype
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

