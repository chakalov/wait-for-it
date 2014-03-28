#ifndef FUNCTIONEXPR_H
#define FUNCTIONEXPR_H

#include "expr.h"
#include "functionprototypeexpr.h"
#include "blockexpr.h"

namespace wait_for_it {

class FunctionExpr: public Expr {
    FunctionPrototypeExpr *m_prototype;
    BlockExpr *m_body;
public:
    FunctionExpr(FunctionPrototypeExpr *prototype, BlockExpr *body);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // FUNCTIONEXPR_H
