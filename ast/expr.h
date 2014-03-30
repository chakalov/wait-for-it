#ifndef EXPR_H
#define EXPR_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include <llvm/ADT/StringSwitch.h>

namespace wait_for_it {

class Expr
{
public:
    virtual ~Expr();
    llvm::Type *getLLVMType(llvm::IRBuilder<> &builder, std::string type);
    llvm::Value *typecast(llvm::IRBuilder<>& builder, llvm::Value *value, llvm::Type *type);
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module) = 0;
};

}

#endif // EXPR_H
