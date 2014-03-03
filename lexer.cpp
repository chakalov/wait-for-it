#include "lexer.h"

#include <llvm/ADT/StringSwitch.h>

using namespace wait_for_it;

Lexer::Lexer(char *filename)
{
    m_sourceStream = new std::ifstream(filename);
}

Lexer::~Lexer()
{
    delete m_sourceStream;
}

Token Lexer::getNextToken()
{
    Token token;
    char curr;

    curr = m_sourceStream->get();

    // remove delimiters and comments
    while(isspace(curr)) {
        curr = m_sourceStream->get();

        // Comments
        if (curr == '/') {
            char peek = m_sourceStream->peek();
            if (peek == '/') {
                while (curr != '\n' && curr != '\r' && curr != EOF) {
                    curr = m_sourceStream->get();
                }
            } else if (peek == '*') {
                while (curr != '*' && m_sourceStream->peek() != '/') {
                    curr = m_sourceStream->get();
                }
            }
        }
    }

    // check for EOF
    if (!m_sourceStream->good() || curr == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identifiers and keywords
    if (isalpha(curr)) {
        token.value += curr;
        while(isalnum(curr = m_sourceStream->get()) || curr == '_') {
            token.value += curr;
        }

        token.type = llvm::StringSwitch<TokenType>(llvm::StringRef(token.value))
                .Cases("typedef", "static", TOKEN_DECLARATION_SPECIFIER)
                .Case("void", TOKEN_TYPE_SPECIFIER)
                .Case("char", TOKEN_TYPE_SPECIFIER)
                .Case("bool", TOKEN_TYPE_SPECIFIER)
                .Cases("short", "int", TOKEN_TYPE_SPECIFIER)
                .Cases("long", "double", TOKEN_TYPE_SPECIFIER)
                .Cases("signed", "unsigned", TOKEN_TYPE_SPECIFIER)
                .Case("struct", TOKEN_STRUCT)
                .Case("union", TOKEN_UNION)
                .Case("enum", TOKEN_ENUM)
                .Case("case", TOKEN_CASE)
                .Case("default", TOKEN_DEFAULT_CASE)
                .Case("if", TOKEN_IF)
                .Case("else", TOKEN_ELSE)
                .Case("switch", TOKEN_SWITCH)
                .Case("while", TOKEN_WHILE)
                .Case("do", TOKEN_DO)
                .Case("for", TOKEN_FOR)
                .Case("continue", TOKEN_CONTINUE)
                .Case("break", TOKEN_BREAK)
                .Case("return", TOKEN_RETURN)
                .Default(TOKEN_IDENTIFIER);

        return token;
    }

    // Numbers
    if(isdigit(curr)) {
        token.type = TOKEN_NUMBER;
        do {
            token.value += curr;
            curr = m_sourceStream->get();
        } while(isdigit(curr) || curr == '.');

        return token;
    }

    // Special Symbols
    token.type = TOKEN_SPECIAL_SYMBOL;
    token.value += curr;

    // arithmetics
    if (curr == '*' || curr == '/' || curr == '%' || curr == '+' || curr == '-') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        }
        return token;
    }

    // binary
    if (curr == '<' && m_sourceStream->peek() == '<') {
        token.value += m_sourceStream->get();
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        }
        return token;
    }
    if (curr == '>' && m_sourceStream->peek() == '>') {
        token.value += m_sourceStream->get();
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        }
        return token;
    }
    if (curr == '&' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '|' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '^' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }

    // unary
    if (curr == '+' && m_sourceStream->peek() == '+') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '-' && m_sourceStream->peek() == '-') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '-' && m_sourceStream->peek() == '>') {
        token.value += m_sourceStream->get();
        return token;
    }

    // logical
    if (curr == '|' && m_sourceStream->peek() == '|') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '&' && m_sourceStream->peek() == '&') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '=' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '!' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '<' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }
    if (curr == '>' && m_sourceStream->peek() == '=') {
        token.value += m_sourceStream->get();
        return token;
    }

    return token;
}
