#include "scope.h"

using namespace wait_for_it;

Scope::Scope(int level) : m_level(level) {}

void Scope::addVariable(const std::string &name, VariableDeclarationExpression *value)
{
    m_variables[name] = value;
}

VariableDeclarationExpression *Scope::getVariable(const std::string &name)
{
    return m_variables[name];
}

int Scope::getLevel()
{
    return m_level;
}
