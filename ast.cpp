#include "ast.h"

using namespace wait_for_it;

BaseExpression::~BaseExpression()
{
}


NumberExpression::NumberExpression(double val) : m_val(val)
{

}


VariableExpression::VariableExpression(const std::string &name) : m_name(name)
{

}


BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs)
{

}


CallExpression::CallExpression(const std::string &callee, std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args)
{

}


FunctionPrototype::FunctionPrototype(const std::string &name, std::vector<BaseExpression *> &args) : m_name(name), m_args(args)
{

}


FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body) : m_prototype(prototype), m_body(body)
{

}
