#include "parser.h"
#include "scope.h"

using namespace wait_for_it;

BaseExpression *Parser::_handleTypeSpecifier(std::string type, Scope *scope)
{
    _getNextToken();

    if (m_currentToken.type != TOKEN_IDENTIFIER) {
        // Error();
        return NULL;
    }

    std::string identifier = m_currentToken.value;

    _getNextToken();
    if(scope->getLevel() == 0) {
        switch(m_currentToken.type) {
        case TOKEN_SEMICOLON:
            return _handleGlobalVariableDeclaration(type, identifier);
        case TOKEN_OPEN_PARENTHESES:
            return _handleFunctionDeclaration(type, identifier);
        default:
            return NULL;
        }
    } else {
        switch(m_currentToken.type) {
        case TOKEN_SEMICOLON:
            return _handleVariableDeclaration(type, identifier);
        default:
            return NULL;
        }
    }
}

Token Parser::_getNextToken()
{
    return m_currentToken = m_lexer->getNextToken();
    //    m_currentToken = m_lexer->getNextToken();
    //    printf("--debug: {{%d:[%c]:%s}}\n", m_currentToken.line, m_currentToken.type, m_currentToken.value.c_str());
    //    return m_currentToken;
}

BaseExpression *Parser::_handleVariableDeclaration(std::string type, std::string identifier)
{
    VariableDeclarationExpression *declaration = new VariableDeclarationExpression(type, identifier);
    scopes.back()->addVariable(identifier, declaration);
    return declaration;
}

BaseExpression *Parser::_handleGlobalVariableDeclaration(std::string type, std::string identifier)
{
    GlobalVariableExpression *declaration = new GlobalVariableExpression(type, identifier);
    scopes.back()->addVariable(identifier, declaration);
    return declaration;
}

BaseExpression *Parser::_handleFunctionDeclaration(std::string type, std::string identifier)
{
    _getNextToken();

    FunctionPrototype *prototype;
    BlockDefinition *body;
    FunctionDefinition *definition;
    std::vector<FunctionArgument *> args;

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
            prototype = new FunctionPrototype(identifier, args, type);
            break;
        case TOKEN_TYPE_SPECIFIER:
            args.push_back(_handleParameterDeclaratrion(m_currentToken.value));
            _getNextToken();
            break;
        case TOKEN_OPEN_BRACES:
            _getNextToken();
            scopes.push_back(new Scope(scopes.back()->getLevel() + 1));
            definition = new FunctionDefinition(prototype, _handleBlockDeclaration(*(new std::vector<BaseExpression *>()), scopes.back()));
            scopes.pop_back();
            return definition;
            break;
        default:
            return NULL;
        }
    }

    return new FunctionDefinition(prototype, body);
}

FunctionArgument *Parser::_handleParameterDeclaratrion(std::string type)
{
    _getNextToken();

    if (m_currentToken.type != TOKEN_IDENTIFIER) {
        return NULL;
    }

    return new FunctionArgument(type, m_currentToken.value);
}

BlockDefinition *Parser::_handleBlockDeclaration(const std::vector<BaseExpression *> &args, Scope *scope)
{
    std::vector<BaseExpression *> expressions;
    while (m_currentToken.type != TOKEN_EOF) {
        switch (m_currentToken.type) {
        case TOKEN_TYPE_SPECIFIER:
            expressions.push_back(_handleTypeSpecifier(m_currentToken.value, scope));
            break;
        case TOKEN_IF:
            expressions.push_back(_handleIfStatement());
            break;
            //        case TOKEN_STRUCT:
            //            break;
            //        case TOKEN_UNION:
            //            break;
            //        case TOKEN_ENUM:
            //            break;
        case TOKEN_CLOSE_BRACES:
            return new BlockDefinition(expressions);
        default:
            // TODO: push only valid expressions
            expressions.push_back(_handleExpression());
            if (m_currentToken.type) {

            }
            break;
        }
        _getNextToken();
    }

    return new BlockDefinition(expressions);
}

BaseExpression *Parser::_handleNumberExpression()
{
    BaseExpression *result = new NumberExpression(strtod(m_currentToken.value.c_str(), 0));
    _getNextToken();
    return result;
}

