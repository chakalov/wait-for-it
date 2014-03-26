#include "stringexpr.h"

using namespace wait_for_it;

StringExpr::StringExpr(std::string val)
    : m_val(val)
{

}

std::string StringExpr::getValue()
{
    return m_val;
}

llvm::Value *StringExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(builder.getContext(), m_val.c_str());
    llvm::GlobalVariable *var = new llvm::GlobalVariable(module, llvm::ArrayType::get(builder.getInt8Ty(), m_val.length() + 1), true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");

    return var;
}
