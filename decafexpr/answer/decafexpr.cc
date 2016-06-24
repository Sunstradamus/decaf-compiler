
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
  virtual llvm::Value *Codegen() = 0;
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

template <class T>
llvm::Value *listCodegen(list<T> vec) {
	llvm::Value *val = NULL;
	for (typename list<T>::iterator i = vec.begin(); i != vec.end(); i++) { 
		llvm::Value *j = (*i)->Codegen();
		if (j != NULL) { val = j; }
	}	
	return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
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
	llvm::Value *Codegen() { 
		return listCodegen<decafAST *>(stmts); 
	}
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
	llvm::Value *Codegen() { 
		llvm::Value *val = NULL;
		TheModule->setModuleIdentifier(llvm::StringRef(Name)); 
		if (NULL != FieldDeclList) {
			val = FieldDeclList->Codegen();
		}
		if (NULL != MethodDeclList) {
			val = MethodDeclList->Codegen();
		} 
		// Q: should we enter the class name into the symbol table?
		return val; 
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
	llvm::Value *Codegen() { 
		llvm::Value *val = NULL;
		if (NULL != ExternList) {
			val = ExternList->Codegen();
		}
		if (NULL != PackageDef) {
			val = PackageDef->Codegen();
		} else {
			throw runtime_error("no package definition in decaf program");
		}
		return val; 
	}
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafBreakStmt : public decafStatement {
public:
	decafBreakStmt() {}
	~decafBreakStmt() {}
	string str() {
		return string("BreakStmt");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafContinueStmt : public decafStatement {
public:
	decafContinueStmt() {}
	~decafContinueStmt() {}
	string str() {
		return string("ContinueStmt");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafBoolFalse : public decafBoolean {
public:
	decafBoolFalse() {}
	~decafBoolFalse() {}
	string str() {
		return string("BoolExpr(False)");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafNotOperator : public decafUnaryOperator {
public:
	decafNotOperator() {}
	~decafNotOperator() {}
	string str() {
		return string("Not");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafMinusOperator : public decafBinaryOperator {
public:
	decafMinusOperator() {}
	~decafMinusOperator() {}
	string str() {
		return string("Minus");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafMultOperator : public decafBinaryOperator {
public:
	decafMultOperator() {}
	~decafMultOperator() {}
	string str() {
		return string("Mult");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafDivOperator : public decafBinaryOperator {
public:
	decafDivOperator() {}
	~decafDivOperator() {}
	string str() {
		return string("Div");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafLeftshiftOperator : public decafBinaryOperator {
public:
	decafLeftshiftOperator() {}
	~decafLeftshiftOperator() {}
	string str() {
		return string("Leftshift");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafRightshiftOperator : public decafBinaryOperator {
public:
	decafRightshiftOperator() {}
	~decafRightshiftOperator() {}
	string str() {
		return string("Rightshift");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafModOperator : public decafBinaryOperator {
public:
	decafModOperator() {}
	~decafModOperator() {}
	string str() {
		return string("Mod");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafGtOperator : public decafBinaryOperator {
public:
	decafGtOperator() {}
	~decafGtOperator() {}
	string str() {
		return string("Gt");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafLeqOperator : public decafBinaryOperator {
public:
	decafLeqOperator() {}
	~decafLeqOperator() {}
	string str() {
		return string("Leq");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafGeqOperator : public decafBinaryOperator {
public:
	decafGeqOperator() {}
	~decafGeqOperator() {}
	string str() {
		return string("Geq");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafEqOperator : public decafBinaryOperator {
public:
	decafEqOperator() {}
	~decafEqOperator() {}
	string str() {
		return string("Eq");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafNeqOperator : public decafBinaryOperator {
public:
	decafNeqOperator() {}
	~decafNeqOperator() {}
	string str() {
		return string("Neq");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafAndOperator : public decafBinaryOperator {
public:
	decafAndOperator() {}
	~decafAndOperator() {}
	string str() {
		return string("And");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class decafOrOperator : public decafBinaryOperator {
public:
	decafOrOperator() {}
	~decafOrOperator() {}
	string str() {
		return string("Or");
	}
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};

class BinaryExprAST : public decafExpression {
	decafBinaryOperator *Op;
	decafExpression *Left;
	decafExpression *Right;
public:
	BinaryExprAST(decafBinaryOperator *op, decafExpression *left, decafExpression *right) : Op(op), Left(left), Right(right) {}
	~BinaryExprAST() {
		if (Op != NULL) { delete Op; }
		if (Left != NULL) { delete Left; }
		if (Right != NULL) { delete Right; }
	}
	string str() {
		return string("BinaryExpr") + "(" + getString(Op) + "," + getString(Left) + "," + getString(Right) + ")";
	}
    llvm::Value *Codegen() {
        llvm::Value *L = Left->Codegen();
        llvm::Value *R = Right->Codegen();
        if (L == 0 || R == 0) return 0;

        if (Op->str().compare("+") == 0)  return Builder.CreateAdd(L, R, "addtmp");
        if (Op->str().compare("-") == 0)  return Builder.CreateSub(L, R, "subtmp");
        if (Op->str().compare("*") == 0)  return Builder.CreateMul(L, R, "multmp");
        if (Op->str().compare("/") == 0)  return Builder.CreateSDiv(L, R, "divtmp");
        if (Op->str().compare("%") == 0)  return Builder.CreateSRem(L, R, "modtmp");
        if (Op->str().compare(">>") == 0) return Builder.CreateAShr(L, R, "rshtmp");
        if (Op->str().compare("<<") == 0) return Builder.CreateShl(L, R, "lshtmp");
        if (Op->str().compare("&&") == 0) return Builder.CreateAnd(L, R, "andtmp");
        if (Op->str().compare("||") == 0) return Builder.CreateOr(L, R, "ortmp");
        if (Op->str().compare("<") == 0)  return Builder.CreateICmpSLT(L, R, "lttmp");
        if (Op->str().compare(">") == 0)  return Builder.CreateICmpSGT(L, R, "gttmp");
        if (Op->str().compare("<=") == 0) return Builder.CreateICmpSLE(L, R, "letmp");
        if (Op->str().compare(">=") == 0) return Builder.CreateICmpSGE(L, R, "getmp");
        if (Op->str().compare("==") == 0) return Builder.CreateICmpEQ(L, R, "eqtmp");
        if (Op->str().compare("!=") == 0) return Builder.CreateICmpNE(L, R, "neqtmp");
        return NULL;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *val = NULL;
        return val;
    }
};