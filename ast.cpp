#include "ast.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/ADT/StringSwitch.h>

#include <cstdio>

using namespace wait_for_it;

BaseExpression::~BaseExpression()
{
}

NumberExpression::NumberExpression(double val) : m_val(val)
{
}

llvm::Value *NumberExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(m_val));
}

VariableExpression::VariableExpression(const std::string &type, const std::string &name) : m_type(type), m_name(name)
{
}

llvm::Value *VariableExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::Type *type = llvm::StringSwitch<llvm::Type *>(llvm::StringRef(m_type))
            .Case("char", llvm::Type::getInt8Ty(llvm::getGlobalContext()))
            .Case("bool", llvm::Type::getInt1Ty(llvm::getGlobalContext()))
            .Cases("short", "int", llvm::Type::getInt32Ty(llvm::getGlobalContext()))
            .Cases("long", "double", llvm::Type::getDoubleTy(llvm::getGlobalContext()))
            .Cases("signed", "unsigned", llvm::Type::getInt32Ty(llvm::getGlobalContext()));

    return builder.CreateAlloca(type, 0, m_name.c_str());
}

BinaryExpression::BinaryExpression(char op, BaseExpression *lhs, BaseExpression *rhs) : m_op(op), m_lhs(lhs), m_rhs(rhs)
{
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
        return builder.CreateUIToFP(L, llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltmp");
    default:
        return NULL; //ErrorV("invalid binary operator");
    }
}

CallExpression::CallExpression(const std::string &callee, const std::vector<BaseExpression *> &args) : m_callee(callee), m_args(args)
{
}

llvm::Value *CallExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{

}

FunctionPrototype::FunctionPrototype(const std::string &name, const std::vector<BaseExpression *> &args) : m_name(name), m_args(args)
{
}

llvm::Value *FunctionPrototype::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    printf("Proto[%s (", m_name.c_str());
    for (std::vector<BaseExpression *>::iterator arg = m_args.begin() ; arg != m_args.end(); ++arg) {
        (*arg)->emitCode(builder, module);
    }
    printf(")]\n");
}

FunctionDefinition::FunctionDefinition(FunctionPrototype *prototype, BaseExpression *body) : m_prototype(prototype), m_body(body)
{
}

llvm::Value *FunctionDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    printf("Func[");
    m_prototype->emitCode(builder, module);
    m_body->emitCode(builder, module);
    printf("]\n");
}

BlockDefinition::BlockDefinition(std::vector<BaseExpression *> &expressions, Scope scope) : m_expressions(expressions), m_scope(scope)
{
}

llvm::Value *BlockDefinition::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", &module);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);
    for (std::vector<BaseExpression *>::iterator expression = m_expressions.begin(); expression != m_expressions.end(); ++expression) {
        (*expression)->emitCode(builder, module);
    }

    return entry;
}


IfStatmentExpression::IfStatmentExpression(BaseExpression *expression, BaseExpression *ifBlock, BaseExpression *elseBlock) :
    m_expression(expression), m_ifBlock(ifBlock), m_elseBlock(elseBlock)
{

}

llvm::Value *IfStatmentExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    printf("IFF (");
    m_expression->emitCode(builder, module);
    printf("\t{{");
    m_ifBlock->emitCode(builder, module);
    printf("}}\n");
}


IdentifierExpression::IdentifierExpression(std::string name) : m_name(name)
{

}

std::string IdentifierExpression::getValue()
{
    return m_name;
}

llvm::Value *IdentifierExpression::emitCode(llvm::IRBuilder<>& builder, llvm::Module &module)
{
    printf("$Ident[%s]$", m_name.c_str());
}


GlobalVariableExpression::GlobalVariableExpression(const std::string &type, const std::string &name) : VariableExpression(type, name)
{
}

llvm::Value *GlobalVariableExpression::emitCode(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    llvm::Type *type = llvm::StringSwitch<llvm::Type *>(llvm::StringRef(m_type))
            .Case("char", llvm::Type::getInt8Ty(llvm::getGlobalContext()))
            .Case("bool", llvm::Type::getInt1Ty(llvm::getGlobalContext()))
            .Cases("short", "int", llvm::Type::getInt32Ty(llvm::getGlobalContext()))
            .Cases("long", "double", llvm::Type::getDoubleTy(llvm::getGlobalContext()))
            .Cases("signed", "unsigned", llvm::Type::getInt32Ty(llvm::getGlobalContext()));

    return new llvm::GlobalVariable(module, type, false, llvm::GlobalValue::CommonLinkage, 0, m_name.c_str());
}
