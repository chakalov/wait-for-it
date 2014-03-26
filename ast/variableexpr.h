#ifndef VARIABLEEXPR_H
#define VARIABLEEXPR_H

#include "expr.h"

namespace wait_for_it {

class VariableExpr: public Expr
{
protected:
    std::string m_type;
    std::string m_name;
    llvm::Value *m_value;
    VariableExpr(const std::string &type, const std::string &name);
public:
    llvm::Value *getValue();
    std::string getType();
    std::string getName();
};

}

#endif // VARIABLEEXPR_H
