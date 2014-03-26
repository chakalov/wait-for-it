#include "variableexpr.h"

using namespace wait_for_it;

VariableExpr::VariableExpr(const std::string &type, const std::string &name)
    : m_type(type),
      m_name(name)
{

}

llvm::Value *VariableExpr::getValue()
{
    return m_value;
}

std::string VariableExpr::getType()
{
    return m_type;
}

std::string VariableExpr::getName()
{
    return m_name;
}
