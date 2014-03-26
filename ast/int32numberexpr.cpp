#include "int32numberexpr.h"

using namespace wait_for_it;

Int32NumberExpr::Int32NumberExpr(long val)
    : m_val(val)
{

}

long Int32NumberExpr::getValue()
{
    return m_val;
}
