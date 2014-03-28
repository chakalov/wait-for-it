#ifndef BLOCKEXPR_H
#define BLOCKEXPR_H

#include "expr.h"

namespace wait_for_it {

class BlockExpr: public Expr {
    std::vector<Expr *> m_expressions;
public:
    BlockExpr(std::vector<Expr *> &expressions);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // BLOCKEXPR_H
