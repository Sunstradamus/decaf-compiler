%{
#include <iostream>
#include <ostream>
#include <string>
#include <cstdlib>
#include "decafexpr-defs.h"

int yylex(void);
int yyerror(char *); 

// print AST?
bool printAST = true;

#include "decafexpr.cc"

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
%type <ast> cs_id statements statement assign methodcall boolconstant unaryop op_cs_methodarg cs_methodarg methodarg decafexpr
%type <ast> expr1 expr2 expr3 expr4 expr5 binaryop1 binaryop2 binaryop3 binaryop4 binaryop5
%type <ast> constant lvalue op_returnexpr op_decafexpr op_else cs_assign field_decl arraytype

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
  $$ = new PackageAST(*$2, (decafStmtList *)$4, (decafStmtList *)$5);
  delete $2;
};

field_decl: T_VAR T_ID decaftype T_SEMICOLON field_decl
{
  decafStmtList *slist = (decafStmtList *)$5;
  FieldDeclAST *field = new FieldDeclAST(*$2, (decafType *)$3, new decafFieldSize());
  slist->push_front(field);
  $$ = slist;
  delete $2;
}
          | T_VAR T_ID T_COMMA cs_id decaftype T_SEMICOLON field_decl
{
  decafStmtList *slist = (decafStmtList *)$7;
  decafIdList *id_list = (decafIdList *)$4;
  id_list->push_front(*$2);
  decafType *type = (decafType *)$5;
  for (list<string>::iterator i = id_list->begin(); i != id_list->end(); i++) {
    slist->push_back(new FieldDeclAST(*i, type->clone(), new decafFieldSize()));
  }
  $$ = slist;
  delete type;
  delete id_list;
  delete $2;
}
          | T_VAR cs_id arraytype T_SEMICOLON field_decl
{
  decafStmtList *slist = (decafStmtList *)$5;
  decafIdList *id_list = (decafIdList *)$2;
  decafArrayType *at = (decafArrayType *)$3;
  decafType *type = at->getType();
  for (list<string>::iterator i = id_list->begin(); i != id_list->end(); i++) {
    slist->push_back(new FieldDeclAST(*i, type->clone(), new decafFieldSize(at->getSize())));
  }
  $$ = slist;
  delete type;
  delete at;
  delete id_list;
}
          | T_VAR T_ID decaftype T_ASSIGN constant T_SEMICOLON field_decl
{
  decafStmtList *slist = (decafStmtList *)$7;
  AssignGlobalVarAST *field = new AssignGlobalVarAST(*$2, (decafType *)$3, (decafExpression *)$5);
  slist->push_front(field);
  $$ = slist;
  delete $2;
}
          | /* empty string */
{
  decafStmtList *slist = new decafStmtList();
  $$ = slist;
}
          ;

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
  $$ = new decafBlock((decafStmtList *)$2, (decafStmtList *)$3);
}

