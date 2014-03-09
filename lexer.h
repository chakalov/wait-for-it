#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <fstream>

namespace wait_for_it {

class Lexer
{
    int m_currentLineNumber;
    std::ifstream *m_sourceStream;
public:
    Lexer(const char *);
    virtual ~Lexer();
    Token getNextToken();
};

}

#endif // LEXER_H
