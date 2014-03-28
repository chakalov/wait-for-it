#include "ifexpr.h"

using namespace wait_for_it;

IfExpr::IfExpr(Expr *expression, BlockExpr *ifBlock, BlockExpr *elseBlock)
    : m_expression(expression),
      m_ifBlock(ifBlock),
      m_elseBlock(elseBlock)
{

}

llvm::Value *IfExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *ElseV = NULL;
    llvm::Value *CondV = m_expression->emitCode(builder, module);
    if (CondV == 0) {
        return 0;
    }

    CondV = builder.CreateFCmpONE(CondV, llvm::ConstantFP::get(builder.getContext(), llvm::APFloat(0.0)), "ifcond");

    // get the function where the if expression is in
    llvm::Function *parent = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(builder.getContext(), "then", parent);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(builder.getContext(), "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(builder.getContext(), "ifcont");

    builder.CreateCondBr(CondV, ThenBB, ElseBB);

    builder.SetInsertPoint(ThenBB);
    llvm::Value *ThenV = m_ifBlock->emitCode(builder, module);
    if (ThenV == 0) {
        return 0;
    }

    builder.CreateBr(MergeBB);

    ThenBB = builder.GetInsertBlock();

    parent->getBasicBlockList().push_back(ElseBB);
    builder.SetInsertPoint(ElseBB);

    if (m_elseBlock) {
        ElseV = m_elseBlock->emitCode(builder, module);
        if (ElseV == 0) {
            return 0;
        }
    }

    builder.CreateBr(MergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = builder.GetInsertBlock();

    // Emit merge block.
    parent->getBasicBlockList().push_back(MergeBB);
    builder.SetInsertPoint(MergeBB);
    llvm::PHINode *PN = builder.CreatePHI(builder.getDoubleTy(), 2, "iftmp");

    PN->addIncoming(ThenV, ThenBB);
    if (m_elseBlock) {
        PN->addIncoming(ElseV, ElseBB);
    } else {
        PN->addIncoming(ThenV, ElseBB);
    }
    return PN;
}
