#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

namespace wait_for_it {

class Parser
{
    Lexer *m_lexer;
public:
    Parser(Lexer *lexer);
};

}

#endif // PARSER_H
