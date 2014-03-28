#include "int32numberexpr.h"

using namespace wait_for_it;

Int32NumberExpr::Int32NumberExpr(long val)
    : m_val(val)
{

}

long Int32NumberExpr::getValue()
{
    return m_val;
}


llvm::Value *Int32NumberExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return llvm::ConstantInt::get(module.getContext(), llvm::APInt(32, m_val));
}
