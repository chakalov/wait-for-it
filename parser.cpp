#include "parser.h"

using namespace wait_for_it;

Token wait_for_it::Parser::_getNextToken()
{
    return m_currToken = m_lexer->getNextToken();
}

BaseExpression *Parser::_parseNumberExpression()
{
    BaseExpression *expr = new NumberExpression(strtod(m_currToken.value.c_str(), 0));
    _getNextToken();
    return expr;
}

BaseExpression *Parser::_parseParenthesisExpression()
{
    _getNextToken(); // eats the (
    BaseExpression *expr = _parseExpression();
    if (!expr) return 0;

    if (m_currToken.type != TOKEN_CLOSE_PARENTHESIS) {
        return Error("expected ')'");
    }
    _getNextToken();  // eats the ).
    return expr;
}

BaseExpression *Parser::_parseIdentifierExpression()
{
    std::string IdName = m_currToken.value;

    _getNextToken();  // eat identifier.

    // Simple variable ref.
    if (m_currToken.value[0] != '(') {
        return new VariableExpression(IdName);
    }

    // Call.
    _getNextToken();  // eat (

    std::vector<BaseExpression*> Args;

    if (m_currToken.value[0] != ')') {
        while (1) {
            BaseExpression *Arg = _parseExpression();

            if (!Arg) {
                return 0;
            }
            Args.push_back(Arg);

            if (m_currToken.type = TOKEN_CLOSE_PARENTHESIS) {
                break;
            }

            if (m_currToken.type != TOKEN_COMMA) {
                return Error("Expected ')' or ',' in argument list");
            }

            _getNextToken();
        }
    }

    // Eat the ')'.
    _getNextToken();

    return new CallExpression(IdName, Args);
}

BaseExpression *Parser::_parsePrimary()
{
    switch (m_currToken.type) {
    default:
        return Error("unknown token when expecting an expression");
    case TOKEN_IDENTIFIER:
        return _parseIdentifierExpression();
    case TOKEN_NUMBER:
        return _parseNumberExpression();
    case TOKEN_OPEN_PARENTHESIS:
        return _parseParenthesisExpression();
    }
}

BaseExpression *Parser::_parseExpression()
{
    BaseExpression *LHS = _parsePrimary();
    if (!LHS) {
        return 0;
    }

    return _parseBinaryOperationExpression(0, LHS);
}

BaseExpression *Parser::_parseBinaryOperationExpression(int expressionPrecedence, BaseExpression *lhs)
{
    while (1) {
        int tokenPrecedence = m_binaryOperatorPrecedence[m_currToken.type];

        if (tokenPrecedence < expressionPrecedence)
            return lhs;

        int binaryOperator = m_currToken.type;
        _getNextToken();  // eat binop

        BaseExpression *rhs = _parsePrimary();
        if (!rhs) {
            return 0;
        }

        int nextPrecedence = m_binaryOperatorPrecedence[m_currToken.type];
        if (tokenPrecedence < nextPrecedence) {
            rhs = _parseBinaryOperationExpression(tokenPrecedence+1, rhs);
            if (rhs == 0) {
                return 0;
            }
        }

        // Merge LHS/RHS.
        lhs = new BinaryExpression(binaryOperator, lhs, rhs);
    }
}

Parser::Parser(Lexer *lexer) : m_lexer(lexer)
{
    m_binaryOperatorPrecedence[TOKEN_LESS_THAN] = 10;
    m_binaryOperatorPrecedence[TOKEN_PLUS] = 20;
    m_binaryOperatorPrecedence[TOKEN_MINUS] = 20;
    m_binaryOperatorPrecedence[TOKEN_ASTERISK] = 40;
}
