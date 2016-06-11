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

%token <ast> T_AND
%token <ast> T_ASSIGN
%token <ast> T_COMMA
%token <ast> T_DIV
%token <ast> T_DOT
%token <ast> T_EQ
%token <ast> T_GEQ
%token <ast> T_GT
%token <ast> T_LCB
%token <ast> T_LEFTSHIFT
%token <ast> T_LEQ
%token <ast> T_LPAREN
%token <ast> T_LSB
%token <ast> T_LT
%token <ast> T_MINUS
%token <ast> T_MOD
%token <ast> T_MULT
%token <ast> T_NEQ
%token <ast> T_NOT
%token <ast> T_OR
%token <ast> T_PLUS
%token <ast> T_RCB
%token <ast> T_RIGHTSHIFT
%token <ast> T_RPAREN
%token <ast> T_RSB
%token <ast> T_SEMICOLON
%token <ast> T_BOOLTYPE
%token <ast> T_BREAK
%token <ast> T_CONTINUE
%token <ast> T_ELSE
%token <ast> T_EXTERN
%token <ast> T_FALSE
%token <ast> T_FOR
%token <ast> T_FUNC
%token <ast> T_IF
%token <ast> T_INTTYPE
%token <ast> T_NULL
%token <ast> T_PACKAGE
%token <ast> T_RETURN
%token <ast> T_STRINGTYPE
%token <ast> T_TRUE
%token <ast> T_VAR
%token <ast> T_VOID
%token <ast> T_WHILE
%token <sval> T_CHARCONSTANT
%token <sval> T_INTCONSTANT
%token <sval> T_STRINGCONSTANT
%token T_WHITESPACE
%token <sval> T_ID
%token T_COMMENT

%type <ast> st_extern decafpackage decaftype methodtype externtype op_cs_externtype cs_externtype method_decl op_cs_idtype cs_idtype decafblock var_decls
%type <ast> cs_id statements statement assign methodcall boolconstant booleanop arithmeticop binaryop unaryop

%%

/*  op = option (zero or one)
    st = star (zero or more)
    pl = plus (one or more)
    cs = comma separated (1 or more separated by commas)
*/


start: program

program: st_extern decafpackage
{ 
  ProgramAST *prog = new ProgramAST((decafStmtList *)$1, (PackageAST *)$2); 
  if (printAST) {
      cout << getString(prog) << endl;
  }
  delete prog;
}

st_extern: T_EXTERN T_FUNC T_ID T_LPAREN op_cs_externtype T_RPAREN methodtype T_SEMICOLON st_extern
{
  decafStmtList *slist = (decafStmtList *)$9;
  ExternFunctionAST *func = new ExternFunctionAST(*$3, (decafType *)$7, (decafStmtList *)$5);
  slist->push_front(func);
  $$ = slist;
  delete $3;
}
           | /* empty string */
{
  decafStmtList *slist = new decafStmtList();
  $$ = slist;
}
           ;

decafpackage: T_PACKAGE T_ID T_LCB field_decl method_decl T_RCB
{ 
  $$ = new PackageAST(*$2, new decafStmtList(), (decafStmtList *)$5);
  delete $2;
};

field_decl: T_VAR T_ID decaftype T_SEMICOLON field_decl
          | T_VAR T_ID T_COMMA cs_id decaftype T_SEMICOLON field_decl
          | T_VAR cs_id arraytype T_SEMICOLON field_decl
          | T_VAR T_ID decaftype T_ASSIGN constant T_SEMICOLON field_decl
          | /* empty string */
    {};

method_decl: T_FUNC T_ID T_LPAREN op_cs_idtype T_RPAREN methodtype decafblock method_decl
{
  decafBlock *block = (decafBlock *)$7;
  MethodBlockAST *mbAST = block->getMethodBlock();
  MethodAST *method = new MethodAST(*$2, (decafType *)$6, (decafStmtList *)$4, mbAST);
  decafStmtList *slist = (decafStmtList *)$8;
  slist->push_front(method);
  $$ = slist;
  delete $2;
  delete block;
}
           | /* empty string */
{
  decafStmtList *slist = new decafStmtList();
  $$ = slist;
}
           ;

decafblock: T_LCB var_decls statements T_RCB
{
  $$ = new decafBlock((decafStmtList *)$2, new decafStmtList());
}

var_decls: T_VAR cs_id decaftype T_SEMICOLON var_decls
{
  decafStmtList *slist = (decafStmtList *)$5;
  decafType *type = (decafType *)$3;
  decafIdList *idlist = (decafIdList *)$2;
  for (list<string>::iterator i = idlist->begin(); i != idlist->end(); i++) {
    slist->push_front(new decafSymbol(*i, type->clone()));
  }
  $$ = slist;
  delete type;
  delete idlist;
}
         | /* empty string */
{
  decafStmtList *slist = new decafStmtList();
  $$ = slist;
}
         ;

statements: statement statements
{
  decafStmtList *slist = (decafStmtList *)$2;
  decafStatement *stmt = (decafStatement *)$1;
  slist->push_front(stmt);
  $$ = slist;
}
          | /* empty string */
{
  decafStmtList *slist = new decafStmtList();
  $$ = slist;
}
          ;

