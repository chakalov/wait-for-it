#ifndef DOUBLENUMBEREXPR_H
#define DOUBLENUMBEREXPR_H

#include "numberexpr.h"

namespace wait_for_it {

class DoubleNumberExpr: public NumberExpr {
    double m_val;
public:
    DoubleNumberExpr(double val);
    double getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // DOUBLENUMBEREXPR_H
