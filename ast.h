#ifndef AST_H
#define AST_H

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

// Base class for all expressions
class BaseExpression {
public:
    virtual ~BaseExpression();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module) = 0;
};

// Expression class for numeric literals
class DoubleNumberExpression: public BaseExpression {
    double m_val;
public:
    DoubleNumberExpression(double val);
    double getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class IntegerNumberExpression: public BaseExpression {
    long m_val;
public:
    IntegerNumberExpression(long val);
    long getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

// Expression class for numeric literals
class StringExpression: public BaseExpression {
    std::string m_val;
public:
    StringExpression(std::string val);
    std::string getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

// Expression class for referencing a variable
class VariableDeclarationExpression: public BaseExpression {
protected:
    std::string m_type;
    std::string m_name;
    llvm::Value *m_value;
public:
    VariableDeclarationExpression(const std::string &type, const std::string &name);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
    llvm::Value *getValue();
    std::string getName();
};

// Expression class for identifiers
class IdentifierExpression: public BaseExpression {
    bool hasAlloca;
    VariableDeclarationExpression *m_var;
public:
    IdentifierExpression(VariableDeclarationExpression * var);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
    llvm::Value *getValue();
};

class FunctionArgument: public VariableDeclarationExpression {
public:
    FunctionArgument(std::string type, std::string name);
    std::string getType();
    std::string getName();
    void setValue(llvm::Value *val);
};

class GlobalVariableExpression: public VariableDeclarationExpression {
public:
    GlobalVariableExpression(const std::string &type, const std::string &name);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

// Expression class for binary operator
class BinaryExpression: public BaseExpression {
    char m_op;
    BaseExpression *m_lhs, *m_rhs;
public:
    BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module );
};

// Expression class for function calls
class CallExpression: public BaseExpression {
    std::string m_callee;
    std::vector<BaseExpression *> m_args;
public:
    CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class FunctionPrototype: public BaseExpression {
    std::string m_name;
    std::vector<FunctionArgument *> m_args;
    std::string m_returnType;
public:
    FunctionPrototype(const std::string &name, const std::vector<FunctionArgument *> &args, std::string returnType);
    virtual llvm::Function *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class FunctionDefinition: public BaseExpression {
    FunctionPrototype *m_prototype;
    BaseExpression *m_body;
public:
    FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class PredefinedFunction: public FunctionDefinition {
    llvm::Function *m_predefined;
public:
    PredefinedFunction(llvm::Function *predefined);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);

};

class BlockDefinition: public BaseExpression {
    std::vector<BaseExpression *> m_expressions;
public:
    BlockDefinition(std::vector<BaseExpression *> &expressions);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

class IfStatmentExpression: public BaseExpression {
    BaseExpression  *m_expression;
    BaseExpression *m_ifBlock;
    BaseExpression *m_elseBlock;
public:
    IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // AST_H
