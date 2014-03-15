#ifndef AST_H
#define AST_H

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <string>
#include <vector>

namespace wait_for_it {

// Base class for all expressions
class BaseExpression {
public:
    virtual ~BaseExpression();
    virtual llvm::Value *emitCode() = 0;
};

// Expression class for numeric literals
class NumberExpression: public BaseExpression {
    double m_val;
public:
    NumberExpression(double val);
    double getValue();
    virtual llvm::Value *emitCode();
};

// TODO: REFACTOR -- add symbol table
// Expression class for identifiers
class IdentifierExpression: public BaseExpression {
    std::string m_name;
public:
    IdentifierExpression(std::string name);
    std::string getValue();
    virtual llvm::Value *emitCode();
};

// Expression class for referencing a variable
class VariableExpression: public BaseExpression {
    std::string m_type;
    std::string m_name;
public:
    VariableExpression(const std::string &type, const std::string &name);
    virtual llvm::Value *emitCode();
};

// Expression class for binary operator
class BinaryExpression: public BaseExpression {
    char m_op;
    BaseExpression *m_lhs, *m_rhs;
public:
    BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs);
    virtual llvm::Value *emitCode();
};

// Expression class for function calls
class CallExpression: public BaseExpression {
    std::string m_callee;
    std::vector<BaseExpression *> m_args;
public:
    CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args);
    virtual llvm::Value *emitCode();
};

class FunctionPrototype: public BaseExpression {
    std::string m_name;
    std::vector<BaseExpression *> m_args;
public:
    FunctionPrototype(const std::string &name, const std::vector<BaseExpression *> &args);
    virtual llvm::Value *emitCode();
};

class FunctionDefinition: public BaseExpression {
    FunctionPrototype *m_prototype;
    BaseExpression *m_body;
public:
    FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body);
    virtual llvm::Value *emitCode();
};

class BlockDefinition: public BaseExpression {
    std::vector<BaseExpression *> m_expressions;
public:
    BlockDefinition(std::vector<BaseExpression *> &expressions);
    virtual llvm::Value *emitCode();
};

class IfStatmentExpression: public BaseExpression {
    BaseExpression  *m_expression;
    BaseExpression *m_ifBlock;
    BaseExpression *m_elseBlock;
public:
    IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock);
    virtual llvm::Value *emitCode();
};

}

#endif // AST_H
