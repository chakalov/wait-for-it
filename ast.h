#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

namespace wait_for_it {

// Base class for all expressions
class BaseExpression {
public:
    virtual ~BaseExpression();
    virtual void emitCode() = 0;
};

// Expression class for numeric literals
class NumberExpression: public BaseExpression {
    double m_val;
public:
    NumberExpression(double val);
    double getValue();
    virtual void emitCode();
};

// TODO: REFACTOR -- add symbol table
// Expression class for identifiers
class IdentifierExpression: public BaseExpression {
    std::string m_name;
public:
    IdentifierExpression(std::string name);
    std::string getValue();
    virtual void emitCode();
};

// Expression class for referencing a variable
class VariableExpression: public BaseExpression {
    std::string m_type;
    std::string m_name;
public:
    VariableExpression(const std::string &type, const std::string &name);
    virtual void emitCode();
};

// Expression class for binary operator
class BinaryExpression: public BaseExpression {
    char m_op;
    BaseExpression *m_lhs, *m_rhs;
public:
    BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs);
    virtual void emitCode();
};

// Expression class for function calls
class CallExpression: public BaseExpression {
    std::string m_callee;
    std::vector<BaseExpression *> m_args;
public:
    CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args);
    virtual void emitCode();
};

class FunctionPrototype: public BaseExpression {
    std::string m_name;
    std::vector<BaseExpression *> m_args;
public:
    FunctionPrototype(const std::string &name, const std::vector<BaseExpression *> &args);
    virtual void emitCode();
};

class FunctionDefinition: public BaseExpression {
    FunctionPrototype *m_prototype;
    BaseExpression *m_body;
public:
    FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body);
    virtual void emitCode();
};

class BlockDefinition: public BaseExpression {
    std::vector<BaseExpression *> m_expressions;
public:
    BlockDefinition(std::vector<BaseExpression *> &expressions);
    virtual void emitCode();
};

class IfStatmentExpression: public BaseExpression {
    BaseExpression  *m_expression;
    BlockDefinition *m_ifBlock;
    BlockDefinition *m_elseBlock;
public:
    IfStatmentExpression(BaseExpression  &expression, BlockDefinition &ifBlock, BlockDefinition &elseBlock);
    virtual void emitCode();
};

}

#endif // AST_H
