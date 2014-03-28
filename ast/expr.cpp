#include "expr.h"

using namespace wait_for_it;

Expr::~Expr()
{

}

llvm::Type *Expr::getLLVMType(llvm::IRBuilder<> &builder, std::string type)
{
    return llvm::StringSwitch<llvm::Type *>(llvm::StringRef(type))
            .Case("void", builder.getVoidTy())
            .Case("char", builder.getInt8Ty())
            .Case("bool", builder.getInt1Ty())
            .Cases("short", "int", builder.getInt32Ty())
            .Cases("long", "double", builder.getDoubleTy())
            .Cases("signed", "unsigned", builder.getInt32Ty());
}

