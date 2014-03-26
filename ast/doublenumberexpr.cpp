#include "doublenumberexpr.h"

using namespace wait_for_it;

DoubleNumberExpr::DoubleNumberExpr(double val)
    : m_val(val)
{

}

llvm::Value *DoubleNumberExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    return llvm::ConstantFP::get(module.getContext(), llvm::APFloat(m_val));
}
