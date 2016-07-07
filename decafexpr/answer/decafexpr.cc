
#include "decafexpr-defs.h"
#include <list>
#include <map>
#include <ostream>
#include <iostream>
#include <sstream>
#include <assert.h>

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
class FieldDeclAST;
class AssignGlobalVarAST;

typedef map<string, llvm::Value*> symbol_table;

struct array_info {
    llvm::ArrayType *arraytype;
    llvm::Type *elementtype;
};
typedef map<string, array_info> arrayinfo_table;

arrayinfo_table arraytbl;


/// decafAST - Base class for all abstract syntax tree nodes.
class decafAST {
protected:
	decafAST *parent;
	symbol_table symTable;
    bool isblock = false;
    bool isloop = false;
public:
    virtual ~decafAST() {}
    virtual string str() { return string(""); }
    void setParent(decafAST *node) {
        this->parent = node;
    }
    decafAST *find_loop() {
        if (isloop) {
            return this;
        }
        return this->parent->find_loop();
    }
    void insert_symtbl(string ident, llvm::Value *alloca) {
        if (isblock) {
            // cerr << "inserted " << ident << " into symbol table." << endl;
            symTable.insert(pair<string,llvm::Value*>(ident,alloca));
        }
        else {
            // cerr << "Can't store " << ident << ". Looking in parent..." << endl;
            parent->insert_symtbl(ident, alloca);
        }
    }
    llvm::Value* access_symtbl(string ident) {
        map<string,llvm::Value*>::iterator it = symTable.find(ident);
        if (it != symTable.end()) {
            return it->second;
        } else {
            if (parent == NULL) {
                // cerr << "Not found at all." << endl;
                return NULL;
            } else {
                // cerr << "Not found in me. Looking in parent..." << endl;
                return parent->access_symtbl(ident);
            }
        }
    }
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
	list<decafAST *>::iterator begin() { return stmts.begin(); }
	list<decafAST *>::iterator end() { return stmts.end(); }
	list<decafAST *>::reverse_iterator rbegin() { return stmts.rbegin(); }
	list<decafAST *>::reverse_iterator rend() { return stmts.rend(); }
	string str() { return commaList<class decafAST *>(stmts); }
	llvm::Value *Codegen() {
        for (auto it=stmts.begin(); it != stmts.end(); it++) {
            if (*it != NULL) { (*it)->setParent((decafAST *)this); }
        }
		return listCodegen<decafAST *>(stmts); 
	}
};

