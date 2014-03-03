#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <fstream>

namespace wait_for_it {

class Lexer
{
    std::ifstream *m_sourceStream;
public:
    Lexer(char *);
    virtual ~Lexer();
    Token getNextToken();
};

}

#endif // LEXER_H
