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
    LocalVariableExpr *_handleVariableDeclaration(std::string type, std::string identifier);
    GlobalVariableExpr *_handleGlobalVariableDeclaration(std::string type, std::string identifier);
    FunctionExpr *_handleFunctionDeclaration(std::string type, std::string identifier);
    FunctionArgumentExpr *_handleParameterDeclaratrion(std::string type);
    BlockExpr *_handleBlockDeclaration(const std::vector<Expr *> &args, Scope *scope);
    IfExpr *_handleIfStatement();
    WhileExpr *_handleWhileLoop();
    NumberExpr *_handleNumberExpression();
    CallExpr *_handleCallFunctionExpression(std::string identifier);
    Expr *_handleIdentifierExpression();
    Expr *_handleParenthesesExpression();
    Expr *_handlePrimaryExpression();
    Expr *_handleExpression();
    Expr *_handleBinaryOperationExpression(int ExprPrec, Expr *LHS);
    bool emitCode = true;
public:
    Parser(Lexer *lexer);
    BlockExpr *parse();
    bool canEmit();
};

}

#endif // PARSER_H
