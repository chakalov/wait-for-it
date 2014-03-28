#include "callexpr.h"

using namespace wait_for_it;

CallExpr::CallExpr(const std::string &callee, const std::vector<Expr *> &args)
    : m_callee(callee),
      m_args(args)
{

}

llvm::Value *CallExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Function *proto = module.getFunction(m_callee);
    if (!proto) {
        return NULL;
    }

    std::vector<llvm::Value*> ArgsV;

    for(std::vector<Expr *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
        ArgsV.push_back((*it)->emitCode(builder, module));
    }

    llvm::CallInst *call = builder.CreateCall(proto, ArgsV, "calltmp");
    call->setTailCall(false);

    return call;
}
