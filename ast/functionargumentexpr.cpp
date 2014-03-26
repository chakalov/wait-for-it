#include "functionargumentexpr.h"

using namespace wait_for_it;

FunctionArgumentExpr::FunctionArgumentExpr(std::string type, std::string name)
    : LocalVariableExpr(type, name)
{
}

void FunctionArgumentExpr::setParameter(llvm::Value *val)
{
    m_param = val;
}

llvm::Value *FunctionArgumentExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    LocalVariableExpr::emitCode(builder, module);

    builder.CreateStore(m_param, m_value);
    return m_value;
}