// decafReturnType
class decafType : public decafAST {
public:
	virtual decafType* clone() const = 0;
    virtual llvm::Type *LLVMType() = 0;
    llvm::Value *Codegen() {return NULL;} // We don't use this for types
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
    llvm::Type *LLVMType() {
        return Builder.getInt32Ty();
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
    llvm::Type *LLVMType() {
        return Builder.getInt1Ty();
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
    llvm::Type *LLVMType() {
        return Builder.getVoidTy();
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
    llvm::Type *LLVMType() {
        return Builder.getInt8PtrTy();
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
	}
    llvm::Value *Codegen() { return NULL; }
    llvm::Type *LLVMType() {
        return Type->LLVMType();
    }
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

    llvm::Function *Codegen() {
        vector<llvm::Type *> Params;
        for (auto i = TypeList->begin(); i != TypeList->end(); i++) {
            decafExternType *dtype = dynamic_cast<decafExternType *>(*i);
            llvm::Type *type = dtype->LLVMType();
            assert(type != NULL);
            Params.push_back(type);
        }
        llvm::FunctionType *FT = llvm::FunctionType::get((llvm::Type *)ReturnType->LLVMType(), Params, false);
        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule);
        return F;
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
	string str() {
		return string("VarDef") + "(" + Name + "," + getString(Type) + ")";
	}
    string get_Name() {
        return Name;
    }
    decafType *get_Type() {
        return Type;
    }
    llvm::Value *Codegen() {
        //cerr << "Creating variable " << Name << "..." << endl;
        llvm::Value *alloca = Builder.CreateAlloca(Type->LLVMType(), 0, Name.c_str());
        assert(alloca != NULL);
        //cerr << "Creation of " << Name << " complete" << endl;
        insert_symtbl(Name,alloca);
        return NULL;
    }
};

class decafStatement : public decafAST {
};

class MethodBlockAST : public decafAST {
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	MethodBlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {
        isblock = true;
		if (VarList != NULL) { VarList->setParent((decafAST *)this); }
		if (StmtList != NULL) { StmtList->setParent((decafAST *)this); }
	}
	~MethodBlockAST() {
		if (VarList != NULL) { delete VarList; }
		if (StmtList != NULL) { delete StmtList; }
	}
	bool addMethodParamSymbols(decafStmtList *SymbolList) {

		// Process method params
		for (list<decafAST*>::iterator it = SymbolList->begin(); it != SymbolList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = dynamic_cast<decafSymbol*>(obj);
            var->Codegen();
		}
		return true;
	}    
	string str() {
		return string("MethodBlock") + "(" + getString(VarList) + "," + getString(StmtList) + ")";
	}
    llvm::Value *Codegen() {
		llvm::Value *val = NULL;
		if (NULL != VarList) {
			val = VarList->Codegen();
		}
		if (NULL != StmtList) {
			val = StmtList->Codegen();
		} 
		return val;
    }
};

class BlockAST : public decafStatement {
	decafStmtList *VarList;
	decafStmtList *StmtList;
public:
	BlockAST(decafStmtList *varlist, decafStmtList *stmtlist) : VarList(varlist), StmtList(stmtlist) {
        isblock = true;
		if (VarList != NULL) { VarList->setParent((decafAST *)this); }
		if (StmtList != NULL) { StmtList->setParent((decafAST *)this); }

		// Process method block var decls
		for (list<decafAST*>::iterator it = VarList->begin(); it != VarList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = static_cast<decafSymbol*>(obj);
			// this->symTable.insert(pair<string,symbol*>(identifier, sym));
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
    llvm::Value *Codegen() {
		llvm::Value *val = NULL;
		if (NULL != VarList) {
			val = VarList->Codegen();
		}
		if (NULL != StmtList) {
			val = StmtList->Codegen();
		} 
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
	decafOptBlock(BlockAST *block) : Block(block) {
        isblock = true;
		if (Block != NULL) { Block->setParent((decafAST *)this); }
	}
	~decafOptBlock() {}
	string str() {
		if (Block != NULL) {
			return getString(Block);
		} else {
			return string("None");
		}
	}
    llvm::Value *Codegen() {
        return Block->Codegen();
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
    bool hasElse;
public:
	decafIfStmt(decafExpression *cond, BlockAST *ifblock, decafOptBlock *elseblock) : Condition(cond), IfBlock(ifblock), ElseBlock(elseblock) {
        //hasElse = 
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
    llvm::Value *Codegen() {
        llvm::Value *cond = Condition->Codegen();
        if (cond == NULL) return NULL;

        // We want to insert a new block after the current one
        llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

        // Define the blocks
        llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", TheFunction);
        llvm::BasicBlock *ElseBB;
        if (ElseBlock != NULL) {
            ElseBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else");
        }
        llvm::BasicBlock *EndIfBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "endif");

        // Add the conditional branch
        Builder.CreateCondBr(cond, ThenBB, (ElseBlock != NULL ? ElseBB : EndIfBB) );

        // Start adding to the 'then' block
        Builder.SetInsertPoint(ThenBB);
        IfBlock->Codegen();

        // Add the branch so we skip the else block
        Builder.CreateBr(EndIfBB);
        
        // Update ThenBB
        ThenBB = Builder.GetInsertBlock();

        // Start adding to the 'else' block.
        // If it's empty we will just jump to the end of the if statement
        if (ElseBlock != NULL) {
            TheFunction->getBasicBlockList().push_back(ElseBB);
            Builder.SetInsertPoint(ElseBB);
            ElseBlock->Codegen();

            // Branch to the end of the if statement
            Builder.CreateBr(EndIfBB);
        
            // Update ElseBB
            ElseBB = Builder.GetInsertBlock();
        }

        // Setup the next code block
        TheFunction->getBasicBlockList().push_back(EndIfBB);
        Builder.SetInsertPoint(EndIfBB);

        return EndIfBB;
    }
};

class decafLoop : public decafStatement {
protected:
    llvm::BasicBlock *Start, *End;
public:
    virtual llvm::BasicBlock *getStart() { return Start; }
    virtual llvm::BasicBlock *getEnd() { return End; }
};

class decafWhileStmt : public decafLoop {
	decafExpression *Condition;
	BlockAST *WhileBlock;
public:
	decafWhileStmt(decafExpression *cond, BlockAST *blk) : Condition(cond), WhileBlock(blk) {
        isloop = true;
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
    llvm::Value *Codegen() {
        // We want to insert a new block after the current one
        llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

        // Define the blocks
        llvm::BasicBlock *WhileCondBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "whilecond", TheFunction);
        llvm::BasicBlock *WhileBodyBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "whilebody");
        llvm::BasicBlock *EndWhileBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "endwhile");

        // Used for break and continue statements
        Start = WhileCondBB;
        End = EndWhileBB;

        // Create the condition block
        Builder.CreateBr(WhileCondBB);
        Builder.SetInsertPoint(WhileCondBB);
        llvm::Value *cond = Condition->Codegen();
        if (cond == NULL) return NULL;
        Builder.CreateCondBr(cond, WhileBodyBB, EndWhileBB);
        // Update WhileCondBB
        WhileCondBB = Builder.GetInsertBlock();
        
        // Add the body of the while loop and a jump to the beginning
        TheFunction->getBasicBlockList().push_back(WhileBodyBB);
        Builder.SetInsertPoint(WhileBodyBB);
        WhileBlock->Codegen();
        Builder.CreateBr(WhileCondBB);
        
        // Update WhileBB
        WhileBodyBB = Builder.GetInsertBlock();

        // Setup the next code block
        TheFunction->getBasicBlockList().push_back(EndWhileBB);
        Builder.SetInsertPoint(EndWhileBB);

        return EndWhileBB;
    }
};

class decafForStmt : public decafLoop {
	decafStmtList *PreAssign;
	decafExpression *Condition;
	decafStmtList *LoopAssign;
	BlockAST *ForBlock;
public:
	decafForStmt(decafStmtList *pa, decafExpression *cond, decafStmtList *la, BlockAST *fb) : PreAssign(pa), Condition(cond), LoopAssign(la), ForBlock(fb) {
        isloop = true;
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
    llvm::Value *Codegen() {

        // We want to insert a new block after the current one
        llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();

        // Define the blocks
        llvm::BasicBlock *ForCondBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "forcond", TheFunction);
        llvm::BasicBlock *ForBodyBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "forbody");
        llvm::BasicBlock *EndForBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "endfor");

        // Used for break and continue statements
        Start = ForCondBB;
        End = EndForBB;

        // Execute the pre-assign code first
        PreAssign->Codegen();

        // Create the condition block
        Builder.CreateBr(ForCondBB);
        Builder.SetInsertPoint(ForCondBB);
        llvm::Value *cond = Condition->Codegen();
        if (cond == NULL) return NULL;
        Builder.CreateCondBr(cond, ForBodyBB, EndForBB);
        // Update ForCondBB
        ForCondBB = Builder.GetInsertBlock();
        
        // Add the body of the for loop
        TheFunction->getBasicBlockList().push_back(ForBodyBB);
        Builder.SetInsertPoint(ForBodyBB);
        ForBlock->Codegen();
        
        // Update the loop assignment and jump to the beginning
        LoopAssign->Codegen();
        Builder.CreateBr(ForCondBB);
        
        // Update ForBB
        ForBodyBB = Builder.GetInsertBlock();

        // Setup the next code block
        TheFunction->getBasicBlockList().push_back(EndForBB);
        Builder.SetInsertPoint(EndForBB);

        return EndForBB;
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
    llvm::Value *Codegen() {
        llvm::Value *val = Value->Codegen();
        return Builder.CreateRet(val);
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
        decafAST *p = this->parent->find_loop();
        assert( p != NULL && "Error: break statement outside of a loop");
        assert(dynamic_cast<decafLoop *>(p) != NULL);
        llvm::Value *val = Builder.CreateBr(dynamic_cast<decafLoop *>(p)->getEnd());
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
        decafAST *p = this->parent->find_loop();
        assert( p != NULL && "Error: continue statement outside of a loop");
        assert(dynamic_cast<decafLoop *>(p) != NULL);
        llvm::Value *val = Builder.CreateBr(dynamic_cast<decafLoop *>(p)->getStart());
        return val;
    }
};

class MethodAST : public decafAST {
	string Name;
	decafType *ReturnType;
	decafStmtList *SymbolList;
	MethodBlockAST *MethodBlock;
    llvm::Function *TheFunction;
public:
	MethodAST(string name, decafType *rt, decafStmtList *sl, MethodBlockAST *mb) : Name(name), ReturnType(rt), SymbolList(sl), MethodBlock(mb) {
		if (ReturnType != NULL) { ReturnType->setParent((decafAST *)this); }
		if (MethodBlock != NULL) { MethodBlock->setParent((decafAST *)this); }
		if (SymbolList != NULL) {
            for (auto it = SymbolList->begin(); it != SymbolList->end(); it++) {
                (*it)->setParent((decafAST *)this);
            }
        }
	}
	~MethodAST() {
		if (ReturnType != NULL) { delete ReturnType; }
		if (SymbolList != NULL) { delete SymbolList; }
		if (MethodBlock != NULL) { delete MethodBlock; }
	}
	string str() {
		return string("Method") + "(" + Name + "," + getString(ReturnType) + "," + getString(SymbolList) + "," + getString(MethodBlock) + ")";
	}
    void CreateMethodHeader() {
        // Define the number and type of parameters
        vector<llvm::Type *> Params;
		for (auto it = SymbolList->begin(); it != SymbolList->end(); it++) {
			decafAST *obj = *it;
			decafSymbol *var = dynamic_cast<decafSymbol*>(obj);
            llvm::Type *type = var->get_Type()->LLVMType();
            assert(type != NULL);
            Params.push_back(type);
        }
        // Create the function declaration
        llvm::FunctionType *FT = llvm::FunctionType::get((llvm::Type *)ReturnType->LLVMType(), Params, false);
        TheFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule);
        if (TheFunction == 0) {
            throw runtime_error("empty function block"); 
        }
    }
    llvm::Function *Codegen() {
        // Create a new basic block which contains a sequence of LLVM instructions
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", TheFunction);
        // All subsequent calls to IRBuilder will place instructions in this location
        Builder.SetInsertPoint(BB);
        // Add the parameters to the symbol table in MethodBlock
        auto syml = SymbolList->begin();
        for (auto &Arg : TheFunction->args()) {
            string name = dynamic_cast<decafSymbol*>(*syml++)->get_Name();
            //Arg.setName(name);
            llvm::AllocaInst *alloca = Builder.CreateAlloca(Arg.getType(), 0, name.c_str());
            assert(alloca != NULL);
            Builder.CreateStore(&Arg, alloca);
            MethodBlock->insert_symtbl(name.c_str(), alloca);
        }
        
        // This was used for accessing the parameters directly
        //  however we cannot assign values to them
        //
        // auto syml = SymbolList->begin();
        // for (auto &Arg : TheFunction->args()) {
        //     string name = dynamic_cast<decafSymbol*>(*syml)->get_Name();
        //     syml++;
        //     Arg.setName(name);
        //     MethodBlock->insert_symtbl(name.c_str(), &Arg);
        // }
        
        // Generate the remaining code
        auto retValue = MethodBlock->Codegen();
        if (ReturnType->str().compare("VoidType") == 0) {
            Builder.CreateRetVoid();
        }
        else if (ReturnType->str().compare("BoolType") == 0) {
            Builder.CreateRet(Builder.getInt1(0));
        }
        else { // Int type
            Builder.CreateRet(Builder.getInt32(0));
        }
        verifyFunction(*TheFunction);
        return TheFunction;
    }
};

class MethodCallAST : public decafExpression {
	string Name;
	decafStmtList *ArgsList;
public:
	MethodCallAST(string name, decafStmtList *args) : Name(name), ArgsList(args) {
		if (ArgsList != NULL) {
            for (auto i = ArgsList->begin(); i != ArgsList->end(); i++) {
                (*i)->setParent((decafAST *)this);
            }
        }
    }
    ~MethodCallAST() {
        if (ArgsList != NULL) { delete ArgsList; }
	}
	string str() {
		return string("MethodCall") + "(" + Name + "," + getString(ArgsList) + ")";
	}
 
    llvm::Value *Codegen() {
        llvm::Function *method = TheModule->getFunction(Name);
        assert(method != NULL);
        std::vector<llvm::Value *> args;
        for (auto i = ArgsList->begin(); i != ArgsList->end(); i++) {
            args.push_back((*i)->Codegen());
            if (!args.back()) {
                return NULL;
            }
        }

        // Convert any bool type parameters into integer type if that is required
        int count = 0;
        for (auto i = method->arg_begin(); i != method->arg_end(); i++) {
            if (i->getType()->isIntegerTy(32) && args[count]->getType()->isIntegerTy(1)) {
                args[count] = Builder.CreateIntCast(args[count], Builder.getInt32Ty(), false);
            }
            count++;
        }
        // Don't make an assignment if the return type is void
        if (method->getReturnType()->isVoidTy()) {
            return Builder.CreateCall(method, args);
        }
        return Builder.CreateCall(method, args, "calltmp");
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
        return Builder.getInt1(1);
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
        return Builder.getInt1(0);
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
        //cerr << "Looking for " << Name << " in the symbol tables..."  << endl;
        llvm::Value *alloca = access_symtbl(Name);
        if (llvm::isa<llvm::Argument>(alloca)) return alloca;
        assert(alloca != NULL);
        //cerr << "Found " << Name << "." << endl;
        return Builder.CreateLoad(alloca, "ld_" + Name);
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
    llvm::Value *Codegen() {
        // Get the memory location
        llvm::Value *alloca = access_symtbl(Name);
        assert(alloca);
        // Get the additional information
        map<string, array_info>::iterator it = arraytbl.find(Name);
        array_info info;
        assert(it != arraytbl.end());
        info = it->second;
        // Look up the value at the given index
        llvm::Value *ArrayLoc = Builder.CreateStructGEP(info.arraytype, alloca, 0, "arrayloc");
        llvm::Value *Ind = Index->Codegen();
        llvm::Value *ArrayIndex = Builder.CreateGEP(info.elementtype, ArrayLoc, Ind, "arrayindex");
        llvm::Value *val = Builder.CreateLoad(ArrayIndex, "ld_" + Name);
        return val;
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
    llvm::Value *Codegen() {
        llvm::Value *Expr = Expression->Codegen();
        if (Expr == 0) return 0;

        if (Operator->str().compare("UnaryMinus") == 0)  return Builder.CreateNeg(Expr, "negtmp");
        if (Operator->str().compare("Not") == 0)  return Builder.CreateNot(Expr, "nottmp");
        return NULL;
    }
};

class BinaryExprAST : public decafExpression {
	decafBinaryOperator *Op;
	decafExpression *Left;
	decafExpression *Right;
public:
	BinaryExprAST(decafBinaryOperator *op, decafExpression *left, decafExpression *right) : Op(op), Left(left), Right(right) {
		if (Op != NULL) { Op->setParent((decafAST *)this); }
		if (Left != NULL) { Left->setParent((decafAST *)this); }
		if (Right != NULL) { Right->setParent((decafAST *)this); }
	}
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
        if (L == 0 ) return 0;

        if (Op->str().compare("And") == 0) {
            // We want to insert a new block after the current one
            llvm::BasicBlock *StartingBB = Builder.GetInsertBlock();
            llvm::Function *TheFunction = StartingBB->getParent();

            // Define the blocks
            llvm::BasicBlock *RightBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "and_right", TheFunction);
            llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "and_end");
            // add a jump if false
            Builder.CreateCondBr(L, RightBB, EndBB);
            // Start adding to the 'right' block
            Builder.SetInsertPoint(RightBB);
            llvm::Value *R = Right->Codegen();
            llvm::Value *andV = Builder.CreateAnd(L, R, "andtmp");
            Builder.CreateBr(EndBB);
            // Update RightBB
            RightBB = Builder.GetInsertBlock();
            // Add the end of the and statement
            TheFunction->getBasicBlockList().push_back(EndBB);
            Builder.SetInsertPoint(EndBB);
            // Phi node makes sure the correct boolean value is returned
            llvm::PHINode *PN = Builder.CreatePHI(Builder.getInt1Ty(), 2, "andphi");
            PN->addIncoming(L, StartingBB);
            PN->addIncoming(andV, RightBB);
            return PN;
        }
        else if (Op->str().compare("Or") == 0) {
            // We want to insert a new block after the current one
            llvm::BasicBlock *StartingBB = Builder.GetInsertBlock();
            llvm::Function *TheFunction = StartingBB->getParent();

            // Define the blocks
            llvm::BasicBlock *RightBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "or_right", TheFunction);
            llvm::BasicBlock *EndBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "or_end");
            // add a jump if true
            Builder.CreateCondBr(L, EndBB, RightBB);
            // Start adding to the 'right' block
            Builder.SetInsertPoint(RightBB);
            llvm::Value *R = Right->Codegen();
            llvm::Value *orV = Builder.CreateOr(L, R, "ortmp");
            Builder.CreateBr(EndBB);
            // Update RightBB
            RightBB = Builder.GetInsertBlock();
            // Add the end of the or statement
            TheFunction->getBasicBlockList().push_back(EndBB);
            Builder.SetInsertPoint(EndBB);
            // Phi node makes sure the correct boolean value is returned
            llvm::PHINode *PN = Builder.CreatePHI(Builder.getInt1Ty(), 2, "orphi");
            PN->addIncoming(L, StartingBB);
            PN->addIncoming(orV, RightBB);
            return PN;
        }
        llvm::Value *R = Right->Codegen();
        if (R == 0) return 0;
        
        if (Op->str().compare("Plus") == 0)  return Builder.CreateAdd(L, R, "addtmp");
        if (Op->str().compare("Minus") == 0)  return Builder.CreateSub(L, R, "subtmp");
        if (Op->str().compare("Mult") == 0)  return Builder.CreateMul(L, R, "multmp");
        if (Op->str().compare("Div") == 0)  return Builder.CreateSDiv(L, R, "divtmp");
        if (Op->str().compare("Mod") == 0)  return Builder.CreateSRem(L, R, "modtmp");
        if (Op->str().compare("Rightshift") == 0) return Builder.CreateAShr(L, R, "rshtmp");
        if (Op->str().compare("Leftshift") == 0) return Builder.CreateShl(L, R, "lshtmp");
        if (Op->str().compare("Lt") == 0)  return Builder.CreateICmpSLT(L, R, "lttmp");
        if (Op->str().compare("Gt") == 0)  return Builder.CreateICmpSGT(L, R, "gttmp");
        if (Op->str().compare("Leq") == 0) return Builder.CreateICmpSLE(L, R, "letmp");
        if (Op->str().compare("Geq") == 0) return Builder.CreateICmpSGE(L, R, "getmp");
        if (Op->str().compare("Eq") == 0) return Builder.CreateICmpEQ(L, R, "eqtmp");
        if (Op->str().compare("Neq") == 0) return Builder.CreateICmpNE(L, R, "neqtmp");
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
        return Builder.getInt32(stoi(Value,0,0));
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
        string v = string("");
        // Ignore the quotes in the string
        for (int i = 1; i < Value.length()-1; i++) {
            // Convert escape characters
            if (Value.at(i) == '\\') {
                switch (Value.at(i+1)) {
                    case 'a':
                        v.append(1u, '\a');
                        i++;
                        break;
                    case 'b':
                        v.append(1u, '\b');
                        i++;
                        break;
                    case 't':
                        v.append(1u, '\t');
                        i++;
                        break;
                    case 'n':
                        v.append(1u, '\n');
                        i++;
                        break;
                    case 'v':
                        v.append(1u, '\v');
                        i++;
                        break;
                    case 'f':
                        v.append(1u, '\f');
                        i++;
                        break;
                    case 'r':
                        v.append(1u, '\r');
                        i++;
                        break;
                    case '\\':
                        v.append(1u, '\\');
                        i++;
                        break;
                    case '\'':
                        v.append(1u, '\'');
                        i++;
                        break;
                    case '\"':
                        v.append(1u, '\"');
                        i++;
                    default:
                        break;
                }
            }
            else {
                v.append(1u, Value.at(i));
            }
        }
        //llvm::Value *val = llvm::ConstantDataArray::getString(TheModule->getContext(), v.c_str(), true);
        return Builder.CreateGlobalStringPtr(v.c_str(), "cstrtmp");
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
	AssignVarAST(string name, decafExpression *value) : Name(name), Expression(value) {
		if (Expression != NULL) { Expression->setParent((decafAST *)this); }
	}
	~AssignVarAST() {
		if (Expression != NULL) { delete Expression; }
	}
	string str() {
		return string("AssignVar") + "(" + Name + "," + getString(Expression) + ")";
	}
    llvm::Value *Codegen() {
        //cerr << "Looking in symbol tables for " << Name << " to assign a value to..." << endl;
        llvm::Value *alloca = access_symtbl(Name);
        assert(alloca != NULL);
        //cerr << "Found " << Name << " in the symbol table. Assigning a value now..." << endl;
        auto val = Builder.CreateStore(Expression->Codegen(), alloca);
        //cerr << "Finished assigning value." << endl;
        return val;
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
    llvm::Value *Codegen() {
        // Get the memory location
        llvm::Value *alloca = access_symtbl(Name);
        assert(alloca);
        // Get the additional information
        map<string, array_info>::iterator it = arraytbl.find(Name);
        array_info info;
        assert(it != arraytbl.end());
        info = it->second;
        // Store the value at the given index
        llvm::Value *ArrayLoc = Builder.CreateStructGEP(info.arraytype, alloca, 0, "arrayloc");
        llvm::Value *Ind = Index->Codegen();
        llvm::Value *ArrayIndex = Builder.CreateGEP(info.elementtype, ArrayLoc, Ind, "arrayindex");
        llvm::Value *val = Builder.CreateStore(Expression->Codegen(), ArrayIndex);
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
    bool isArray() { return array; }
    int getSize() { return stoi(ArraySize); }
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
	FieldDeclAST(string name, decafType *type, decafFieldSize *fs) : Name(name), Type(type), FieldSize(fs) {
		if (Type != NULL) { Type->setParent((decafAST *)this); }
		if (FieldSize != NULL) { FieldSize->setParent((decafAST *)this); }
	}
	~FieldDeclAST() {
		if (Type != NULL) { delete Type; }
		if (FieldSize != NULL) { delete FieldSize; }
	}
	string str() {
		return string("FieldDecl") + "(" + Name + "," + getString(Type) + "," + getString(FieldSize) + ")";
	}
    llvm::Value *Codegen() {
        if (FieldSize->isArray()) {
            // set the array size and type
            llvm::ArrayType *atype = llvm::ArrayType::get(Type->LLVMType(), FieldSize->getSize());
            // initialize all the values of the array to zero
            llvm::Constant *zeroInit = llvm::Constant::getNullValue(atype);
            // declare the array as a global variable
            llvm::GlobalVariable *GV = new llvm::GlobalVariable(*TheModule, atype, false, llvm::GlobalValue::ExternalLinkage, zeroInit, Name);
            assert(dynamic_cast<llvm::Value *>(GV));
            insert_symtbl(Name,dynamic_cast<llvm::Value *>(GV));
            // Add info about the array to the global array info table
            array_info info;
            info.arraytype = atype;
            info.elementtype = Type->LLVMType();
            arraytbl.insert(pair<string, array_info>(Name, info));
            return GV;
        }
        else {
            llvm::Constant *zeroInit = llvm::Constant::getNullValue(Type->LLVMType());
            llvm::GlobalVariable *GV = new llvm::GlobalVariable(*TheModule, Type->LLVMType(), false, llvm::GlobalValue::ExternalLinkage, zeroInit, Name);
            assert(dynamic_cast<llvm::Value *>(GV));
            insert_symtbl(Name,dynamic_cast<llvm::Value *>(GV));
            return GV;
            
        }
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
	string str() {
		return string("AssignGlobalVar") + "(" + Name + "," + getString(Type) + "," + getString(Expression) + ")";
	}
    llvm::Value *Codegen() {
        // Assume that the expression is a constant
        llvm::Constant *expr = dynamic_cast<llvm::Constant *>(Expression->Codegen());
        assert(expr);
        llvm::GlobalVariable *GV = new llvm::GlobalVariable(*TheModule, Type->LLVMType(), false, llvm::GlobalValue::ExternalLinkage, expr, Name);
        llvm:: Value *alloca = dynamic_cast<llvm::Value *>(GV);
        assert(alloca);
        insert_symtbl(Name, alloca);
        return GV;
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
				// this->symTable.insert(pair<string,symbol*>(identifier, sym));
			} else if (AssignGlobalVarAST *var = dynamic_cast<AssignGlobalVarAST*>(obj)) {
				// this->symTable.insert(pair<string,symbol*>(identifier, sym));
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
	llvm::Value *Codegen() { 
		llvm::Value *val = NULL;
		TheModule->setModuleIdentifier(llvm::StringRef(Name)); 
		if (NULL != FieldDeclList) {
			val = FieldDeclList->Codegen();
		}
		if (NULL != MethodDeclList) {
            for (auto it : (*MethodDeclList)) {
                dynamic_cast<MethodAST *>(it)->CreateMethodHeader();
            }
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
	ProgramAST(decafStmtList *externs, PackageAST *c) : ExternList(externs), PackageDef(c) {
        isblock = true;
		this->parent = NULL;
		if (ExternList != NULL) { ExternList->setParent((decafAST *)this); } 
		if (PackageDef != NULL) { PackageDef->setParent((decafAST *)this); }
	}
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

