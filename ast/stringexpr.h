#ifndef STRINGEXPR_H
#define STRINGEXPR_H

#include "literalexpr.h"
#include <string>

namespace wait_for_it {

class StringExpr: public LiteralExpr {
    std::string m_val;
public:
    StringExpr(std::string val);
    std::string getValue();
    virtual llvm::Value *emitCode(llvm::IRBuilder<>& builder, llvm::Module &module);
};

}

#endif // STRINGEXPR_H
