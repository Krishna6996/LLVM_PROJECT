#ifndef LLVM_TRANSFORMS_UTILS_SPECKERNELSAFETYPASS_H
#define LLVM_TRANSFORMS_UTILS_SPECKERNELSAFETYPASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class SpecKernelSafetyPass : public PassInfoMixin<SpecKernelSafetyPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
    
    // Make this a required pass
    static bool isRequired() { return true; }
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_SPECKERNELSAFETYPASS_H