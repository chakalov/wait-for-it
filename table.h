#ifndef TABLE_H
#define TABLE_H

#include "ast.h"
#include <stack>
#include <map>
#include <string>

namespace wait_for_it {

typedef std::stack<std::map<std::string, BaseExpression *>> table;

class Table
{
    table symbolTable;
public:
    Table();
    void push(std::string name, BaseExpression *expression);
    table pop();

};

}

#endif // TABLE_H
