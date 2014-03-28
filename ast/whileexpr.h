#ifndef WHILEEXPR_H
#define WHILEEXPR_H

#include "expr.h"
#include "blockexpr.h"

namespace wait_for_it {

class WhileExpr: public Expr {
    Expr  *m_expression;
    BlockExpr *m_block;
public:
    WhileExpr(Expr *expression, BlockExpr *Block);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // WHILEEXPR_H
