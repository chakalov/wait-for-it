#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

namespace wait_for_it {

// Base class for all expressions
class BaseExpression {
public:
    virtual ~BaseExpression();
};

// Expression class for numeric literals
class NumberExpression: public BaseExpression {
    double m_val;
public:
    NumberExpression(double val);
};

// Expression class for referencing a variable
class VariableExpression: public BaseExpression {
    std::string m_name;
public:
    VariableExpression(const std::string &name);
};

// Expression class for binary operator
class BinaryExpression: public BaseExpression {
    char m_op;
    BaseExpression *m_lhs, *m_rhs;
public:
    BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs);
};

// Expression class for function calls
class CallExpression: public BaseExpression {
    std::string m_callee;
    std::vector<BaseExpression *> m_args;
public:
    CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args);
};

class FunctionPrototype: public BaseExpression {
    std::string m_name;
    std::vector<std::string> m_args;
public:
    FunctionPrototype(const std::string &name, const std::vector<std::string> &args);
};

class FunctionDefinition: public BaseExpression {
    FunctionPrototype *m_prototype;
    BaseExpression *m_body;
public:
    FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body);
};

}

#endif // AST_H
