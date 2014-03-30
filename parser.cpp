#include "parser.h"
#include "scope.h"

using namespace wait_for_it;

VariableExpr *Parser::findVariable(std::string name)
{
    VariableExpr *ret = NULL;
    for(std::vector<Scope *>::iterator it = scopes.begin(); it != scopes.end(); it++) {
        ret = (*it)->getVariable(name);
        if (ret != NULL) {
            return ret;
        }
    }
    // error
    return NULL;
}

Expr *Parser::_handleTypeSpecifier(std::string type, Scope *scope)
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

LocalVariableExpr *Parser::_handleVariableDeclaration(std::string type, std::string identifier)
{
    LocalVariableExpr *declaration = new LocalVariableExpr(type, identifier);
    scopes.back()->addVariable(identifier, declaration);
    return declaration;
}

GlobalVariableExpr *Parser::_handleGlobalVariableDeclaration(std::string type, std::string identifier)
{
    GlobalVariableExpr *declaration = new GlobalVariableExpr(type, identifier);
    scopes.back()->addVariable(identifier, declaration);
    return declaration;
}

FunctionExpr *Parser::_handleFunctionDeclaration(std::string type, std::string identifier)
{
    _getNextToken();

    FunctionPrototypeExpr *prototype;
    BlockExpr *body;
    FunctionExpr *definition;
    std::vector<FunctionArgumentExpr *> args;

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
            prototype = new FunctionPrototypeExpr(identifier, args, type);
            break;
        case TOKEN_TYPE_SPECIFIER:
            args.push_back(_handleParameterDeclaratrion(m_currentToken.value));
            _getNextToken();
            break;
        case TOKEN_OPEN_BRACES:
            _getNextToken();
            scopes.push_back(new Scope(scopes.back()->getLevel() + 1));
            if (args.size()) {
                for(std::vector<FunctionArgumentExpr *>::iterator it = args.begin(); it != args.end(); it++) {
                    scopes.back()->addVariable((*it)->getName(), (*it));
                }
            }
            definition = new FunctionExpr(prototype, _handleBlockDeclaration(*(new std::vector<Expr *>()), scopes.back()));
            scopes.pop_back();
            return definition;
            break;
        default:
            return NULL;
        }
    }

    return new FunctionExpr(prototype, body);
}

FunctionArgumentExpr *Parser::_handleParameterDeclaratrion(std::string type)
{
    _getNextToken();

    if (m_currentToken.type != TOKEN_IDENTIFIER) {
        return NULL;
    }

    return new FunctionArgumentExpr(type, m_currentToken.value);
}

BlockExpr *Parser::_handleBlockDeclaration(const std::vector<Expr *> &args, Scope *scope)
{
    std::vector<Expr *> expressions;
    while (m_currentToken.type != TOKEN_EOF) {
        switch (m_currentToken.type) {
        case TOKEN_TYPE_SPECIFIER:
            expressions.push_back(_handleTypeSpecifier(m_currentToken.value, scope));
            _getNextToken();
            break;
        case TOKEN_IF:
            expressions.push_back(_handleIfStatement());
            break;
        case TOKEN_WHILE:
            expressions.push_back(_handleWhileLoop());
            _getNextToken();
            break;
            //        case TOKEN_STRUCT:
            //            break;
            //        case TOKEN_UNION:
            //            break;
            //        case TOKEN_ENUM:
            //            break;
        case TOKEN_CLOSE_BRACES:
            return new BlockExpr(expressions);
        default:
            // TODO: push only valid expressions
            expressions.push_back(_handleExpression());
            if (m_currentToken.type) {

            }
            _getNextToken();
            break;
        }
    }

    return new BlockExpr(expressions);
}

NumberExpr *Parser::_handleNumberExpression()
{
    NumberExpr *result = NULL;
    if (m_currentToken.type == TOKEN_DOUBLE_NUMBER) {
        result = new DoubleNumberExpr(strtod(m_currentToken.value.c_str(), 0));
    } else if (m_currentToken.type == TOKEN_INTEGER_NUMBER) {
        result = new Int32NumberExpr(strtol(m_currentToken.value.c_str(), 0, 10));
    }
    _getNextToken();
    return result;
}