statement: decafblock
{
  decafBlock *block = (decafBlock *)$1;
  BlockAST *blockAST = block->getBlock();
  $$ = blockAST;
  delete block;
}
         | assign T_SEMICOLON
         | methodcall T_SEMICOLON
         | T_IF T_LPAREN decafexpr T_RPAREN decafblock op_else
         | T_WHILE T_LPAREN decafexpr T_RPAREN decafblock
         | T_FOR T_LPAREN cs_assign T_SEMICOLON decafexpr T_SEMICOLON cs_assign T_RPAREN decafblock
         | T_RETURN op_returnexpr T_SEMICOLON
         | T_BREAK T_SEMICOLON
{
  $$ = new decafBreakStmt();
}
         | T_CONTINUE T_SEMICOLON
{
  $$ = new decafContinueStmt();
}
         ;

decafexpr: expr binaryop decafexpr
         | expr {};

expr: T_ID
    | methodcall
    | constant
    | T_LPAREN decafexpr T_RPAREN
    | unaryop expr
    | T_ID T_LSB decafexpr T_RSB

unaryop: T_NOT
{
  $$ = new decafNotOperator();
}
       | T_MINUS
{
  $$ = new decafUnaryMinusOperator();
}
       ;

binaryop: arithmeticop
        | booleanop
        ;

arithmeticop: T_PLUS
{
  $$ = new decafPlusOperator();
}
            | T_MINUS
{
  $$ = new decafMinusOperator();
}
            | T_MULT
{
  $$ = new decafMultOperator();
}
            | T_DIV
{
  $$ = new decafDivOperator();
}
            | T_LEFTSHIFT
{
  $$ = new decafLeftshiftOperator();
}
            | T_RIGHTSHIFT
{
  $$ = new decafRightshiftOperator();
}
            | T_MOD
{
  $$ = new decafModOperator();
}
            ;

booleanop: T_EQ
{
  $$ = new decafEqOperator();
}
         | T_NEQ
{
  $$ = new decafNeqOperator();
}
         | T_LT
{
  $$ = new decafLtOperator();
}
         | T_LEQ
{
  $$ = new decafLeqOperator();
}
         | T_GT
{
  $$ = new decafGtOperator();
}
         | T_GEQ
{
  $$ = new decafGeqOperator();
}
         | T_AND
{
  $$ = new decafAndOperator();
}
         | T_OR
{
  $$ = new decafOrOperator();
}
         ;

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
{
  decafIdList *list = (decafIdList *)$3;
  list->push_back(*$1);
  $$ = list;
  delete $1;
}
     | T_ID
{
  decafIdList *list = new decafIdList();
  list->push_back(*$1);
  $$ = list;
  delete $1;
}
     ;

op_cs_externtype: cs_externtype
{
  decafStmtList *list = (decafStmtList *)$1;
  $$ = list;
}
                | /* empty string */
{
  decafStmtList *list = new decafStmtList();
  $$ = list;
}
                ;

cs_externtype: externtype T_COMMA cs_externtype
{
  decafStmtList *list = (decafStmtList *)$3;
  decafType *type = (decafType *)$1;
  list->push_front(type);
  $$ = list;
}
             | externtype
{
  decafStmtList *list = new decafStmtList();
  decafType *type = (decafType *)$1;
  list->push_front(type);
  $$ = list;
}
             ;

op_cs_idtype: cs_idtype
            | /* empty string */
{
  $$ = new decafStmtList();
}
            ;

cs_idtype: T_ID decaftype T_COMMA cs_idtype
{
  decafSymbol *sym = new decafSymbol(*$1, (decafType *)$2);
  decafStmtList *list = (decafStmtList *)$4;
  list->push_front(sym);
  $$ = list;
  delete $1;
}
         | T_ID decaftype
{
  decafSymbol *sym = new decafSymbol(*$1, (decafType *)$2);
  decafStmtList *list = new decafStmtList();
  list->push_front(sym);
  $$ = list;
  delete $1;
}
         ;

// Basic definitions
decaftype: T_INTTYPE
{
  $$ = new decafIntType();
}
         | T_BOOLTYPE
{
  $$ = new decafBoolType();
}
         ;

methodtype: T_VOID
{
  $$ = new decafVoidType();
}
          | decaftype
          ;

externtype: T_STRINGTYPE
{
  decafStringType *type = new decafStringType();
  $$ = new decafExternType(type);
}
          | methodtype
{
  $$ = new decafExternType((decafType *)$1);
}
          ;

arraytype: T_LSB T_INTCONSTANT T_RSB decaftype {};

boolconstant: T_TRUE
{
  $$ = new decafBoolTrue();
}
            | T_FALSE
{
  $$ = new decafBoolFalse();
}
            ;

constant: T_INTCONSTANT | T_CHARCONSTANT | boolconstant {};

%%

int main() {
  // parse the input and create the abstract syntax tree
  int retval = yyparse();
  return(retval >= 1 ? EXIT_FAILURE : EXIT_SUCCESS);
}

