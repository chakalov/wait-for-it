#include "whileexpr.h"

using namespace wait_for_it;

WhileExpr::WhileExpr(Expr *expression, BlockExpr *Block)
    : m_expression(expression),
      m_block(Block)
{

}

llvm::Value *WhileExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Value *pCondV = m_expression->emitCode(builder, module);
    if (pCondV == 0) {
        return 0;
    }

    llvm::Value *CondV = builder.CreateFCmpONE(pCondV, llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(0.0)), "whilecond");

    // get the function where the if expression is in
    llvm::Function *parent = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(builder.getContext(), "loop", parent);
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(builder.getContext(), "whilecont");

    builder.CreateCondBr(CondV, LoopBB, MergeBB);

    builder.SetInsertPoint(LoopBB);
    llvm::Value *loopV = m_block->emitCode(builder, module);
    if (loopV == 0) {
        return 0;
    }

    CondV = builder.CreateFCmpONE(m_expression->emitCode(builder, module), llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(0.0)), "whilecond");
    builder.CreateCondBr(CondV, LoopBB, MergeBB);

    LoopBB = builder.GetInsertBlock();

    // Emit merge block.
    parent->getBasicBlockList().push_back(MergeBB);
    builder.SetInsertPoint(MergeBB);
    llvm::PHINode *PN = builder.CreatePHI(builder.getDoubleTy(), 2, "whiletmp");

    PN->addIncoming(loopV, LoopBB);
    PN->addIncoming(loopV, LoopBB);
    return PN;
}
