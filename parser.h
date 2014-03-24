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
    std::map<std::string, llvm::Value *> functions;
    Lexer *m_lexer;
    std::map<char, int> m_binopPrecedence;
    Token m_currentToken;
    BaseExpression *_handleTypeSpecifier(std::string type, Scope *scope);
    Token _getNextToken();
    BaseExpression *_handleVariableDeclaration(std::string type, std::string identifier);
    BaseExpression *_handleGlobalVariableDeclaration(std::string type, std::string identifier);
    BaseExpression *_handleFunctionDeclaration(std::string type, std::string identifier);
    FunctionArgument *_handleParameterDeclaratrion(std::string type);
    BlockDefinition *_handleBlockDeclaration(const std::vector<BaseExpression *> &args, Scope *scope);
    BaseExpression *_handleIfStatement();
    BaseExpression *_handleWhileLoop();
    BaseExpression *_handleNumberExpression();
    BaseExpression *_handleCallFunctionExpression(std::string identifier);
    BaseExpression *_handleIdentifierExpression();
    BaseExpression *_handleParenthesesExpression();
    BaseExpression *_handlePrimaryExpression();
    BaseExpression *_handleExpression();
    BaseExpression *_handleBinaryOperationExpression(int ExprPrec, BaseExpression *LHS);
public:
    Parser(Lexer *lexer);
    BlockDefinition *parse(llvm::Function *printf, llvm::Function *scanf);
};

}

#endif // PARSER_H
