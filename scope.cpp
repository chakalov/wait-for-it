#include "scope.h"

using namespace wait_for_it;

Scope::Scope(int level) : m_level(level) {}

void Scope::addVariable(const std::string &name, VariableExpr *value)
{
    m_variables[name] = value;
}

VariableExpr *Scope::getVariable(const std::string &name)
{
    return m_variables[name];
}

int Scope::getLevel()
{
    return m_level;
}
