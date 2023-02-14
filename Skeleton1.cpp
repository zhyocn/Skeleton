#include "Skeleton.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

bool SkeletonModulePass::runOnModule(Module &M) {
//    LLVMContext &context = M.getContext();

    for (auto &F : M) {
        outs() << "FunctionName：" << F.getName() << "\n";
        if (!F.isDeclaration()) {
// 指令遍历来计算函数中控制流图中的边数
            unsigned edgeNum = 0;
            int BlockNum = 0;
            for (auto &B: F) {
                BlockNum++;
                outs() << "Block " << BlockNum << " has "<< B.size() << " instructions.\n";
                for (auto &I: B) {
                    if (auto *bran = dyn_cast<BranchInst>(&I)) {
// 普通的分支指令边数为2
                        edgeNum = edgeNum+ bran->isConditional() ? 2 : 1;
                    } else if (auto *swit = dyn_cast<SwitchInst>(&I)) {
// switch指令边数为case数加1
                        edgeNum = edgeNum+ swit->getNumCases() + 1;
                    }
                }
            }
            outs() << "Function " << F.getName() << " has " << edgeNum << " edges." << "\n";
        }
    }
    outs() << "The module has : " << M.size() << " functions. \n";
    return false;
}

char SkeletonFunctionPass::ID = 0;
char SkeletonModulePass::ID = 0;

//static RegisterPass<SkeletonFunctionPass>
//        X("SkeletonPassDemo", "Skeleton Pass Demo", false,false);

static RegisterPass<SkeletonModulePass>
        X("SkeletonPassDemo", "Skeleton Pass Demo", false,false);
