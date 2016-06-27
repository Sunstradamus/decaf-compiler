
#include "decafsym-defs.h"
#include <list>
#include <map>
#include <ostream>
#include <iostream>
#include <sstream>

#ifndef YYTOKENTYPE
#include "decafsym.tab.h"
#endif

using namespace std;

// Forward class declarations
class decafAST;
class decafStmtList;
class PackageAST;
class ProgramAST;
class decafType;
class decafIntType;
class decafBoolType;
class decafVoidType;
class decafStringType;
class decafExternType;
class ExternFunctionAST;
class decafSymbol;
class MethodBlockAST;
class MethodAST;
class decafIdList;
class decafStatement;
class decafBlock;
class decafBreakStmt;
class decafContinueStmt;
class symbol;

typedef map<string, symbol*> symbol_table;

enum symType { DECAF_INT, DECAF_BOOL, DECAF_VOID, DECAF_STRING };

class symbol {
public:
	symType type;
	unsigned int regDest;
	unsigned int address;
	bool spilled;
};

/// decafAST - Base class for all abstract syntax tree nodes.
class decafAST {
protected:
	decafAST *parent;
	symbol_table symTable;
public:
  virtual ~decafAST() {}
  virtual string str() { return string(""); }
  void setParent(decafAST *node) {
  	this->parent = node;
  }
  symbol* access_symtbl(string ident) {
  	map<string,symbol*>::iterator it = symTable.find(ident);
  	if (it != symTable.end()) {
  		return it->second;
  	} else {
  		if (parent == NULL) {
  			return NULL;
  		} else {
  			return parent->access_symtbl(ident);
  		}
  	}
  }
};

string getString(decafAST *d) {
	if (d != NULL) {
		return d->str();
	} else {
		return string("None");
	}
}

template <class T>
string commaList(list<T> vec) {
    string s("");
    for (typename list<T>::iterator i = vec.begin(); i != vec.end(); i++) { 
        s = s + (s.empty() ? string("") : string(",")) + (*i)->str(); 
    }   
    if (s.empty()) {
        s = string("None");
    }   
    return s;
}

class decafIdList : public decafAST {
	list<string> ids;
public:
	decafIdList() {}
	~decafIdList() {}
	void push_front(string id) { ids.push_front(id); }
	void push_back(string id) { ids.push_back(id); }
	list<string>::iterator begin() { return ids.begin(); }
	list<string>::iterator end() { return ids.end(); }
	list<string>::reverse_iterator rbegin() { return ids.rbegin(); }
	list<string>::reverse_iterator rend() { return ids.rend(); }
};

/// decafStmtList - List of Decaf statements
class decafStmtList : public decafAST {
	list<decafAST *> stmts;
public:
	decafStmtList() {}
	~decafStmtList() {
		for (list<decafAST *>::iterator i = stmts.begin(); i != stmts.end(); i++) { 
			delete *i;
		}
	}
	int size() { return stmts.size(); }
	void push_front(decafAST *e) { stmts.push_front(e); }
	void push_back(decafAST *e) { stmts.push_back(e); }
	list<decafAST *>::iterator begin() { return stmts.begin(); }
	list<decafAST *>::iterator end() { return stmts.end(); }
	string str() { return commaList<class decafAST *>(stmts); }
};

// decafReturnType
class decafType : public decafAST {
public:
	virtual decafType* clone() const = 0;
	virtual symType getSymType() const = 0;
};

class decafIntType : public decafType {
public:
	decafIntType() {}
	~decafIntType() {}
	decafType* clone() const {
		return new decafIntType();
	}
	symType getSymType() const {
		return DECAF_INT;
	}
	string str() {
		return string("IntType");
	}
};

class decafBoolType : public decafType {
public:
	decafBoolType() {}
	~decafBoolType() {}
	decafType* clone() const {
		return new decafBoolType();
	}
	symType getSymType() const {
		return DECAF_BOOL;
	}
	string str() {
		return string("BoolType");
	}
};

