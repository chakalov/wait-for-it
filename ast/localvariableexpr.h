#ifndef LOCALVARIABLEEXPR_H
#define LOCALVARIABLEEXPR_H

#include "variableexpr.h"
#include <string>

namespace wait_for_it {

class LocalVariableExpr: public VariableExpr
{
public:
    LocalVariableExpr(const std::string &type, const std::string &name);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // LOCALVARIABLEEXPR_H
