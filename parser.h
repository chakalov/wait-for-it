#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "token.h"
#include <cstdlib>
#include <vector>
#include <map>


namespace wait_for_it {

class Parser
{
    Lexer *m_lexer;
    std::map<char, int> m_binopPrecedence;
    Token m_currentToken;
    BaseExpression *_handleTypeSpecifier(std::string type);
    Token _getNextToken();
    BaseExpression *_handleVariableDeclaration(std::string type, std::string identifier);
    BaseExpression *_handleFunctionDeclaration(std::string type, std::string identifier);
    BaseExpression *_handleParameterDeclaratrion(std::string type);
    BlockDefinition *_handleBlockDeclaration(const std::vector<BaseExpression *> &args, BlockDefinition::Scope scope);
    BaseExpression *_handleIfStatement();
    BaseExpression *_handleNumberExpression();
    BaseExpression *_handleIdentifierExpression();
    BaseExpression *_handleParenthesesExpression();
    BaseExpression *_handlePrimaryExpression();
    BaseExpression *_handleExpression();
    BaseExpression *_handleBinaryOperationExpression(int ExprPrec, BaseExpression *LHS);
public:
    Parser(Lexer *lexer);
    BlockDefinition *parse();
};

}

#endif // PARSER_H
