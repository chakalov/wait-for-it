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
NumberExpression::NumberExpression(double val) : m_val(val) {}
VariableDeclarationExpression::VariableDeclarationExpression(const std::string &type, const std::string &name) : m_type(type), m_name(name) {}
BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs) {}
CallExpression::CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args) {}
FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<FunctionArgument *> &args, std::string returnType) : m_name(name), m_args(args), m_returnType(returnType) {}
FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body) : m_prototype(prototype), m_body(body) {}
BlockDefinition::BlockDefinition(std::vector<BaseExpression *> &expressions) : m_expressions(expressions) {}
IfStatmentExpression::IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock) : m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock) {}
IdentifierExpression::IdentifierExpression(std::string name) : m_name(name) {}
GlobalVariableExpression::GlobalVariableExpression(const std::string &type, const std::string &name) : VariableDeclarationExpression(type, name) {}
FunctionArgument::FunctionArgument(std::string type, std::string name) : mType(type), mName(name) {}

llvm::Value *NumberExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
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

llvm::Value *BinaryExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Value *L = m_lhs->emitCode(builder, module);
    llvm::Value *R = m_rhs->emitCode(builder, module);
    if (L == 0 || R == 0) {
        return 0;
    }

    switch (m_op) {
    case '+':
        return builder.CreateFAdd(L, R, "addtmp");
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

}

llvm::Function *FunctionPrototype::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type* returnType = getLLVMTypeTTT(builder, m_returnType);

    llvm::FunctionType *functionType = 0;

    if (m_args.size() > 0) {
        std::vector<llvm::Type*> args;

        for(std::vector<FunctionArgument *>::iterator it = m_args.begin(); it != m_args.end(); it++) {
            args.push_back(getLLVMTypeTTT(builder, (*it)->mType));
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
            AI->setName(m_args[i]->mName);
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
    printf("IFF (");
    m_expression->emitCode(builder, module);
    printf("\t{{");
    m_ifBlock->emitCode(builder, module);
    printf("}}\n");
}

std::string IdentifierExpression::getValue()
{
    return m_name;
}

llvm::Value *IdentifierExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    printf("$Ident[%s]$", m_name.c_str());
}

llvm::Value *GlobalVariableExpression::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Type *type = getLLVMTypeTTT(builder, m_type);

    return m_value = new llvm::GlobalVariable(module, type, false, llvm::GlobalValue::WeakAnyLinkage, 0, m_name.c_str());
}
