#ifndef IDENTIFIEREXPR_H
#define IDENTIFIEREXPR_H

#include "variableexpr.h"

namespace wait_for_it {

class IdentifierExpr: public Expr
{
    VariableExpr *m_var;
public:
    IdentifierExpr(VariableExpr * var);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
    llvm::Value *getValue();
};

}

#endif // IDENTIFIEREXPR_H
