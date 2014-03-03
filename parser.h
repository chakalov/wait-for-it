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
    Token m_currToken;
    Token _getNextToken();
    std::map<int, int> m_binaryOperatorPrecedence;

    BaseExpression *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
    FunctionPrototype *ErrorP(const char *Str) { Error(Str); return 0; }
    FunctionDefinition *ErrorF(const char *Str) { Error(Str); return 0; }

    BaseExpression *_parseNumberExpression();
    BaseExpression *_parseParenthesisExpression();
    BaseExpression *_parseIdentifierExpression();
    BaseExpression *_parsePrimary();
    BaseExpression *_parseExpression();
    BaseExpression *_parseBinaryOperationExpression(int expressionPrecedence, BaseExpression *lhs);
public:
    Parser(Lexer *lexer);
};

}

#endif // PARSER_H
