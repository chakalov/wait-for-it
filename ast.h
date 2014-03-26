#ifndef AST_H
#define AST_H

#include "ast/expr.h"
#include "ast/doublenumberexpr.h"
#include "ast/int32numberexpr.h"
#include "ast/stringexpr.h"
#include "ast/localvariableexpr.h"
#include "ast/globalvariableexpr.h"
#include "ast/identifierexpr.h"
#include "ast/functionargumentexpr.h"

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/ADT/StringSwitch.h>
#include <string>
#include <vector>

namespace wait_for_it {

// Expression class for binary operator
class BinaryExpression: public Expr {
    char m_op;
    Expr *m_lhs, *m_rhs;
public:
    BinaryExpression(char op, Expr *lhs, Expr *rhs);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module );
};

// Expression class for function calls
class CallExpression: public Expr {
    std::string m_callee;
    std::vector<Expr *> m_args;
public:
    CallExpression(const std::string &callee, const std::vector<Expr *> &args);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class FunctionPrototype: public Expr {
    std::string m_name;
    std::vector<FunctionArgumentExpr *> m_args;
    std::string m_returnType;
public:
    FunctionPrototype(const std::string &name, const std::vector<FunctionArgumentExpr *> &args, std::string returnType);
    void allocateArguments(llvm::IRBuilder<> &builder, llvm::Module &module);
    virtual llvm::Function *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class FunctionDefinition: public Expr {
    FunctionPrototype *m_prototype;
    Expr *m_body;
public:
    FunctionDefinition(FunctionPrototype *prototype, Expr *body);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class PredefinedFunction: public FunctionDefinition {
    llvm::Function *m_predefined;
public:
    PredefinedFunction(llvm::Function *predefined);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);

};

class BlockDefinition: public Expr {
    std::vector<Expr *> m_expressions;
public:
    BlockDefinition(std::vector<Expr *> &expressions);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class IfStatmentExpression: public Expr {
    Expr  *m_expression;
    Expr *m_ifBlock;
    Expr *m_elseBlock;
public:
    IfStatmentExpression(Expr *expression, Expr *ifBlock, Expr *elseBlock);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // AST_H
