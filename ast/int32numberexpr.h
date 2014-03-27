#ifndef INT32NUMBEREXPR_H
#define INT32NUMBEREXPR_H

#include "numberexpr.h"

namespace wait_for_it {

class Int32NumberExpr: public Expr {
    long m_val;
public:
    Int32NumberExpr(long val);
    long getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // INT32NUMBEREXPR_H
