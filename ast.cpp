#include "ast.h"

#include <cstdio>

using namespace wait_for_it;

BaseExpression::~BaseExpression()
{
}

NumberExpression::NumberExpression(double val) : m_val(val)
{
}

llvm::Value *NumberExpression::emitCode()
{

}

VariableExpression::VariableExpression(const std::string &type, const std::string &name) : m_type(type), m_name(name)
{
}

llvm::Value *VariableExpression::emitCode()
{
    printf("Var[%s %s]\n", m_type.c_str(), m_name.c_str());
}

BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs)
{
}

llvm::Value *BinaryExpression::emitCode()
{
    printf("Expr[ ");
    m_lhs->emitCode();
    printf(" (%c) ", m_op);
    m_rhs->emitCode();
    printf(" ]\n");
}

CallExpression::CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args)
{
}

llvm::Value *CallExpression::emitCode()
{

}

FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<BaseExpression *> &args) : m_name(name), m_args(args)
{
}

llvm::Value *FunctionPrototype::emitCode()
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

llvm::Value *FunctionDefinition::emitCode()
{
    printf("Func[");
    m_prototype->emitCode();
    m_body->emitCode();
    printf("]\n");
}

BlockDefinition::BlockDefinition(std::vector<BaseExpression *> &expressions) : m_expressions(expressions)
{
}

llvm::Value *BlockDefinition::emitCode()
{
    printf("BeginBlock\n");
    for (std::vector<BaseExpression *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
        (*expression)->emitCode();
    }
    printf("EndBlock\n");
}


IfStatmentExpression::IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock) :
    m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock)
{

}

llvm::Value *IfStatmentExpression::emitCode()
{
    printf("IFF (");
    m_expression->emitCode();
    printf("\t{{");
    m_ifBlock->emitCode();
    printf("}}\n");
}


IdentifierExpression::IdentifierExpression(std::string name) : m_name(name)
{

}

std::string IdentifierExpression::getValue()
{
    return m_name;
}

llvm::Value *IdentifierExpression::emitCode()
{
    printf("$Ident[%s]$", m_name.c_str());
}
