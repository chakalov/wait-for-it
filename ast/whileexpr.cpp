#include "whileexpr.h"

using namespace wait_for_it;

WhileExpr::WhileExpr(Expr *expression, BlockExpr *Block)
    : m_expression(expression),
      m_block(Block)
{

}

llvm::Value *WhileExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return NULL;
}
