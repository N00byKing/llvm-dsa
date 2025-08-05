#ifndef __STEENSGAARD_HH_
#define __STEENSGAARD_HH_
#include "dsa/DSGraph.h"
#include "dsa/DataStructure.h"
#include <llvm/IR/Analysis.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

namespace llvm
{
  /// SteensgaardsDataStructures - Analysis that computes a context-insensitive
  /// data structure graphs for the whole program.
  ///
  class SteensgaardDataStructures : public DataStructures, public AnalysisInfoMixin<SteensgaardDataStructures> {
    std::shared_ptr<DSGraph> ResultGraph;
    DataStructures* DS;
    void ResolveFunctionCall (const Function *F, const DSCallSite &Call,
                              DSNodeHandle &RetVal);
    bool runOnModuleInternal(Module &M);

  public:
    static inline AnalysisKey Key;
    using Result = std::shared_ptr<DSGraph>;
    std::shared_ptr<DSGraph> run(Module &M, ModuleAnalysisManager& MAM);
  
    /// getDSGraph - Return the data structure graph for the specified function.
    ///
    DSGraph* getOrCreateGraph(const Function *F) const {return ResultGraph.get();}
    DSGraph *getDSGraph(const Function& F) const override
    {
      return F.isDeclaration () ? NULL : getOrCreateGraph(&F);
    }
  
    bool hasDSGraph(const Function& F) const override {return !F.isDeclaration ();}

    void print(llvm::raw_ostream &O, const Module *M) const;
  };

  extern char &SteensgaardDataStructuresID;  
}


#endif