class decafVoidType : public decafType {
public:
	decafVoidType() {}
	~decafVoidType() {}
	decafType* clone() const {
		return new decafVoidType();
	}
	symType getSymType() const {
		return DECAF_VOID;
	}
	string str() {
		return string("VoidType");
	}
};

class decafStringType : public decafType {
public:
	decafStringType() {}
	~decafStringType() {}
	decafType* clone() const {
		return new decafStringType();
	}
	symType getSymType() const {
		return DECAF_STRING;
	}
	string str() {
		return string("StringType");
	}
};

class decafExternType : public decafAST {
	decafType *Type;
public:
	decafExternType(decafType *type) : Type(type) {
		if (Type != NULL) { Type->setParent((decafAST *)this); }
	}
	~decafExternType() {
		if (Type != NULL) { delete Type; }
	}
	string str() {
		return string("VarDef(") + getString(Type) + ")";
	};
};

class ExternFunctionAST : public decafAST {
	string Name;
	decafType *ReturnType;
	decafStmtList *TypeList;
public:
	ExternFunctionAST(string name, decafType *ret_type, decafStmtList *typelist) : Name(name), ReturnType(ret_type), TypeList(typelist) {
		if (ReturnType != NULL) { ReturnType->setParent((decafAST *)this); }
		if (TypeList != NULL) { TypeList->setParent((decafAST *)this); }
	}
	~ExternFunctionAST() {
		if (ReturnType != NULL) { delete ReturnType; }
		if (TypeList != NULL) { delete TypeList; }
	}
	string str() {
		return string("ExternFunction") + "(" + Name + "," + getString(ReturnType) + "," + getString(TypeList) + ")";
	}
};

class decafSymbol : public decafAST {
	string Name;
	decafType *Type;
public:
	decafSymbol(string name, decafType *type) : Name(name), Type(type) {
		if (Type != NULL) { Type->setParent((decafAST *)this); }
	}
	~decafSymbol() {
		if (Type != NULL) { delete Type; }
	}
	string getName() {
		return Name;
	}
	decafType* getType() {
		return Type;
	}
	string str() {
		return string("VarDef") + "(" + Name + "," + getString(Type) + ")";
	}
};

class decafStatement : public decafAST {
};

class MethodBlockAST : public decafAST {
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	MethodBlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {
		if (VarList != NULL) { VarList->setParent((decafAST *)this); }
		if (StmtList != NULL) { StmtList->setParent((decafAST *)this); }

		// Process method block var decls
		for (list<decafAST*>::iterator it = VarList->begin(); it != VarList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = static_cast<decafSymbol*>(obj);
			string identifier = var->getName();
			decafType *type = var->getType();
			symbol *sym = new symbol();

			sym->type = type->getSymType();
			sym->regDest = 0;
			sym->address = 0;
			sym->spilled = 0;

			this->symTable.insert(pair<string,symbol*>(identifier, sym));
		}
		//cout << "MethodBlock SymTable size: " << this->symTable.size() << endl;
	}
	~MethodBlockAST() {
		if (VarList != NULL) { delete VarList; }
		if (StmtList != NULL) { delete StmtList; }
	}
	bool addMethodParamSymbols(decafStmtList *SymbolList) {
		pair<map<string,symbol*>::iterator,bool> ret;

		// Process method params
		for (list<decafAST*>::iterator it = SymbolList->begin(); it != SymbolList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = static_cast<decafSymbol*>(obj);
			string identifier = var->getName();
			decafType *type = var->getType();
			symbol *sym = new symbol();

			sym->type = type->getSymType();
			sym->regDest = 0;
			sym->address = 0;
			sym->spilled = 0;

			ret = this->symTable.insert(pair<string,symbol*>(identifier, sym));
			if (ret.second == false) {
				// Failed to insert method parameter symbols into the symbol table
				// Another symbol with the same identifier exists
				return false;
			}
		}
		//cout << "Method_Decl SymTable size: " << this->symTable.size() << endl;
	}
	string str() {
		return string("MethodBlock") + "(" + getString(VarList) + "," + getString(StmtList) + ")";
	}
};

