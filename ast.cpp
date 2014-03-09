#include "ast.h"

#include <cstdio>

using namespace wait_for_it;

BaseExpression::~BaseExpression()
{
}

NumberExpression::NumberExpression(double val) : m_val(val)
{
}

void NumberExpression::emitCode()
{

}

VariableExpression::VariableExpression(const std::string &type, const std::string &name) : m_type(type), m_name(name)
{
}

void VariableExpression::emitCode()
{
    printf("Var[%s %s]\n", m_type.c_str(), m_name.c_str());
}

BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs)
{
}

void BinaryExpression::emitCode()
{
    printf("Expr[\n");
    m_lhs->emitCode();
    printf("(%c)\n", m_op);
    m_rhs->emitCode();
    printf("]\n");
}

CallExpression::CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args)
{
}

void CallExpression::emitCode()
{

}

FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<BaseExpression *> &args) : m_name(name), m_args(args)
{
}

void FunctionPrototype::emitCode()
{
    printf("Proto[%s (", m_name.c_str());
    for (std::vector<BaseExpression *>::iterator arg = m_args.begin() ; arg != m_args.end(); ++arg) {
        (*arg)->emitCode();
    }
    printf(")]\n");
}

FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body) : m_prototype(prototype), m_body(body)
{
}

void FunctionDefinition::emitCode()
{
    printf("Func[");
    m_prototype->emitCode();
    m_body->emitCode();
    printf("]\n");
}

BlockDefinition::BlockDefinition(std::vector<BaseExpression *> &expressions) : m_expressions(expressions)
{
}

void BlockDefinition::emitCode()
{
    printf("BeginBlock\n");
    for (std::vector<BaseExpression *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
        (*expression)->emitCode();
    }
    printf("EndBlock\n");
}


//IfStatmentExpression::IfStatmentExpression(BaseExpression &expression, BlockDefinition &ifBlock, BlockDefinition &elseBlock) :
//    m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock)
//{

//}

//void IfStatmentExpression::emitCode()
//{
//    printf("IFF");
//}


IdentifierExpression::IdentifierExpression(std::string name) : m_name(name)
{

}

std::string IdentifierExpression::getValue()
{
    return m_name;
}

void IdentifierExpression::emitCode()
{
    printf("Ident[%s] // Please REFACTOR ME\n", m_name.c_str());
}
