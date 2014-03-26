#include "ast.h"

#include <cstdio>

using namespace wait_for_it;

// destructors
Expr::~Expr() {}

// constructors
BinaryExpression::BinaryExpression(char op, Expr *lhs, Expr *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs) {}
CallExpression::CallExpression(const std::string &callee, const std::vector<Expr *> &args) : m_callee(callee), m_args(args) {}
FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<FunctionArgumentExpr *> &args, std::string returnType) : m_name(name), m_args(args), m_returnType(returnType) {}

void FunctionPrototype::allocateArguments(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    for(std::vector<FunctionArgumentExpr *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
        (*it)->emitCode(builder, module);
    }
}

FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, Expr *body) : m_prototype(prototype), m_body(body) {}
BlockDefinition::BlockDefinition(std::vector<Expr *> &expressions) : m_expressions(expressions) {}
IfStatmentExpression::IfStatmentExpression(Expr *expression, Expr *ifBlock, Expr *elseBlock) : m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock) {}
PredefinedFunction::PredefinedFunction(llvm::Function *predefined) : FunctionDefinition(NULL, NULL), m_predefined(predefined) {}


llvm::Value *Int32NumberExpr::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return llvm::ConstantInt::get(module.getContext(), llvm::APInt(32, m_val));
}

llvm::Value *PredefinedFunction::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return m_predefined;
}

llvm::Value *BinaryExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *L, *R;
    if (m_op == '=') {
        IdentifierExpr *identifier = dynamic_cast<IdentifierExpr *>(m_lhs);
        L = identifier->getValue();
        R = m_rhs->emitCode(builder, module);
        if (L == 0 || R == 0) {
            return 0;
        }
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

llvm::Value *CallExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Function *proto = module.getFunction(m_callee);
    std::vector<llvm::Value*> ArgsV;

    for(std::vector<Expr *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
        ArgsV.push_back((*it)->emitCode(builder, module));
    }

    llvm::CallInst *call = builder.CreateCall(proto, ArgsV, "calltmp");
    call->setTailCall(false);

    return call;
}

llvm::Function *FunctionPrototype::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
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

llvm::Value *FunctionDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Function *functionPrototype = m_prototype->emitCode(builder, module);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(module.getContext(), "entrypoint", functionPrototype);
    builder.SetInsertPoint(block);
    m_prototype->allocateArguments(builder, module);
    m_body->emitCode(builder, module);

    return functionPrototype;
}

llvm::Value *BlockDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *val = NULL;
    for (std::vector<Expr *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
        val = (*expression)->emitCode(builder, module);
    }
    return val;
}

llvm::Value *IfStatmentExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
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
    }
    return PN;
}
