#ifndef GLOBALVARIABLEEXPR_H
#define GLOBALVARIABLEEXPR_H

#include "variableexpr.h"
#include <string>

namespace wait_for_it {

class GlobalVariableExpr: public VariableExpr
{
public:
    GlobalVariableExpr(const std::string &type, const std::string &name);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // GLOBALVARIABLEEXPR_H
