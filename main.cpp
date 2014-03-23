#include <cstdio>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/PassManager.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/TargetSelect.h>
#include "llvm/ExecutionEngine/JIT.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "ast.h"

using namespace wait_for_it;
using namespace llvm;

static Module *module;
static IRBuilder<> builder(getGlobalContext());

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

    module = new Module("wait for it jit", Context);

    llvm::Function *printf_func = printf_prototype(Context, module);
    llvm::Function *scanf_func = scanf_prototype(Context, module);

    Lexer lexer("/home/spas/qtprojects/wait-for-it/main.txt");

    Parser parser(&lexer);

    BlockDefinition *ast = parser.parse(printf_func, scanf_func);

    ast->emitCode(builder, *module);

    builder.CreateRet(ConstantInt::get(getGlobalContext(), APInt(32, 5)));
    module->dump();

    llvm::InitializeNativeTarget();

    std::string ErrStr;
    //llvm::ExecutionEngine *engine = llvm::EngineBuilder(module).setErrorStr(&ErrStr).create();
    llvm::ExecutionEngine *engine = llvm::ExecutionEngine::create(module);

    if(engine) {
        llvm::Function* mainFunction = module->getFunction("main");
        if(!mainFunction)
        {
            printf("No main function");
        }
        else
        {
            printf("######### BEGIN \"%s\" OUTPUT #########\n"/*, argv[1]*/);

            std::vector<llvm::GenericValue> args;
            llvm::GenericValue val;
            val.IntVal = llvm::APInt(32, 6);
            args.push_back(val);
            engine->runFunction(mainFunction, args);

            printf("\nexited with code \n");
        }
    } else {
        printf("Failed to initialize the execution engine: %s", ErrStr.c_str());
    }
}
