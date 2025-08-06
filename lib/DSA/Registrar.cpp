#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/OptimizationLevel.h>

#include "dsa/AddressTakenAnalysis.h"
#include "dsa/AllocatorIdentification.h"
#include "dsa/DataStructure.h"
#include "dsa/Steensgaard.hh"

using namespace llvm;

namespace {
  void MAMHook(ModuleAnalysisManager &MAM) {
      MAM.registerPass([&] { return SteensgaardDataStructures(); });
      MAM.registerPass([&] { return StdLibDataStructures(); });
      MAM.registerPass([&] { return LocalDataStructures(); });
      MAM.registerPass([&] { return AddressTakenAnalysis(); });
      MAM.registerPass([&] { return AllocIdentify(); });
  };

  void PBHook(PassBuilder &PB) {
      PB.registerAnalysisRegistrationCallback(MAMHook);
  }
}

llvm::PassPluginLibraryInfo getDSAInfo() {
    return {LLVM_PLUGIN_API_VERSION, "DataStructureAnalysis",
            LLVM_VERSION_STRING, PBHook};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getDSAInfo();
}