var_decls: T_VAR cs_id decaftype T_SEMICOLON var_decls
{
  decafStmtList *slist = (decafStmtList *)$5;
  decafType *type = (decafType *)$3;
  decafIdList *idlist = (decafIdList *)$2;
  // Reverse iterator cuz anoop's files do it
  //for (list<string>::iterator i = idlist->begin(); i != idlist->end(); i++) {
  for (list<string>::reverse_iterator i = idlist->rbegin(); i != idlist->rend(); i++) {
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
{
  $$ = (MethodCallAST *)$1;
}
         | T_IF T_LPAREN decafexpr T_RPAREN decafblock op_else
{
  decafBlock *block = (decafBlock *)$5;
  BlockAST *ifblk = block->getBlock();
  decafOptBlock *opt = (decafOptBlock *)$6;
  $$ = new decafIfStmt((decafExpression *)$3, ifblk, opt);
  delete block;
}
         | T_WHILE T_LPAREN decafexpr T_RPAREN decafblock
{
  decafBlock *block = (decafBlock *)$5;
  BlockAST *blockAST = block->getBlock();
  $$ = new decafWhileStmt((decafExpression *)$3, blockAST);
  delete block;
}
         | T_FOR T_LPAREN cs_assign T_SEMICOLON decafexpr T_SEMICOLON cs_assign T_RPAREN decafblock
{
  decafBlock *block = (decafBlock *)$9;
  BlockAST *blockAST = block->getBlock();
  $$ = new decafForStmt((decafStmtList *)$3, (decafExpression *)$5, (decafStmtList *)$7, blockAST);
  delete block;
}
         | T_RETURN op_returnexpr T_SEMICOLON
{
  $$ = new decafReturnStmt((decafExpression *)$2);
}
         | T_BREAK T_SEMICOLON
{
  $$ = new decafBreakStmt();
}
         | T_CONTINUE T_SEMICOLON
{
  $$ = new decafContinueStmt();
}
         ;

decafexpr: decafexpr binaryop1 expr1
{
  $$ = new BinaryExprAST((decafBinaryOperator *)$2, (decafExpression *)$1, (decafExpression *)$3);
}
         | expr1
         ;

expr1: expr1 binaryop2 expr2
{
  $$ = new BinaryExprAST((decafBinaryOperator *)$2, (decafExpression *)$1, (decafExpression *)$3);
}
         | expr2
         ;


expr2: expr2 binaryop3 expr3
{
  $$ = new BinaryExprAST((decafBinaryOperator *)$2, (decafExpression *)$1, (decafExpression *)$3);
}
         | expr3
         ;

expr3: expr3 binaryop4 expr4
{
  $$ = new BinaryExprAST((decafBinaryOperator *)$2, (decafExpression *)$1, (decafExpression *)$3);
}
         | expr4
         ;

expr4: expr4 binaryop5 expr5
{
  $$ = new BinaryExprAST((decafBinaryOperator *)$2, (decafExpression *)$1, (decafExpression *)$3);
}
         | expr5
         ;

expr5: T_ID
{
  $$ = new VariableExprAST(*$1);
  delete $1;
}
    | methodcall
{
  $$ = (MethodCallAST *)$1;
}
    | constant
    | T_LPAREN decafexpr T_RPAREN
{
  $$ = (decafExpression *)$2;
}
    | unaryop expr5
{
  $$ = new UnaryExprAST((decafUnaryOperator *)$1, (decafExpression *)$2);
}
    | T_ID T_LSB decafexpr T_RSB
{
  $$ = new ArrayLocExprAST(*$1, (decafExpression *)$3);
  delete $1;
}
    ;

unaryop: T_NOT
{
  $$ = new decafNotOperator();
}
       | T_MINUS
{
  $$ = new decafUnaryMinusOperator();
}
       ;

binaryop1: T_OR
{
  $$ = new decafOrOperator();
}

binaryop2: T_AND
{
  $$ = new decafAndOperator();
}

binaryop3:  T_EQ
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

binaryop4: T_PLUS
{
  $$ = new decafPlusOperator();
}
         | T_MINUS
{
  $$ = new decafMinusOperator();
}

binaryop5: T_MULT
{
  $$ = new decafMultOperator();
}
         | T_DIV
{
  $$ = new decafDivOperator();
}
         | T_MOD
{
  $$ = new decafModOperator();
}
         | T_LEFTSHIFT
{
  $$ = new decafLeftshiftOperator();
}
         | T_RIGHTSHIFT
{
  $$ = new decafRightshiftOperator();
}
            ;

op_returnexpr: T_LPAREN op_decafexpr T_RPAREN
{
  $$ = (decafExpression *)$2;
}
             | /* empty string */
{
  $$ = new decafEmptyExpression();
}
             ;

op_decafexpr: decafexpr
            | /* empty string */
{
  $$ = new decafEmptyExpression();
}
            ;

op_else: T_ELSE decafblock
{
  decafBlock *block = (decafBlock *)$2;
  BlockAST *blockAST = block->getBlock();
  decafOptBlock *opt = new decafOptBlock(blockAST);
  $$ = opt;
  delete block;
}
       | /* empty string */
{
  $$ = new decafOptBlock(NULL);
}
       ;

cs_assign: assign T_COMMA cs_assign
{
  decafStmtList *list = (decafStmtList *)$3;
  list->push_front((decafStatement *)$1);
  $$ = list;
}
         | assign
{
  decafStmtList *list = new decafStmtList();
  list->push_back((decafStatement *)$1);
  $$ = list;
}
         ;

assign: lvalue T_ASSIGN decafexpr
{
  decafLValue *lv = (decafLValue *)$1;
  if ( lv->isArray() ) {
    $$ = new AssignArrayLocAST(lv->getName(), lv->getIndex(), (decafExpression *)$3);
  } else {
    $$ = new AssignVarAST(lv->getName(), (decafExpression *)$3);
  }
  delete lv;
}

lvalue: T_ID
{
  $$ = new decafLValue(*$1);
  delete $1;
}
      | T_ID T_LSB decafexpr T_RSB
{
  $$ = new decafLValue(*$1, (decafExpression *)$3);
  delete $1;
}
      ;

methodcall: T_ID T_LPAREN op_cs_methodarg T_RPAREN
{
  MethodCallAST *call = new MethodCallAST(*$1, (decafStmtList *)$3);
  $$ = call;
  delete $1;
}

op_cs_methodarg: cs_methodarg
{
  $$ = (decafStmtList *)$1;
}
               | /* empty string */
{
  $$ = new decafStmtList();
}
               ;

cs_methodarg: methodarg T_COMMA cs_methodarg
{
  decafStmtList *list = (decafStmtList *)$3;
  list->push_front((decafExpression *)$1);
  $$ = list;
}
            | methodarg
{
  decafStmtList *list = new decafStmtList();
  list->push_back((decafExpression *)$1);
  $$ = list;
}
            ;

methodarg: decafexpr
         | T_STRINGCONSTANT
{
  $$ = new StringConstantAST(*$1);
}
         ;

cs_id: T_ID T_COMMA cs_id
{
  decafIdList *list = (decafIdList *)$3;
  list->push_front(*$1);
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
          | decaftype
{
  $$ = new decafExternType((decafType *)$1);
}
          ;

arraytype: T_LSB T_INTCONSTANT T_RSB decaftype
{
  $$ = new decafArrayType(*$2, (decafType *)$4);
}

boolconstant: T_TRUE
{
  $$ = new decafBoolTrue();
}
            | T_FALSE
{
  $$ = new decafBoolFalse();
}
            ;

constant: T_INTCONSTANT
{
  $$ = new NumberExprAST(*$1);
}
        | T_CHARCONSTANT
{
  string character = *$1;
  if (character.at(1) == '\\') {
    switch (character.at(2)) {
      case 'a':
        $$ = new NumberExprAST('\a');
        break;
      case 'b':
        $$ = new NumberExprAST('\b');
        break;
      case 't':
        $$ = new NumberExprAST('\t');
        break;
      case 'n':
        $$ = new NumberExprAST('\n');
        break;
      case 'v':
        $$ = new NumberExprAST('\v');
        break;
      case 'f':
        $$ = new NumberExprAST('\f');
        break;
      case 'r':
        $$ = new NumberExprAST('\r');
        break;
      case '\\':
        $$ = new NumberExprAST('\\');
        break;
      case '\'':
        $$ = new NumberExprAST('\'');
        break;
      case '\"':
        $$ = new NumberExprAST('\"');
        break;
      default:
        $$ = new NumberExprAST(character.at(2));
        break;
    }
  } else {
    $$ = new NumberExprAST(character.at(1));
  }
  delete $1;
}
        | boolconstant
{}
        ;

%%

int main() {
  // parse the input and create the abstract syntax tree
  int retval = yyparse();
  return(retval >= 1 ? EXIT_FAILURE : EXIT_SUCCESS);
}

