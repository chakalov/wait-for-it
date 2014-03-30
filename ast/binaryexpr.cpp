#include "binaryexpr.h"

using namespace wait_for_it;

BinaryExpr::BinaryExpr(char op, Expr *lhs, Expr *rhs)
    : m_op(op),
      m_lhs(lhs),
      m_rhs(rhs)
{

}

llvm::Value *BinaryExpr::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *L, *R;
    if (m_op == '=') {
        IdentifierExpr *identifier = dynamic_cast<IdentifierExpr *>(m_lhs);
        L = identifier->getValue();
        R = m_rhs->emitCode(builder, module);
        if (L == 0 || R == 0) {
            return 0;
        }

//        if (L->getType() != R->getType()) {
//            R = typecast(builder, R, L->getType());
//        }

        return builder.CreateStore(R, L);
    }

    L = m_lhs->emitCode(builder, module);
    R = m_rhs->emitCode(builder, module);
    if (L == 0 || R == 0) {
        return 0;
    }

    switch (m_op) {
    case '+':
        return builder.CreateAdd(L, R, "addtmp");
    case '-':
        return builder.CreateFSub(L, R, "subtmp");
    case '*':
        return builder.CreateFMul(L, R, "multmp");
    case '<':
        L = builder.CreateFCmpULT(L, R, "cmptmp");
        return builder.CreateUIToFP(L, llvm::Type::getDoubleTy(module.getContext()), "booltmp");
    default:
        return NULL; //ErrorV("invalid binary operator");
    }
}
