#include "parser.h"

using namespace wait_for_it;

BaseExpression *Parser::_handleTypeSpecifier(std::string type)
{
    _getNextToken();

    if (m_currentToken.type != TOKEN_IDENTIFIER) {
        // Error();
        return NULL;
    }

    std::string identifier = m_currentToken.value;

    _getNextToken();

    switch(m_currentToken.type) {
    case TOKEN_SEMICOLON:
        return _handleVariableDeclaration(type, identifier);
    case TOKEN_OPEN_PARENTHESES:
        return _handleFunctionDeclaration(type, identifier);
    default:
        return NULL;
    }
}

Token Parser::_getNextToken()
{
    //    return m_currentToken = m_lexer->getNextToken();
    m_currentToken = m_lexer->getNextToken();
    printf("--debug: {{%d:[%c]:%s}}\n", m_currentToken.line, m_currentToken.type, m_currentToken.value.c_str());
    return m_currentToken;
}

BaseExpression *Parser::_handleVariableDeclaration(std::string type, std::string identifier)
{
    return new VariableExpression(type, identifier);
}

BaseExpression *Parser::_handleFunctionDeclaration(std::string type, std::string identifier)
{
    _getNextToken();

    FunctionPrototype *prototype;
    BlockDefinition *body;
    std::vector<BaseExpression *> args;

    while (1) {
        switch(m_currentToken.type) {
        case TOKEN_COMMA:
            _getNextToken();
            if (m_currentToken.type == TOKEN_COMMA) {
                // printf("Error on line %d: unexpected token ','", m_currentToken.line);
            }
            break;
        case TOKEN_CLOSE_PARENTHESES:
            _getNextToken();
            prototype = new FunctionPrototype(identifier, args);
            break;
        case TOKEN_TYPE_SPECIFIER:
            args.push_back(_handleParameterDeclaratrion(m_currentToken.value));
            _getNextToken();
            break;
        case TOKEN_OPEN_BRACES:
            return new FunctionDefinition(prototype, _handleBlockDeclaration(args));
            break;
        default:
            return NULL;
        }
    }

    return new FunctionDefinition(prototype, body);
}

BaseExpression *Parser::_handleParameterDeclaratrion(std::string type)
{
    _getNextToken();

    if (m_currentToken.type != TOKEN_IDENTIFIER) {
        return NULL;
    }

    return new VariableExpression(type, m_currentToken.value);
}

BlockDefinition *Parser::_handleBlockDeclaration(const std::vector<BaseExpression *> &args)
{
    std::vector<BaseExpression *> expressions;
    while (m_currentToken.type != TOKEN_EOF) {
        switch (m_currentToken.type) {
        case TOKEN_TYPE_SPECIFIER:
            expressions.push_back(_handleTypeSpecifier(m_currentToken.value));
            break;
            //        case TOKEN_IF:
            //            expressions.push_back(_handleIfStatement());
            //            break;
            //        case TOKEN_STRUCT:
            //            break;
            //        case TOKEN_UNION:
            //            break;
            //        case TOKEN_ENUM:
            //            break;
        case TOKEN_CLOSE_BRACES:
            return new BlockDefinition(expressions);
        default:
            expressions.push_back(_handleExpression());
            break;
        }
        _getNextToken();
    }

    return new BlockDefinition(expressions);
}

BaseExpression *Parser::_handleParenthesesExpression()
{
    BaseExpression *expr;
    _getNextToken();
    expr = _handleExpression();
    _getNextToken();
    return expr;
}

BaseExpression *Parser::_handlePrimaryExpression()
{
    switch(m_currentToken.type) {
    case TOKEN_NUMBER:
        return new NumberExpression(strtod(m_currentToken.value.c_str(), 0));
        break;
    case TOKEN_IDENTIFIER:
        return new IdentifierExpression(m_currentToken.value);
        break;
    case TOKEN_OPEN_PARENTHESES:
        return _handleParenthesesExpression();
        break;
    }
}

//BaseExpression *Parser::_handleIfStatement()
//{
//    _getNextToken();
//    if(m_currentToken.type != TOKEN_OPEN_PARENTHESES) {
//        printf("'(' expected on line: %d", m_currentToken.line);
//    }
//    _handleSimpleExpression();

//    return NULL; //TODO: make it return IfExpression
//}

BaseExpression *Parser::_handleExpression()
{
    BaseExpression *LHS;
    LHS = _handlePrimaryExpression();
    if (!LHS) {
        printf("nqkva gre6ka");
    }
    _handleBinaryOperationExpression(0, LHS);
}

BaseExpression *Parser::_handleBinaryOperationExpression(int ExprPrec, BaseExpression *LHS)
{
    while (1) {
//        int TokPrec = GetTokPrecedence();

//        if (TokPrec < ExprPrec) {
//            return LHS;
//        }

        Token BinOp = m_currentToken;
        _getNextToken();

        // Parse the primary expression after the binary operator.
        BaseExpression *RHS = _handlePrimaryExpression();
        if (!RHS) return LHS;

//        // If BinOp binds less tightly with RHS than the operator after RHS, let
//        // the pending operator take RHS as its LHS.
//        int NextPrec = GetTokPrecedence();
//        if (TokPrec < NextPrec) {
//            RHS = ParseBinOpRHS(TokPrec+1, RHS);
//            if (RHS == 0) return 0;
//        }

        // Merge LHS/RHS.
        LHS = new BinaryExpression(BinOp.type, LHS, RHS);
    }
}

Parser::Parser(Lexer *lexer) : m_lexer(lexer)
{
}

void Parser::parse()
{
    _getNextToken();
    _handleBlockDeclaration(*(new std::vector<BaseExpression *>()))->emitCode();
}
