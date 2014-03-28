#ifndef CALLEXPR_H
#define CALLEXPR_H

#include "expr.h"

namespace wait_for_it {

class CallExpr: public Expr {
    std::string m_callee;
    std::vector<Expr *> m_args;
public:
    CallExpr(const std::string &callee, const std::vector<Expr *> &args);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // CALLEXPR_H
