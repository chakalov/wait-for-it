#include <cstdio>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"

using namespace wait_for_it;
using namespace llvm;

static Module *TheModule;
static IRBuilder<> Builder(getGlobalContext());
static std::map<std::string, Value*> NamedValues;

int main ()
{
    LLVMContext &Context = getGlobalContext();

    TheModule = new Module("wait for it jit", Context);
    Lexer lexer("/home/chakalov/qtprojects/llvm/test3.txt");
    //Token tok;

    //while ( (tok = lexer.getNextToken()).type != TOKEN_EOF )
    //  printf("%s\n", tok.value.c_str());

    Parser parser(&lexer);

    BlockDefinition *ast = parser.parse();

    ast->emitCode(Builder, *TheModule);

    TheModule->dump();

    printf("FINISH HIM;\n");
}
