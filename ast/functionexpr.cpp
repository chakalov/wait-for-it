#include "functionexpr.h"

using namespace wait_for_it;

FunctionExpr::FunctionExpr(FunctionPrototypeExpr *prototype, BlockExpr *body)
    : m_prototype(prototype),
      m_body(body)
{

}

llvm::Value *FunctionExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Function *functionPrototype = m_prototype->emitCode(builder, module);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(module.getContext(), "entrypoint", functionPrototype);
    builder.SetInsertPoint(block);
    m_prototype->allocateArguments(builder, module);
    m_body->emitCode(builder, module);

    return functionPrototype;
}
