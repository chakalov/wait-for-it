#include <cstdio>

#include "lexer.h"

using namespace wait_for_it;

int main ()
{
    Lexer lexer("/home/spas/qtprojects/wait-for-it/test1.txt");

    Token token;
    do {
        token = lexer.getNextToken();
        printf("%c : [%s]\n", token.type, token.value.c_str());
    }
    while (token.type != TOKEN_EOF);
}
