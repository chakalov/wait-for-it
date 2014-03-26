#ifndef FUNCTIONARGUMENTEXPR_H
#define FUNCTIONARGUMENTEXPR_H

#include "localvariableexpr.h"

namespace wait_for_it {

class FunctionArgumentExpr: public LocalVariableExpr
{
    llvm::Value *m_param;
public:
    FunctionArgumentExpr(std::string type, std::string name);
    void setParameter(llvm::Value *val);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // FUNCTIONARGUMENTEXPR_H
