#ifndef INFINITEEXPR_H
#define INFINITEEXPR_H

#include "expr.h"
#include "blockexpr.h"

namespace wait_for_it {

class InfiniteExpr: public Expr {
    BlockExpr *m_block;
public:
    InfiniteExpr(BlockExpr *Block);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // INFINITEEXPR_H
