#include "ast.h"

#include <cstdio>

using namespace wait_for_it;

llvm::Type *getLLVMTypeTTT(llvm::IRBuilder<> &builder, std::string type)
{
    return llvm::StringSwitch<llvm::Type *>(llvm::StringRef(type))
            .Case("void", builder.getVoidTy())
            .Case("char", builder.getInt8Ty())
            .Case("bool", builder.getInt1Ty())
            .Cases("short", "int", builder.getInt32Ty())
            .Cases("long", "double", builder.getDoubleTy())
            .Cases("signed", "unsigned", builder.getInt32Ty());
}

// destructors
BaseExpression::~BaseExpression() {}

// constructors
DoubleNumberExpression::DoubleNumberExpression(double val) : m_val(val) {}
VariableDeclarationExpression::VariableDeclarationExpression(const std::string &type, const std::string &name) : m_type(type), m_name(name) {}
BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs) {}
CallExpression::CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args) {}
FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<FunctionArgument *> &args, std::string returnType) : m_name(name), m_args(args), m_returnType(returnType) {}
FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body) : m_prototype(prototype), m_body(body) {}
BlockDefinition::BlockDefinition(std::vector<BaseExpression *> &expressions) : m_expressions(expressions) {}
IfStatmentExpression::IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock) : m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock) {}
IdentifierExpression::IdentifierExpression(VariableDeclarationExpression *var) : m_var(var) {
    FunctionArgument *arg = dynamic_cast<FunctionArgument *> (m_var);
    if (arg) {
        hasAlloca = false;
    } else {
        hasAlloca = true;
    }
}
GlobalVariableExpression::GlobalVariableExpression(const std::string &type, const std::string &name) : VariableDeclarationExpression(type, name) {}
FunctionArgument::FunctionArgument(std::string type, std::string name) : VariableDeclarationExpression(type, name) {}

std::string FunctionArgument::getType()
{
    return m_type;
}

std::string FunctionArgument::getName()
{
    return m_name;
}

void FunctionArgument::setValue(llvm::Value *val)
{
    m_value = val;
}
PredefinedFunction::PredefinedFunction(llvm::Function *predefined) : FunctionDefinition(NULL, NULL), m_predefined(predefined) {}
StringExpression::StringExpression(std::string val) : m_val(val) {}
IntegerNumberExpression::IntegerNumberExpression(long val) : m_val(val) {}

long IntegerNumberExpression::getValue()
{
    return m_val;
}

llvm::Value *IntegerNumberExpression::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return llvm::ConstantInt::get(module.getContext(), llvm::APInt(32, m_val));
}

std::string StringExpression::getValue()
{
    return m_val;
}

llvm::Value *StringExpression::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(builder.getContext(), m_val.c_str());
    llvm::GlobalVariable *var = new llvm::GlobalVariable(module, llvm::ArrayType::get(builder.getInt8Ty(), m_val.length() + 1), true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");

    return var;
}

llvm::Value *PredefinedFunction::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    return m_predefined;
}

llvm::Value *DoubleNumberExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    return llvm::ConstantFP::get(module.getContext(), llvm::APFloat(m_val));
}

llvm::Value *VariableDeclarationExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type *type = getLLVMTypeTTT(builder, m_type);

    return m_value = builder.CreateAlloca(type, 0, m_name.c_str());
}

llvm::Value *VariableDeclarationExpression::getValue()
{
    return m_value;
}

std::string VariableDeclarationExpression::getName()
{
    return m_name;
}

llvm::Value *BinaryExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *L, *R;
    if (m_op == '=') {
        IdentifierExpression *identifier = dynamic_cast<IdentifierExpression *>(m_lhs);
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

    for(std::vector<BaseExpression *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
        ArgsV.push_back((*it)->emitCode(builder, module));
    }

    llvm::CallInst *call = builder.CreateCall(proto, ArgsV, "calltmp");
    call->setTailCall(false);

    return call;
}

llvm::Function *FunctionPrototype::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type* returnType = getLLVMTypeTTT(builder, m_returnType);

    llvm::FunctionType *functionType = 0;

    if (m_args.size() > 0) {
        std::vector<llvm::Type*> args;

        for(std::vector<FunctionArgument *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
            args.push_back(getLLVMTypeTTT(builder, (*it)->getType()));
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
            m_args[i]->setValue(AI);
        }
    }

    return functionPrototype;
}

llvm::Value *FunctionDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Function *functionPrototype = m_prototype->emitCode(builder, module);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(module.getContext(), "entrypoint", functionPrototype);
    builder.SetInsertPoint(block);
    m_body->emitCode(builder, module);

    return functionPrototype;
}

llvm::Value *BlockDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *val = NULL;
    for (std::vector<BaseExpression *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
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

llvm::Value *IdentifierExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    if (hasAlloca) {
        return builder.CreateLoad(m_var->getValue(), m_var->getName());
    } else {
        return m_var->getValue();
    }
}

llvm::Value *IdentifierExpression::getValue()
{
    return m_var->getValue();
}

llvm::Value *GlobalVariableExpression::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Type *type = getLLVMTypeTTT(builder, m_type);

    return m_value = new llvm::GlobalVariable(module, type, false, llvm::GlobalValue::WeakAnyLinkage, 0, m_name.c_str());
}
