#include <cstdio>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "ast.h"

using namespace wait_for_it;
using namespace llvm;

static Module *TheModule;
static IRBuilder<> Builder(getGlobalContext());

static llvm::Function* printf_prototype(llvm::LLVMContext& context, llvm::Module *module)
{
    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(context));

    llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), printf_arg_types, true);
    llvm::Function *func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), module);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

static llvm::Function* scanf_prototype(llvm::LLVMContext& context, llvm::Module *module)
{
    std::vector<llvm::Type*> scanf_arg_types;
    scanf_arg_types.push_back(llvm::Type::getInt8PtrTy(context));

    llvm::FunctionType* scanf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), scanf_arg_types, true);
    llvm::Function *func = llvm::Function::Create(scanf_type, llvm::Function::ExternalLinkage, llvm::Twine("scanf"), module);
    func->setCallingConv(llvm::CallingConv::C);

    return func;
}



int main ()
{
    LLVMContext &Context = getGlobalContext();

    TheModule = new Module("wait for it jit", Context);

    llvm::Function *printf_func = printf_prototype(Context, TheModule);
    llvm::Function *scanf_func = scanf_prototype(Context, TheModule);

    Lexer lexer("/home/chakalov/qtprojects/llvm/test3.txt");

    Parser parser(&lexer);

    BlockDefinition *ast = parser.parse();

    ast->emitCode(Builder, *TheModule);

    TheModule->dump();

    printf("FINISH HIM;\n");
}