class BlockAST : public decafStatement {
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	BlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {
		if (VarList != NULL) { VarList->setParent((decafAST *)this); }
		if (StmtList != NULL) { StmtList->setParent((decafAST *)this); }

		// Process method block var decls
		for (list<decafAST*>::iterator it = VarList->begin(); it != VarList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = static_cast<decafSymbol*>(obj);
			string identifier = var->getName();
			decafType *type = var->getType();
			symbol *sym = new symbol();

			sym->type = type->getSymType();
			sym->regDest = 0;
			sym->address = 0;
			sym->spilled = 0;

			this->symTable.insert(pair<string,symbol*>(identifier, sym));
		}
		//cout << "Block SymTable size: " << this->symTable.size() << endl;
	}
	~BlockAST() {
		if (VarList != NULL) { delete VarList; }
		if (StmtList != NULL) { delete StmtList; }
	}
	string str() {
		return string("Block") + "(" + getString(VarList) + "," + getString(StmtList) + ")";
	}
};

class decafBlock : public decafStatement {
protected:
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	decafBlock(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {}
	~decafBlock() {
		//if (VarList != NULL) { delete VarList; }
		//if (StmtList != NULL) { delete StmtList; }
	}
	MethodBlockAST* getMethodBlock() {
		return new MethodBlockAST(VarList, StmtList);
	}
	BlockAST* getBlock() {
		return new BlockAST(VarList, StmtList);
	}
	string str() {
		return string("decafBlock") + "(" + getString(VarList) + "," + getString(StmtList) + ")";
	}
};

class decafOptBlock : public decafStatement {
	BlockAST *Block;
public:
	decafOptBlock(BlockAST *block) : Block(block) {
		if (Block != NULL) { Block->setParent((decafAST *)this); }
	}
	~decafOptBlock() {
		if (Block != NULL) { delete Block; }
	}
	string str() {
		if (Block != NULL) {
			return getString(Block);
		} else {
			return string("None");
		}
	}
};

class decafExpression : public decafStatement {
};

class decafEmptyExpression : public decafExpression {
public:
	decafEmptyExpression() {}
	~decafEmptyExpression() {}
	string str() {
		return string("None");
	}
};

class decafIfStmt : public decafStatement {
	decafExpression *Condition;
	BlockAST *IfBlock;
	decafOptBlock *ElseBlock;
public:
	decafIfStmt(decafExpression *cond, BlockAST *ifblock, decafOptBlock *elseblock) : Condition(cond), IfBlock(ifblock), ElseBlock(elseblock) {
		if (Condition != NULL) { Condition->setParent((decafAST *)this); }
		if (IfBlock != NULL) { IfBlock->setParent((decafAST *)this); }
		if (ElseBlock != NULL) { ElseBlock->setParent((decafAST *)this); }
	}
	~decafIfStmt() {
		if (Condition != NULL) { delete Condition; }
		if (IfBlock != NULL) { delete IfBlock; }
		if (ElseBlock != NULL) { delete ElseBlock; }
	}
	string str() {
		return string("IfStmt") + "(" + getString(Condition) + "," + getString(IfBlock) + "," + getString(ElseBlock) + ")";
	}
};

class decafWhileStmt : public decafStatement {
	decafExpression *Condition;
	BlockAST *WhileBlock;
public:
	decafWhileStmt(decafExpression *cond, BlockAST *blk) : Condition(cond), WhileBlock(blk) {
		if (Condition != NULL) { Condition->setParent((decafAST *)this); }
		if (WhileBlock != NULL) { WhileBlock->setParent((decafAST *)this); }
	}
	~decafWhileStmt() {
		if (Condition != NULL) { delete Condition; }
		if (WhileBlock != NULL) { delete WhileBlock; }
	}
	string str() {
		return string("WhileStmt") + "(" + getString(Condition) + "," + getString(WhileBlock) + ")";
	}
};

class decafForStmt : public decafStatement {
	decafStmtList *PreAssign;
	decafExpression *Condition;
	decafStmtList *LoopAssign;
	BlockAST *ForBlock;
public:
	decafForStmt(decafStmtList *pa, decafExpression *cond, decafStmtList *la, BlockAST *fb) : PreAssign(pa), Condition(cond), LoopAssign(la), ForBlock(fb) {
		if (PreAssign != NULL) { PreAssign->setParent((decafAST *)this); }
		if (Condition != NULL) { Condition->setParent((decafAST *)this); }
		if (LoopAssign != NULL) { LoopAssign->setParent((decafAST *)this); }
		if (ForBlock != NULL) { ForBlock->setParent((decafAST *)this); }
	}
	~decafForStmt() {
		if (PreAssign != NULL) { delete PreAssign; }
		if (Condition != NULL) { delete Condition; }
		if (LoopAssign != NULL) { delete LoopAssign; }
		if (ForBlock != NULL) { delete ForBlock; }
	}
	string str() {
		return string("ForStmt") + "(" + getString(PreAssign) + "," + getString(Condition) + "," + getString(LoopAssign) + "," + getString(ForBlock) + ")";
	}
};

class decafReturnStmt : public decafStatement {
	decafExpression *Value;
public:
	decafReturnStmt(decafExpression *val) : Value(val) {
		if (Value != NULL) { Value->setParent((decafAST *)this); }
	}
	~decafReturnStmt() {
		if (Value != NULL) { delete Value; }
	}
	string str() {
		return string ("ReturnStmt") + "(" + getString(Value) + ")";
	}
};

class decafBreakStmt : public decafStatement {
public:
	decafBreakStmt() {}
	~decafBreakStmt() {}
	string str() {
		return string("BreakStmt");
	}
};

class decafContinueStmt : public decafStatement {
public:
	decafContinueStmt() {}
	~decafContinueStmt() {}
	string str() {
		return string("ContinueStmt");
	}
};

class MethodAST : public decafAST {
	string Name;
	decafType *ReturnType;
	decafStmtList *SymbolList;
	MethodBlockAST *MethodBlock;
public:
	MethodAST(string name, decafType *rt, decafStmtList *sl, MethodBlockAST *mb) : Name(name), ReturnType(rt), SymbolList(sl), MethodBlock(mb) {
		if (ReturnType != NULL) { ReturnType->setParent((decafAST *)this); }
		if (SymbolList != NULL) { SymbolList->setParent((decafAST *)this); }
		if (MethodBlock != NULL) { MethodBlock->setParent((decafAST *)this); }

		MethodBlock->addMethodParamSymbols(SymbolList);
	}
	~MethodAST() {
		if (ReturnType != NULL) { delete ReturnType; }
		if (SymbolList != NULL) { delete SymbolList; }
		if (MethodBlock != NULL) { delete MethodBlock; }
	}
	string str() {
		return string("Method") + "(" + Name + "," + getString(ReturnType) + "," + getString(SymbolList) + "," + getString(MethodBlock) + ")";
	}
};

class MethodCallAST : public decafExpression {
	string Name;
	decafStmtList *ArgsList;
public:
	MethodCallAST(string name, decafStmtList *args) : Name(name), ArgsList(args) {
		if (ArgsList != NULL) { ArgsList->setParent((decafAST *)this); }
	}
	~MethodCallAST() {
		if (ArgsList != NULL) { delete ArgsList; }
	}
	string str() {
		return string("MethodCall") + "(" + Name + "," + getString(ArgsList) + ")";
	}
};

class decafBoolean : public decafAST {
};

class decafBoolTrue : public decafBoolean {
public:
	decafBoolTrue() {}
	~decafBoolTrue() {}
	string str() {
		return string("BoolExpr(True)");
	}
};

class decafBoolFalse : public decafBoolean {
public:
	decafBoolFalse() {}
	~decafBoolFalse() {}
	string str() {
		return string("BoolExpr(False)");
	}
};

class decafUnaryOperator : public decafAST {
};

class decafUnaryMinusOperator : public decafUnaryOperator {
public:
	decafUnaryMinusOperator() {}
	~decafUnaryMinusOperator() {}
	string str() {
		return string("UnaryMinus");
	}
};

class decafNotOperator : public decafUnaryOperator {
public:
	decafNotOperator() {}
	~decafNotOperator() {}
	string str() {
		return string("Not");
	}
};

class decafBinaryOperator : public decafAST {
};

// arithmeticop

class decafPlusOperator : public decafBinaryOperator {
public:
	decafPlusOperator() {}
	~decafPlusOperator() {}
	string str() {
		return string("Plus");
	}
};

class decafMinusOperator : public decafBinaryOperator {
public:
	decafMinusOperator() {}
	~decafMinusOperator() {}
	string str() {
		return string("Minus");
	}
};

class decafMultOperator : public decafBinaryOperator {
public:
	decafMultOperator() {}
	~decafMultOperator() {}
	string str() {
		return string("Mult");
	}
};

class decafDivOperator : public decafBinaryOperator {
public:
	decafDivOperator() {}
	~decafDivOperator() {}
	string str() {
		return string("Div");
	}
};

class decafLeftshiftOperator : public decafBinaryOperator {
public:
	decafLeftshiftOperator() {}
	~decafLeftshiftOperator() {}
	string str() {
		return string("Leftshift");
	}
};

class decafRightshiftOperator : public decafBinaryOperator {
public:
	decafRightshiftOperator() {}
	~decafRightshiftOperator() {}
	string str() {
		return string("Rightshift");
	}
};

class decafModOperator : public decafBinaryOperator {
public:
	decafModOperator() {}
	~decafModOperator() {}
	string str() {
		return string("Mod");
	}
};

// booleanop

class decafLtOperator : public decafBinaryOperator {
public:
	decafLtOperator() {}
	~decafLtOperator() {}
	string str() {
		return string("Lt");
	}
};

class decafGtOperator : public decafBinaryOperator {
public:
	decafGtOperator() {}
	~decafGtOperator() {}
	string str() {
		return string("Gt");
	}
};

class decafLeqOperator : public decafBinaryOperator {
public:
	decafLeqOperator() {}
	~decafLeqOperator() {}
	string str() {
		return string("Leq");
	}
};

class decafGeqOperator : public decafBinaryOperator {
public:
	decafGeqOperator() {}
	~decafGeqOperator() {}
	string str() {
		return string("Geq");
	}
};

class decafEqOperator : public decafBinaryOperator {
public:
	decafEqOperator() {}
	~decafEqOperator() {}
	string str() {
		return string("Eq");
	}
};

class decafNeqOperator : public decafBinaryOperator {
public:
	decafNeqOperator() {}
	~decafNeqOperator() {}
	string str() {
		return string("Neq");
	}
};

class decafAndOperator : public decafBinaryOperator {
public:
	decafAndOperator() {}
	~decafAndOperator() {}
	string str() {
		return string("And");
	}
};

class decafOrOperator : public decafBinaryOperator {
public:
	decafOrOperator() {}
	~decafOrOperator() {}
	string str() {
		return string("Or");
	}
};

class VariableExprAST : public decafExpression {
	string Name;
public:
	VariableExprAST(string name) : Name(name) {}
	~VariableExprAST() {}
	string str() {
		return string("VariableExpr") + "(" + Name + ")";
	}
};

class ArrayLocExprAST : public decafExpression {
	string Name;
	decafExpression *Index;
public:
	ArrayLocExprAST(string name, decafExpression *index) : Name(name), Index(index) {
		if (Index != NULL) { Index->setParent((decafAST *)this); }
	}
	~ArrayLocExprAST() {
		if (Index != NULL) { delete Index; }
	}
	string str() {
		return string("ArrayLocExpr") + "(" + Name + "," + getString(Index) + ")";
	}
};

class UnaryExprAST : public decafExpression {
	decafUnaryOperator *Operator;
	decafExpression *Expression;
public:
	UnaryExprAST(decafUnaryOperator *op, decafExpression *exp) : Operator(op), Expression(exp) {
		if (Operator != NULL) { Operator->setParent((decafAST *)this); }
		if (Expression != NULL) { Expression->setParent((decafAST *)this); }
	}
	~UnaryExprAST() {
		if (Operator != NULL) { delete Operator; }
		if (Expression != NULL) { delete Expression; }
	}
	string str() {
		return string("UnaryExpr") + "(" + getString(Operator) + "," + getString(Expression) + ")";
	}
};

class BinaryExprAST : public decafExpression {
	decafBinaryOperator *Operator;
	decafExpression *Left;
	decafExpression *Right;
public:
	BinaryExprAST(decafBinaryOperator *op, decafExpression *left, decafExpression *right) : Operator(op), Left(left), Right(right) {
		if (Operator != NULL) { Operator->setParent((decafAST *)this); }
		if (Left != NULL) { Left->setParent((decafAST *)this); }
		if (Right != NULL) { Right->setParent((decafAST *)this); }
	}
	~BinaryExprAST() {
		if (Operator != NULL) { delete Operator; }
		if (Left != NULL) { delete Left; }
		if (Right != NULL) { delete Right; }
	}
	string str() {
		return string("BinaryExpr") + "(" + getString(Operator) + "," + getString(Left) + "," + getString(Right) + ")";
	}
};

class NumberExprAST : public decafExpression {
	string Value;
public:
	NumberExprAST(char val) {
		int intval = (int) val;
		stringstream ss;
		ss << intval;
		Value.assign(ss.str());
	}
	NumberExprAST(string val) : Value(val) {}
	~NumberExprAST() {}
	string str() {
		return string("NumberExpr") + "(" + Value + ")";
	}
};

class StringConstantAST : public decafExpression {
	string Value;
public:
	StringConstantAST(string value) : Value(value) {}
	~StringConstantAST() {}
	string str() {
		return string("StringConstant") + "(" + Value + ")";
	}
};

class decafLValue : public decafAST {
	bool array;
	string Name;
	decafExpression *Index;
public:
	decafLValue(string name) : Name(name) {
		array = false;
	}
	decafLValue(string name, decafExpression *index) : Name(name), Index(index) {
		array = true;
	}
	~decafLValue() {}
	bool isArray() {
		/*if (Index != NULL) {
			return false;
		} else {
			return true;
		}*/
		return array;
	}
	string getName() {
		return Name;
	}
	decafExpression* getIndex() {
		return Index;
	}
};

class AssignVarAST : public decafStatement {
	string Name;
	decafExpression *Expression;
public:
	AssignVarAST(string name, decafExpression *value) : Name(name), Expression(value) {
		if (Expression != NULL) { Expression->setParent((decafAST *)this); }
	}
	~AssignVarAST() {
		if (Expression != NULL) { delete Expression; }
	}
	string str() {
		return string("AssignVar") + "(" + Name + "," + getString(Expression) + ")";
	}
};

class AssignArrayLocAST : public decafStatement {
	string Name;
	decafExpression *Index;
	decafExpression *Expression;
public:
	AssignArrayLocAST(string name, decafExpression *index, decafExpression *value) : Name(name), Index(index), Expression(value) {
		if (Index != NULL) { Index->setParent((decafAST *)this); }
		if (Expression != NULL) { Expression->setParent((decafAST *)this); }
	}
	~AssignArrayLocAST() {
		if (Index != NULL) { delete Index; }
		if (Expression != NULL) { delete Expression; }
	}
	string str() {
		return string("AssignArrayLoc") + "(" + Name + "," + getString(Index) + "," + getString(Expression) + ")";
	}
};

class decafArrayType : public decafAST {
	string ArraySize;
	decafType *Type;
public:
	decafArrayType(string arraysize, decafType *type) : ArraySize(arraysize), Type(type) {}
	~decafArrayType() {}
	string getSize() { return ArraySize; }
	decafType* getType() { return Type; }
};

class decafFieldSize : public decafAST {
	bool array;
	string ArraySize;
public:
	decafFieldSize() {
		array = false;
	}
	decafFieldSize(string size) : ArraySize(size) {
		array = true;
	}
	~decafFieldSize() {}
	string str() {
		if (array) {
			return string("Array") + "(" + ArraySize + ")";
		} else {
			return string("Scalar");
		}
	}
};

class FieldDeclAST : public decafAST {
	string Name;
	decafType *Type;
	decafFieldSize *FieldSize;
public:
	FieldDeclAST(string name, decafType *type, decafFieldSize *fs) : Name(name), Type(type), FieldSize(fs) {
		if (Type != NULL) { Type->setParent((decafAST *)this); }
		if (FieldSize != NULL) { FieldSize->setParent((decafAST *)this); }
	}
	~FieldDeclAST() {
		if (Type != NULL) { delete Type; }
		if (FieldSize != NULL) { delete FieldSize; }
	}
	string getName() {
		return Name;
	}
	decafType* getType() {
		return Type;
	}
	string str() {
		return string("FieldDecl") + "(" + Name + "," + getString(Type) + "," + getString(FieldSize) + ")";
	}
};

class AssignGlobalVarAST : public decafAST {
	string Name;
	decafType *Type;
	decafExpression *Expression;
public:
	AssignGlobalVarAST(string name, decafType *type, decafExpression *exp) : Name(name), Type(type), Expression(exp) {
		if (Type != NULL) { Type->setParent((decafAST *)this); }
		if (Expression != NULL) { Expression->setParent((decafAST *)this); }
	}
	~AssignGlobalVarAST() {
		if (Type != NULL) { delete Type; }
		if (Expression != NULL) { delete Expression; }
	}
	string getName() {
		return Name;
	}
	decafType* getType() {
		return Type;
	}
	string str() {
		return string("AssignGlobalVar") + "(" + Name + "," + getString(Type) + "," + getString(Expression) + ")";
	}
};

class PackageAST : public decafAST {
	string Name;
	decafStmtList *FieldDeclList;
	decafStmtList *MethodDeclList;
public:
	PackageAST(string name, decafStmtList *fieldlist, decafStmtList *methodlist) : Name(name), FieldDeclList(fieldlist), MethodDeclList(methodlist) {
		if (FieldDeclList != NULL) { FieldDeclList->setParent((decafAST *)this); }
		if (MethodDeclList != NULL) { MethodDeclList->setParent((decafAST *)this); }

		// Process global vars
		for (list<decafAST*>::iterator it = FieldDeclList->begin(); it != FieldDeclList->end(); it++) {
			decafAST *obj = *it;
			if (FieldDeclAST *var = dynamic_cast<FieldDeclAST*>(obj)) {
				string identifier = var->getName();
				decafType *type = var->getType();
				symbol *sym = new symbol();

				sym->type = type->getSymType();
				sym->regDest = 0;
				sym->address = 0;
				sym->spilled = false;

				this->symTable.insert(pair<string,symbol*>(identifier, sym));
			} else if (AssignGlobalVarAST *var = dynamic_cast<AssignGlobalVarAST*>(obj)) {
				string identifier = var->getName();
				decafType *type = var->getType();
				symbol *sym = new symbol();

				sym->type = type->getSymType();
				sym->regDest = 0;
				sym->address = 0;
				sym->spilled = false;

				this->symTable.insert(pair<string,symbol*>(identifier, sym));
			}
		}
		//cout << "PackageAST SymTable size: " << this->symTable.size() << endl;
	}
	~PackageAST() { 
		if (FieldDeclList != NULL) { delete FieldDeclList; }
		if (MethodDeclList != NULL) { delete MethodDeclList; }
	}
	string str() { 
		return string("Package") + "(" + Name + "," + getString(FieldDeclList) + "," + getString(MethodDeclList) + ")";
	}
};

/// ProgramAST - the decaf program
class ProgramAST : public decafAST {
	decafStmtList *ExternList;
	PackageAST *PackageDef;
public:
	ProgramAST(decafStmtList *externs, PackageAST *c) : ExternList(externs), PackageDef(c) {
		this->parent = NULL;
		if (ExternList != NULL) { ExternList->setParent((decafAST *)this); } 
		if (PackageDef != NULL) { PackageDef->setParent((decafAST *)this); }
	}
	~ProgramAST() { 
		if (ExternList != NULL) { delete ExternList; } 
		if (PackageDef != NULL) { delete PackageDef; }
	}
	string str() { return string("Program") + "(" + getString(ExternList) + "," + getString(PackageDef) + ")"; }
};