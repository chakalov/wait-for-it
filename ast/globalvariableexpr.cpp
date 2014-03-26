#include "globalvariableexpr.h"

using namespace wait_for_it;

GlobalVariableExpr::GlobalVariableExpr(const std::string &type, const std::string &name)
    : VariableExpr(type, name)
{

}

llvm::Value *GlobalVariableExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Type *type = getLLVMType(builder, m_type);

    return m_value = new llvm::GlobalVariable(module, type, false, llvm::GlobalValue::WeakAnyLinkage, 0, m_name.c_str());
}
