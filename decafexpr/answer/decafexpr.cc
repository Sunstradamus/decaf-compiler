
#include "decafexpr-defs.h"
#include <list>
#include <ostream>
#include <iostream>
#include <sstream>

#ifndef YYTOKENTYPE
#include "decafexpr.tab.h"
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

/// decafAST - Base class for all abstract syntax tree nodes.
class decafAST {
public:
  virtual ~decafAST() {}
  virtual string str() { return string(""); }
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
	string str() { return commaList<class decafAST *>(stmts); }
};

class PackageAST : public decafAST {
	string Name;
	decafStmtList *FieldDeclList;
	decafStmtList *MethodDeclList;
public:
	PackageAST(string name, decafStmtList *fieldlist, decafStmtList *methodlist) 
		: Name(name), FieldDeclList(fieldlist), MethodDeclList(methodlist) {}
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
	ProgramAST(decafStmtList *externs, PackageAST *c) : ExternList(externs), PackageDef(c) {}
	~ProgramAST() { 
		if (ExternList != NULL) { delete ExternList; } 
		if (PackageDef != NULL) { delete PackageDef; }
	}
	string str() { return string("Program") + "(" + getString(ExternList) + "," + getString(PackageDef) + ")"; }
};

// decafReturnType
class decafType : public decafAST {
public:
	virtual decafType* clone() const = 0;
};

class decafIntType : public decafType {
public:
	decafIntType() {}
	~decafIntType() {}
	decafType* clone() const {
		return new decafIntType();
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
	string str() {
		return string("StringType");
	}
};

class decafExternType : public decafAST {
	decafType *Type;
public:
	decafExternType(decafType *type) : Type(type) {}
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
	ExternFunctionAST(string name, decafType *ret_type, decafStmtList *typelist) : Name(name), ReturnType(ret_type), TypeList(typelist) {}
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
	decafSymbol(string name, decafType *type) : Name(name), Type(type) {}
	~decafSymbol() {
		if (Type != NULL) { delete Type; }
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
	MethodBlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {}
	~MethodBlockAST() {
		if (VarList != NULL) { delete VarList; }
		if (StmtList != NULL) { delete StmtList; }
	}
	string str() {
		return string("MethodBlock") + "(" + getString(VarList) + "," + getString(StmtList) + ")";
	}
};

class BlockAST : public decafStatement {
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	BlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {}
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
	decafOptBlock(BlockAST *block) : Block(block) {}
	~decafOptBlock() {}
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
	decafIfStmt(decafExpression *cond, BlockAST *ifblock, decafOptBlock *elseblock) : Condition(cond), IfBlock(ifblock), ElseBlock(elseblock) {}
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
	decafWhileStmt(decafExpression *cond, BlockAST *blk) : Condition(cond), WhileBlock(blk) {}
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
	decafForStmt(decafStmtList *pa, decafExpression *cond, decafStmtList *la, BlockAST *fb) : PreAssign(pa), Condition(cond), LoopAssign(la), ForBlock(fb) {}
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
	decafReturnStmt(decafExpression *val) : Value(val) {}
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
	MethodAST(string name, decafType *rt, decafStmtList *sl, MethodBlockAST *mb) : Name(name), ReturnType(rt), SymbolList(sl), MethodBlock(mb) {}
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
	MethodCallAST(string name, decafStmtList *args) : Name(name), ArgsList(args) {}
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
	ArrayLocExprAST(string name, decafExpression *index) : Name(name), Index(index) {}
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
	UnaryExprAST(decafUnaryOperator *op, decafExpression *exp) : Operator(op), Expression(exp) {}
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
	BinaryExprAST(decafBinaryOperator *op, decafExpression *left, decafExpression *right) : Operator(op), Left(left), Right(right) {}
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
	AssignVarAST(string name, decafExpression *value) : Name(name), Expression(value) {}
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
	AssignArrayLocAST(string name, decafExpression *index, decafExpression *value) : Name(name), Index(index), Expression(value) {}
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
	FieldDeclAST(string name, decafType *type, decafFieldSize *fs) : Name(name), Type(type), FieldSize(fs) {}
	~FieldDeclAST() {
		if (Type != NULL) { delete Type; }
		if (FieldSize != NULL) { delete FieldSize; }
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
	AssignGlobalVarAST(string name, decafType *type, decafExpression *exp) : Name(name), Type(type), Expression(exp) {}
	~AssignGlobalVarAST() {
		if (Type != NULL) { delete Type; }
		if (Expression != NULL) { delete Expression; }
	}
	string str() {
		return string("AssignGlobalVar") + "(" + Name + "," + getString(Type) + "," + getString(Expression) + ")";
	}
};
