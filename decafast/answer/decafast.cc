
#include "decafast-defs.h"
#include <list>
#include <ostream>
#include <iostream>
#include <sstream>

#ifndef YYTOKENTYPE
#include "decafast.tab.h"
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

class decafBoolean : public decafAST {
};

class decafBoolTrue : public decafBoolean {
public:
	decafBoolTrue() {}
	~decafBoolTrue() {}
	string str() {
		return string("True");
	}
};

class decafBoolFalse : public decafBoolean {
public:
	decafBoolFalse() {}
	~decafBoolFalse() {}
	string str() {
		return string("False");
	}
};