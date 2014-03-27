#include "identifierexpr.h"

using namespace wait_for_it;

IdentifierExpr::IdentifierExpr(VariableExpr *var)
    : m_var(var)
{
}

llvm::Value *IdentifierExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    return builder.CreateLoad(m_var->getValue(), m_var->getName());
}

llvm::Value *IdentifierExpr::getValue()
{
    return m_var->getValue();
}

