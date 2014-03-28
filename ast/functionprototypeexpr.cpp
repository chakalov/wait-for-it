#include "functionprototypeexpr.h"

using namespace wait_for_it;

FunctionPrototypeExpr::FunctionPrototypeExpr(const std::string &name, const std::vector<FunctionArgumentExpr *> &args, std::string returnType)
    : m_name(name),
      m_args(args),
      m_returnType(returnType)
{

}

void FunctionPrototypeExpr::allocateArguments(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    for(std::vector<FunctionArgumentExpr *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
        (*it)->emitCode(builder, module);
    }
}

llvm::Function *FunctionPrototypeExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type* returnType = getLLVMType(builder, m_returnType);

    llvm::FunctionType *functionType = 0;

    if (m_args.size() > 0) {
        std::vector<llvm::Type*> args;

        for(std::vector<FunctionArgumentExpr *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
            args.push_back(getLLVMType(builder, (*it)->getType()));
        }

        functionType = llvm::FunctionType::get(returnType, args, false);
    } else {
        functionType = llvm::FunctionType::get(returnType, false);
    }

    llvm::Function *functionPrototype = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, &module);

    if(m_args.size())
    {
        unsigned i = 0;
        for (llvm::Function::arg_iterator AI = functionPrototype->arg_begin(); i != m_args.size(); ++AI, ++i)
        {
            AI->setName(m_args[i]->getName());
            m_args[i]->setParameter(AI);
        }
    }

    return functionPrototype;
}
