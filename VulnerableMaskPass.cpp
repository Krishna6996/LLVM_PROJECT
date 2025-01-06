#include "llvm/Transforms/Utils/VulnerableMaskPass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "/Users/krishnagoel/llvm-project/llvm/include/llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

PreservedAnalyses VulnerableMaskPass::run(Function &F, FunctionAnalysisManager &AM) {
    bool Modified = false;
    
    for (auto &BB : F) {
        for (auto &I : BB) {
            // Handle binary operations
            if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
                IRBuilder<> Builder(&I);
                
                // Mask operands with OR 0
                for (unsigned i = 0; i < BinOp->getNumOperands(); ++i) {
                    Value *Op = BinOp->getOperand(i);
                    
                    // Only mask integer operands
                    if (Op->getType()->isIntegerTy()) {
                        // Create OR operation with constant 0
                        Value *MaskedOp = Builder.CreateOr(
                            Op,
                            ConstantInt::get(Op->getType(), 0),
                            "masked"
                        );
                        
                        // Replace original operand with masked version
                        BinOp->setOperand(i, MaskedOp);
                        Modified = true;
                    }
                }
            }
            
            // Handle memory operations (loads and stores)
            else if (auto *MemOp = dyn_cast<LoadInst>(&I)) {
                if (MemOp->getPointerOperand()->getType()->isIntegerTy()) {
                    IRBuilder<> Builder(&I);
                    Value *MaskedLoad = Builder.CreateOr(
                        MemOp,
                        ConstantInt::get(MemOp->getType(), 0),
                        "masked_load"
                    );
                    MemOp->replaceAllUsesWith(MaskedLoad);
                    Modified = true;
                }
            }
            else if (auto *MemOp = dyn_cast<StoreInst>(&I)) {
                if (MemOp->getValueOperand()->getType()->isIntegerTy()) {
                    IRBuilder<> Builder(&I);
                    Value *MaskedStore = Builder.CreateOr(
                        MemOp->getValueOperand(),
                        ConstantInt::get(MemOp->getValueOperand()->getType(), 0),
                        "masked_store"
                    );
                    MemOp->setOperand(0, MaskedStore);
                    Modified = true;
                }
            }
        }
    }
    
    return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
} 