#include "localvariableexpr.h"

using namespace wait_for_it;

LocalVariableExpr::LocalVariableExpr(const std::string &type, const std::string &name)
    : VariableExpr(type, name)
{

}

llvm::Value *LocalVariableExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type *type = getLLVMType(builder, m_type);

    return m_value = builder.CreateAlloca(type, 0, m_name.c_str());
}
