#ifndef BINARYEXPR_H
#define BINARYEXPR_H

#include "expr.h"
#include "identifierexpr.h"

namespace wait_for_it {

class BinaryExpr: public Expr {
    char m_op;
    Expr *m_lhs, *m_rhs;
public:
    BinaryExpr(char op, Expr *lhs, Expr *rhs);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module );
};

}

#endif // BINARYEXPR_H