BaseExpression *Parser::_handleCallFunctionExpression(std::string identifier)
{
    bool endArgs = false;
    std::vector<BaseExpression *> args;
    while (!endArgs) {
        switch(m_currentToken.type) {
        case TOKEN_COMMA:
            _getNextToken();
            if (m_currentToken.type == TOKEN_COMMA) {
                // printf("Error on line %d: unexpected token ','", m_currentToken.line);
            }
            break;
        case TOKEN_CLOSE_PARENTHESES:
            endArgs = true;
            _getNextToken();
            break;
        case TOKEN_NUMBER:
            args.push_back(new NumberExpression(strtod(m_currentToken.value.c_str(), 0)));
            _getNextToken();
            break;
            //        case TOKEN_STRING:
            //        case TOKEN_CHAR:
            //        case TOKEN_IDENTIFIER:
        default:
            return NULL;
        }
    }
    return new CallExpression(identifier, args);
}

BaseExpression *Parser::_handleIdentifierExpression()
{
    std::string identifier = m_currentToken.value;
    _getNextToken();
    if (m_currentToken.type == TOKEN_OPEN_PARENTHESES) {
        return _handleCallFunctionExpression(identifier);
    } else {
        return new IdentifierExpression(identifier);
    }
}

BaseExpression *Parser::_handleParenthesesExpression()
{
    BaseExpression *expr;
    _getNextToken();
    expr = _handleExpression();

    if (m_currentToken.type != TOKEN_CLOSE_PARENTHESES) {
        printf("error\n");
        return 0;
    }
    _getNextToken();

    return expr;
}

BaseExpression *Parser::_handlePrimaryExpression()
{
    switch(m_currentToken.type) {
    //    case TOKEN_RETURN:

    case TOKEN_NUMBER:
        return _handleNumberExpression();
        break;
    case TOKEN_IDENTIFIER:
        return _handleIdentifierExpression();
        break;
    case TOKEN_OPEN_PARENTHESES:
        return _handleParenthesesExpression();
        break;
    default:
        return NULL;
    }
}

BaseExpression *Parser::_handleIfStatement()
{
    BaseExpression *expr, *ifBlock, *elseBlock = NULL;
    _getNextToken();
    if(m_currentToken.type != TOKEN_OPEN_PARENTHESES) {
        printf("'(' expected on line: %d", m_currentToken.line);
    }
    _getNextToken();
    expr = _handleExpression();

    if(m_currentToken.type != TOKEN_CLOSE_PARENTHESES) {
        printf("')' expected on line: %d", m_currentToken.line);
    }

    _getNextToken();

    if(m_currentToken.type == TOKEN_OPEN_BRACES) {
        _getNextToken();
        scopes.push_back(new Scope(scopes.back()->getLevel() + 1));
        ifBlock = _handleBlockDeclaration(*(new std::vector<BaseExpression *>()), scopes.back());
        scopes.pop_back();
    } else {
        ifBlock = _handleExpression();
    }

    return new IfStatmentExpression(expr, ifBlock, elseBlock);
}

BaseExpression *Parser::_handleExpression()
{
    BaseExpression *LHS;
    LHS = _handlePrimaryExpression();
    if (!LHS) {
        printf("nqkva gre6ka");
    }
    return _handleBinaryOperationExpression(0, LHS);
}

BaseExpression *Parser::_handleBinaryOperationExpression(int ExprPrec, BaseExpression *LHS)
{
    while (1) {
        int TokPrec = m_binopPrecedence[(char) m_currentToken.type];

        if (TokPrec < ExprPrec) {
            return LHS;
        }

        Token BinOp = m_currentToken;

        _getNextToken();

        // Parse the primary expression after the binary operator.
        BaseExpression *RHS = _handlePrimaryExpression();
        if (!RHS) {
            return LHS;
        }

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = m_binopPrecedence[(char) m_currentToken.type];
        if (TokPrec < NextPrec) {
            RHS = _handleBinaryOperationExpression(TokPrec+1, RHS);
            if (RHS == 0) {
                return 0;
            }
        }

        // Merge LHS/RHS.
        LHS = new BinaryExpression(BinOp.type, LHS, RHS);
    }
}

Parser::Parser(Lexer *lexer) : m_lexer(lexer), m_binopPrecedence()
{
    m_binopPrecedence[';'] = -1;
    m_binopPrecedence[')'] = -1;
    m_binopPrecedence['<'] = 10;
    m_binopPrecedence['>'] = 10;
    m_binopPrecedence['+'] = 20;
    m_binopPrecedence['-'] = 20;
    m_binopPrecedence['*'] = 40;
}

BlockDefinition *Parser::parse()
{
    _getNextToken();
    scopes.push_back(new Scope());
    return _handleBlockDeclaration(*(new std::vector<BaseExpression *>()), scopes.back());
    scopes.pop_back();
}
