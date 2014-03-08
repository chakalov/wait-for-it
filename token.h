#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace wait_for_it {

enum TokenType {
    TOKEN_INVALID = -1,
    TOKEN_EOF = 'E',
    TOKEN_IDENTIFIER = 'I',

    // literals
    TOKEN_NUMBER = 'N',
    TOKEN_STRING = 'S',
    TOKEN_CHAR = 'C',

    // keywords
    TOKEN_DECLARATION_SPECIFIER = 'd',
    TOKEN_TYPE_SPECIFIER = 't',
    TOKEN_STRUCT = 'r',
    TOKEN_UNION = 'u',
    TOKEN_ENUM = 'e',
    TOKEN_CASE = 'c',
    TOKEN_DEFAULT_CASE = 'a',
    TOKEN_IF = 'f',
    TOKEN_ELSE = 'l',
    TOKEN_SWITCH = 't',
    TOKEN_WHILE = 'w',
    TOKEN_DO = 'o',
    TOKEN_FOR = 'F',
    TOKEN_CONTINUE = 'i',
    TOKEN_BREAK = 'b',
    TOKEN_RETURN = 'R',

    // symbols
    TOKEN_SPECIAL_SYMBOL = 'Y',
    TOKEN_OPEN_PARENTHESES = '(',
    TOKEN_CLOSE_PARENTHESES = ')',
    TOKEN_OPEN_BRACKETS = '[',
    TOKEN_CLOSE_BRACKETS= ']',
    TOKEN_OPEN_BRACES = '{',
    TOKEN_CLOSE_BRACES = '}',
    TOKEN_COMMA = ',',
    TOKEN_SEMICOLON = ';',
    TOKEN_LESS_THAN = '<',
    TOKEN_LESS_THAN_OR_EQUAL = '<=',
    TOKEN_GREATER_THAN = '>',
    TOKEN_GREATER_THAN_OR_EQUAL = '>=',
    TOKEN_PLUS = '+',
    TOKEN_MINUS = '-',
    TOKEN_ASTERISK = '*',
    TOKEN_ELLIPSIS = '.'
};

struct Token {
    TokenType type;
    std::string value;
};

}

#endif // TOKEN_H
