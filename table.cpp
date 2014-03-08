#include "table.h"

using namespace wait_for_it;

Table::Table()
{
}

void Table::push(std::string name, BaseExpression * expression)
{
    symbolTable.top()[name] = expression;
}

table Table::pop()
{
    return symbolTable.top();
}

table Table::beginScope()
{
//    std::map<std::string, BaseExpression *> *map = new std::map<std::string, BaseExpression *>();
    symbolTable.push(new std::map<std::string, BaseExpression *>());
}

void Table::endScope()
{
    symbolTable.pop();
}
