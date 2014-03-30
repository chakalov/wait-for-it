#include "expr.h"

using namespace wait_for_it;

Expr::~Expr()
{

}

llvm::Value *Expr::typecast(llvm::IRBuilder<> &builder, llvm::Value *value, llvm::Type *type)
{
    llvm::Type *currType = value->getType();
    if (currType == builder.getDoubleTy()) {
        if (type == builder.getInt32Ty()) {
            return builder.CreateFPToSI(value, type);
        } else {
            return builder.CreateFPToUI(value, type);
        }
    } else {
        if (type == builder.getDoubleTy()) {
            return builder.CreateSIToFP(value, type);
        }
    }
    return value;
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

llvm::Value *test() {
    return NULL;
}

