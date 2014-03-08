#include <cstdio>

#include "lexer.h"
#include "parser.h"
#include "token.h"

using namespace wait_for_it;

int main ()
{
    Lexer lexer("/home/spas/qtprojects/wait-for-it/test2.txt");
    //Token tok;

    //while ( (tok = lexer.getNextToken()).type != TOKEN_EOF )
      //  printf("%s\n", tok.value.c_str());

    Parser parser(&lexer);

    parser.parse();
}
