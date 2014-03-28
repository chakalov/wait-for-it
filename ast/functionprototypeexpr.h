#ifndef FUNCTIONPROTOTYPEEXPR_H
#define FUNCTIONPROTOTYPEEXPR_H

#include "expr.h"
#include "functionargumentexpr.h"

namespace wait_for_it {

class FunctionPrototypeExpr: public Expr {
    std::string m_name;
    std::vector<FunctionArgumentExpr *> m_args;
    std::string m_returnType;
public:
    FunctionPrototypeExpr(const std::string &name, const std::vector<FunctionArgumentExpr *> &args, std::string returnType);
    void allocateArguments(llvm::IRBuilder<> &builder, llvm::Module &module);
    virtual llvm::Function *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};
}

#endif // FUNCTIONPROTOTYPEEXPR_H
