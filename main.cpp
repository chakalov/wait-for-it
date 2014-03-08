#include <cstdio>

#include "lexer.h"
#include "parser.h"

using namespace wait_for_it;

int main ()
{
    Lexer lexer("/home/chakalov/qtprojects/llvm/test2.txt");
    Parser parser(&lexer);

    parser.parse();
}
