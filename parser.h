#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "scope.h"
#include "ast.h"
#include "token.h"
#include <cstdlib>
#include <vector>
#include <map>
#include <stack>


namespace wait_for_it {

class Parser
{
    std::vector<Scope *> scopes;
    Lexer *m_lexer;
    std::map<char, int> m_binopPrecedence;
    Token m_currentToken;
    VariableExpr *findVariable(std::string name);
    Expr *_handleTypeSpecifier(std::string type, Scope *scope);
    Token _getNextToken();
    Expr *_handleVariableDeclaration(std::string type, std::string identifier);
    Expr *_handleGlobalVariableDeclaration(std::string type, std::string identifier);
    Expr *_handleFunctionDeclaration(std::string type, std::string identifier);
    FunctionArgumentExpr *_handleParameterDeclaratrion(std::string type);
    BlockDefinition *_handleBlockDeclaration(const std::vector<Expr *> &args, Scope *scope);
    Expr *_handleIfStatement();
    Expr *_handleWhileLoop();
    Expr *_handleNumberExpression();
    Expr *_handleCallFunctionExpression(std::string identifier);
    Expr *_handleIdentifierExpression();
    Expr *_handleParenthesesExpression();
    Expr *_handlePrimaryExpression();
    Expr *_handleExpression();
    Expr *_handleBinaryOperationExpression(int ExprPrec, Expr *LHS);
public:
    Parser(Lexer *lexer);
    BlockDefinition *parse();
};

}

#endif // PARSER_H
