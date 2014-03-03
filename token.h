#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace wait_for_it {

enum TokenType {
    TOKEN_INVALID = -1,
    TOKEN_EOF = 'E',
    TOKEN_NUMBER = 'N',
    TOKEN_IDENTIFIER = 'I',
    TOKEN_STRING = 'S',
    TOKEN_CHAR = 'C',
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
    TOKEN_SPECIAL_SYMBOL = 'Y',
    TOKEN_ELLIPSIS = '.'
};

struct Token {
    TokenType type;
    std::string value;
};

}

#endif // TOKEN_H
