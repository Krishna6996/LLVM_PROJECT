#include "llvm/Transforms/Utils/SpecKernelSafetyPass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h" 

using namespace llvm;

PreservedAnalyses SpecKernelSafetyPass::run(Function &F, FunctionAnalysisManager &AM) {
    bool Modified = false;
    Module *M = F.getParent();
    
    // Get or create the fence intrinsic declaration
    Type *VoidTy = Type::getVoidTy(F.getContext());
    FunctionType *FenceTy = FunctionType::get(VoidTy, false);
    Function *FenceIntrinsic = cast<Function>(
        M->getOrInsertFunction("llvm.fence", FenceTy).getCallee()
    );
    
    for (auto &BB : F) {
        // Keep track of instructions that need fences
        std::vector<Instruction*> NeedsFence;
        
        for (auto &I : BB) {
            // Memory operations (loads and stores)
            if (isa<LoadInst>(I) || isa<StoreInst>(I)) {
                NeedsFence.push_back(&I);
            }
            // Function calls
            else if (auto *Call = dyn_cast<CallInst>(&I)) {
                // Don't add fence before another fence
                if (!Call->getCalledFunction() || 
                    Call->getCalledFunction()->getName() != "llvm.fence") {
                    NeedsFence.push_back(&I);
                }
            }
            // Branch instructions
            else if (isa<BranchInst>(I) || isa<SwitchInst>(I)) {
                NeedsFence.push_back(&I);
            }
            // Return instructions
            else if (isa<ReturnInst>(I)) {
                NeedsFence.push_back(&I);
            }
        }
        
        // Insert fences before identified instructions
        for (auto *I : NeedsFence) {
            IRBuilder<> Builder(I);
            Builder.CreateFence(AtomicOrdering::SequentiallyConsistent);
            Modified = true;
        }
    }
    
    return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

// Add debug logging if needed
#define DEBUG_TYPE "spec-kernel-safety"
STATISTIC(NumFencesInserted, "Number of fence instructions inserted");