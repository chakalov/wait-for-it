#include "blockexpr.h"

using namespace wait_for_it;

BlockExpr::BlockExpr(std::vector<Expr *> &expressions)
    : m_expressions(expressions)
{

}

llvm::Value *BlockExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *val = NULL;
    for (std::vector<Expr *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
        val = (*expression)->emitCode(builder, module);
    }
    return val;
}
