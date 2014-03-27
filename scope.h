#ifndef SCOPE_H
#define SCOPE_H

#include "ast.h"
#include <map>
#include <string>

namespace wait_for_it {

class Scope
{
    int m_level;
    std::map<std::string, VariableExpr *> m_variables;
public:
    Scope(int level = 0);
    void addVariable(const std::string &name, VariableExpr *value);
    VariableExpr *getVariable(const std::string &name);

    int getLevel();
};

}

#endif // SCOPE_H
