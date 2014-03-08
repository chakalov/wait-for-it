#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <cstdlib>
#include <map>

namespace wait_for_it {

class Parser
{
    Lexer *m_lexer;
    Token m_currentToken;

    Token _getNextToken();

    BaseExpression *_handleDeclarator(std::string declaration, std::string type, int pointer = 0);
    BaseExpression *_handleDeclarationSpecifier(std::string declaration);
    BaseExpression *_handleTypeSpecifier(std::string declaration, std::string type);
public:
    Parser(Lexer *lexer);
    void parse();
};

}

#endif // PARSER_H
