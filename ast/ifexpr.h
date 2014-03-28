#ifndef IFEXPR_H
#define IFEXPR_H

#include "expr.h"
#include "blockexpr.h"

namespace wait_for_it {

class IfExpr: public Expr {
    Expr  *m_expression;
    BlockExpr *m_ifBlock;
    BlockExpr *m_elseBlock;
public:
    IfExpr(Expr *expression, BlockExpr *ifBlock, BlockExpr *elseBlock);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // IFEXPR_H
