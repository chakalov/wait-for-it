#include "lexer.h"

#include <llvm/ADT/StringSwitch.h>

using namespace wait_for_it;

Lexer::Lexer(const char *filename)
{
    m_currentLineNumber = 1;
    m_sourceStream = new std::ifstream(filename);
}

Lexer::~Lexer()
{
    delete m_sourceStream;
}

Token Lexer::getNextToken()
{
    Token token;
    char curr, peek;

    curr = m_sourceStream->get();

    // remove delimiters and comments
    while(isspace(curr)) {
        if (curr == '\n') {
            m_currentLineNumber++;
            if (m_sourceStream->peek() == '\r') {
                m_sourceStream->get();
            }
        } else if (curr == '\n') {
            m_currentLineNumber++;
        }

        curr = m_sourceStream->get();

        // Comments
        if (curr == '/') {
            peek = m_sourceStream->peek();
            if (peek == '/') {
                while (curr != '\n' && curr != '\r' && curr != EOF) {
                    curr = m_sourceStream->get();
                }
                m_currentLineNumber++;
            } else if (peek == '*') {
                while (curr != '*' && m_sourceStream->peek() != '/') {
                    curr = m_sourceStream->get();
                    if (curr == '\n') {
                        m_currentLineNumber++;
                        if (m_sourceStream->peek() == '\r') {
                            m_sourceStream->get();
                        }
                    } else if (curr == '\n') {
                        m_currentLineNumber++;
                    }

                }
            }
        }
    }

    token.line = m_currentLineNumber;

    // check for EOF
    if (!m_sourceStream->good() || curr == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identifiers and keywords
    if (isalpha(curr)) {
        token.value += curr;
        peek = m_sourceStream->peek();
        while(isalnum(peek) || peek == '_') {
            token.value += m_sourceStream->get();
            peek = m_sourceStream->peek();
        };

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
                .Case("infinite", TOKEN_INFINITE)
                .Default(TOKEN_IDENTIFIER);

        return token;
    }

    // Numbers
    if(isdigit(curr)) {
        token.type = TOKEN_INTEGER_NUMBER;

        token.value += curr;
        peek = m_sourceStream->peek();
        while(isdigit(peek)) {
            token.value += m_sourceStream->get();
            peek = m_sourceStream->peek();
        };
        if (peek == '.') {
            token.type = TOKEN_DOUBLE_NUMBER;
            token.value += m_sourceStream->get();
            peek = m_sourceStream->peek();
        }
        while(isdigit(peek)) {
            token.value += m_sourceStream->get();
            peek = m_sourceStream->peek();
        };

        return token;
    }

    // Parentheses
    if (curr == '(') {
        token.type = TOKEN_OPEN_PARENTHESES;
        token.value += curr;
        return token;
    }
    if (curr == ')') {
        token.type = TOKEN_CLOSE_PARENTHESES;
        token.value += curr;
        return token;
    }

    // Brackets
    if (curr == '[') {
        token.type = TOKEN_OPEN_BRACKETS ;
        token.value += curr;
        return token;
    }
    if (curr == ']') {
        token.type = TOKEN_CLOSE_BRACKETS ;
        token.value += curr;
        return token;
    }

    //Braces
    if (curr == '{') {
        token.type = TOKEN_OPEN_BRACES ;
        token.value += curr;
        return token;
    }
    if (curr == '}') {
        token.type = TOKEN_CLOSE_BRACES ;
        token.value += curr;
        return token;
    }

    // Comma
    if (curr == ',') {
        token.type = TOKEN_COMMA;
        token.value += curr;
        return token;
    }

    // Semicolon
    if (curr == ';') {
        token.type = TOKEN_SEMICOLON;
        token.value += curr;
        return token;
    }

    // String literals
    if (curr == '"') {
        token.type = TOKEN_STRING;
        do {
            curr = m_sourceStream->get();
            peek = m_sourceStream->peek();

            if (curr != '\\') {
                token.value += curr;
            } else if (peek == 'n') {
                token.value += '\n';

                curr = m_sourceStream->get();
                peek = m_sourceStream->peek();
            }
        } while(peek != '"' || curr == '\\');
        m_sourceStream->get();

        return token;
    }

    // Char literals
    if (curr == '\'') {
        token.type = TOKEN_CHAR;
        token.value += m_sourceStream->get();
        m_sourceStream->get();

        return token;
    }

    // Special Symbols
    token.type = TOKEN_SPECIAL_SYMBOL;
    token.value += curr;

    // arithmetics
    if (curr == '=') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_EQUALS;
        }
        return token;
    }

    if (curr == '*') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_ASTERISK;
        }
        return token;
    }
    if (curr == '/') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        }
        return token;
    }
    if (curr == '%') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        }
        return token;
    }
    if (curr == '+') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_PLUS;
        }
        return token;
    }
    if (curr == '-') {
        if (m_sourceStream->peek() == '=') {
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_MINUS;
        }
        return token;
    }

    // binary
    if (curr == '<' && m_sourceStream->peek() == '<') {
        token.value += m_sourceStream->get();
        if (m_sourceStream->peek() == '=') {
            token.type = TOKEN_LESS_THAN_OR_EQUAL;
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_LESS_THAN;
        }
        return token;
    }
    if (curr == '>' && m_sourceStream->peek() == '>') {
        token.value += m_sourceStream->get();
        if (m_sourceStream->peek() == '=') {
            token.type = TOKEN_GREATER_THAN_OR_EQUAL;
            token.value += m_sourceStream->get();
        } else {
            token.type = TOKEN_GREATER_THAN;
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
