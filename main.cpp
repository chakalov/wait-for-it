#include <cstdio>

#include "lexer.h"

using namespace wait_for_it;

int main ()
{
    Lexer lexer("/home/chakalov/qtprojects/llvm/test1.txt");

    Token token;
    do {
        token = lexer.getNextToken();
        printf("%c : [%s]\n", token.type, token.value.c_str());
    }
    while (token.type != TOKEN_EOF);
}