CallExpr *Parser::_handleCallFunctionExpression(std::string identifier)
{
    bool endArgs = false;
    std::vector<Expr *> args;
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
        case TOKEN_STRING:
            args.push_back(new StringExpr(m_currentToken.value));
            _getNextToken();
            break;
            //        case TOKEN_CHAR:
            //        case TOKEN_IDENTIFIER:
        default:
            args.push_back(_handleExpression());
            break;
        }
    }
    return new CallExpr(identifier, args);
}

Expr *Parser::_handleIdentifierExpression()
{
    std::string identifier = m_currentToken.value;
    _getNextToken();
    if (m_currentToken.type == TOKEN_OPEN_PARENTHESES) {
        _getNextToken();
        return _handleCallFunctionExpression(identifier);
    } else {
        VariableExpr *var = findVariable(identifier);
        return new IdentifierExpr(var);
    }
}

Expr *Parser::_handleParenthesesExpression()
{
    Expr *expr;
    _getNextToken();
    expr = _handleExpression();

    if (m_currentToken.type != TOKEN_CLOSE_PARENTHESES) {
        printf("error\n");
        return 0;
    }
    _getNextToken();

    return expr;
}

Expr *Parser::_handlePrimaryExpression()
{
    switch(m_currentToken.type) {
    //    case TOKEN_RETURN:

    case TOKEN_INTEGER_NUMBER:
    case TOKEN_DOUBLE_NUMBER:
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

IfExpr *Parser::_handleIfStatement()
{
    Expr *expr;
    BlockExpr *ifBlock, *elseBlock = NULL;
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
        ifBlock = _handleBlockDeclaration(*(new std::vector<Expr *>()), scopes.back());
        scopes.pop_back();
    } else {
        std::vector<Expr *> expressions;
        expressions.push_back(_handleExpression());
        ifBlock = new BlockExpr(expressions);
    }

    _getNextToken();

    if (m_currentToken.type == TOKEN_ELSE) {
        _getNextToken();

        if(m_currentToken.type == TOKEN_OPEN_BRACES) {
            _getNextToken();
            scopes.push_back(new Scope(scopes.back()->getLevel() + 1));
            elseBlock = _handleBlockDeclaration(*(new std::vector<Expr *>()), scopes.back());
            scopes.pop_back();
        } else {
            std::vector<Expr *> expressions;
            expressions.push_back(_handleExpression());
            elseBlock = new BlockExpr(expressions);
        }
        _getNextToken();
    }

    return new IfExpr(expr, ifBlock, elseBlock);
}

WhileExpr *Parser::_handleWhileLoop()
{
    Expr *expr;
    BlockExpr *whileBlock;

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
        whileBlock = _handleBlockDeclaration(*(new std::vector<Expr *>()), scopes.back());
        scopes.pop_back();
    } else {
        std::vector<Expr *> expressions;
        expressions.push_back(_handleExpression());
        whileBlock = new BlockExpr(expressions);
    }

    return new WhileExpr(expr, whileBlock);
}

Expr *Parser::_handleExpression()
{
    Expr *LHS;
    LHS = _handlePrimaryExpression();
    if (!LHS) {
        printf("nqkva gre6ka");
    }
    return _handleBinaryOperationExpression(0, LHS);
}

Expr *Parser::_handleBinaryOperationExpression(int ExprPrec, Expr *LHS)
{
    while (1) {
        int TokPrec = m_binopPrecedence[(char) m_currentToken.type];

        if (TokPrec < ExprPrec) {
            return LHS;
        }

        Token BinOp = m_currentToken;

        _getNextToken();

        // Parse the primary expression after the binary operator.
        Expr *RHS = _handlePrimaryExpression();
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
        LHS = new BinaryExpr(BinOp.type, LHS, RHS);
    }
}

Parser::Parser(Lexer *lexer) : m_lexer(lexer), m_binopPrecedence()
{
    m_binopPrecedence[','] = -1;
    m_binopPrecedence[';'] = -1;
    m_binopPrecedence[')'] = -1;
    m_binopPrecedence['='] = 2;
    m_binopPrecedence['<'] = 10;
    m_binopPrecedence['>'] = 10;
    m_binopPrecedence['+'] = 20;
    m_binopPrecedence['-'] = 20;
    m_binopPrecedence['*'] = 40;
}

BlockExpr *Parser::parse()
{
    _getNextToken();
    scopes.push_back(new Scope());
    return _handleBlockDeclaration(*(new std::vector<Expr *>()), scopes.back());
    scopes.pop_back();
}
