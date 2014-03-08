#include "parser.h"

using namespace wait_for_it;

Token Parser::_getNextToken()
{
    return m_currentToken = m_lexer->getNextToken();
}

BaseExpression *Parser::_handleDeclarator(std::string declaration, std::string type, int pointer)
{
    _getNextToken();
    switch (m_currentToken.type) {
    case TOKEN_ASTERISK:
        return _handleDeclarator(declaration, type, pointer + 1);
    case TOKEN_IDENTIFIER:
        return new VariableExpression(m_currentToken.value);
    default:
        break;
    }
}

BaseExpression *Parser::_handleDeclarationSpecifier(std::string declaration)
{
    if (declaration.compare("typedef") != 0
            && declaration.compare("static") != 0) {
        return NULL;
    } else {
//        Token currentToken = m_lexer->getNextToken();
//        return _handleTypeSpecifier(name);
        return NULL;
    }
}

BaseExpression *Parser::_handleTypeSpecifier(std::string declaration, std::string type)
{
    if (type.compare("void") != 0
            && type.compare("char") != 0
            && type.compare("bool") != 0
            && type.compare("short") != 0
            && type.compare("int") != 0
            && type.compare("long") != 0
            && type.compare("double") != 0
            && type.compare("signed") != 0
            && type.compare("unsigned") != 0) {
        return NULL;
    } else {
        return _handleDeclarator(declaration, type);
    }
}

Parser::Parser(Lexer *lexer) : m_lexer(lexer)
{
}

void Parser::parse()
{
    _getNextToken();
    while (m_currentToken.type != TOKEN_EOF) {
        switch (m_currentToken.type) {
        case TOKEN_DECLARATION_SPECIFIER:
            _handleDeclarationSpecifier(m_currentToken.value);
            break;
        case TOKEN_TYPE_SPECIFIER:
            _handleTypeSpecifier("", m_currentToken.value);
            break;
        case TOKEN_STRUCT:
            break;
        case TOKEN_UNION:
            break;
        case TOKEN_ENUM:
            break;
        default:
            break;
        }
        _getNextToken();
    }
}
