//===- DataStructure.h - Build data structure graphs ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implement the LLVM data structure analysis library.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_ANALYSIS_DATA_STRUCTURE_H
#define LLVM_ANALYSIS_DATA_STRUCTURE_H

#include "dsa/DSCallGraph.h"
#include "dsa/svset.h"
#include "dsa/super_set.h"
#include "dsa/AddressTakenAnalysis.h"
#include "dsa/AllocatorIdentification.h"

#include "llvm/IR/DataLayout.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/ADT/EquivalenceClasses.h"

#include <llvm/IR/Analysis.h>
#include <llvm/IR/PassManager.h>
#include <map>
#include <memory>
#include <set>

namespace llvm {

class Type;
class Instruction;
class GlobalValue;
class DSGraph;
class DSCallSite;
class DSNode;
class DSNodeHandle;

class DataStructures {

  typedef std::map<const Function*, std::shared_ptr<DSGraph>> DSInfoTy;

  /// DataLayout, comes in handy
  const DataLayout* TD = 0;

  /// Pass to get Graphs from
  DataStructures* GraphSource = 0;

  /// Do we clone Graphs or steal them?
  bool Clone;

  /// do we reset the aux list to the func list?
  bool resetAuxCalls;

  /// Were are DSGraphs stolen by another pass?
  bool DSGraphsStolen = false;

  void buildGlobalECs(svset<const GlobalValue*>& ECGlobals);

  void eliminateUsesOfECGlobals(DSGraph& G, const svset<const GlobalValue*> &ECGlobals);

  // DSInfo, one graph for each function
  DSInfoTy DSInfo;

  // Name for printing
  const char* printname;

protected:

  /// The Globals Graph contains all information on the globals
  std::shared_ptr<DSGraph> GlobalsGraph = 0;

  /// GlobalECs - The equivalence classes for each global value that is merged
  /// with other global values in the DSGraphs.
  EquivalenceClasses<const GlobalValue*> GlobalECs;

  std::shared_ptr<SuperSet<Type*>> TypeSS;

  // Callgraph, as computed so far
  DSCallGraph callgraph;
  // List of all address taken functions.
  // This is used as target, of indirect calls for any indirect call site with  // incomplete callee node.
  std::vector<const Function*> GlobalFunctionList;

  void init(DataStructures* D, bool clone, bool useAuxCalls, bool copyGlobalAuxCalls, bool resetAux);
  void init(const DataLayout* T);

  void formGlobalECs();
  
  void cloneIntoGlobals(std::shared_ptr<DSGraph> const& G, unsigned cloneFlags);
  void cloneGlobalsInto(std::shared_ptr<DSGraph> const& G, unsigned cloneFlags);

  void restoreCorrectCallGraph();

  void formGlobalFunctionList();

  virtual ~DataStructures() {};

public:
  /// print - Print out the analysis results...
  ///
  void dumpCallGraph() const;

  /// handleTest - Handles various user-specified testing options.
  /// Returns true iff the user specified for us to test something.
  ///
  bool handleTest(llvm::raw_ostream &O, const Module *M) const;

  virtual bool hasDSGraph(const Function &F) const {
    return DSInfo.find(&F) != DSInfo.end();
  }

  /// getDSGraph - Return the data structure graph for the specified function.
  ///
  virtual std::shared_ptr<DSGraph> getDSGraph(const Function &F) const {
    DSInfoTy::const_iterator I = DSInfo.find(&F);
    assert(I != DSInfo.end() && "Function not in module!");
    return I->second;
  }

  void setDSGraph(const Function& F, std::shared_ptr<DSGraph> G) {
    DSInfo[&F] = G;
  }

  std::shared_ptr<DSGraph> getOrCreateGraph(const Function* F);

  std::shared_ptr<DSGraph> getGlobalsGraph() const { return GlobalsGraph; }
  void setGlobalsGraph(std::shared_ptr<DSGraph> GG) { GlobalsGraph = GG; }

  EquivalenceClasses<const GlobalValue*> &getGlobalECs() { return GlobalECs; }

  const DataLayout& getDataLayout() const { return *TD; }

  const DSCallGraph& getCallGraph() const { return callgraph; }

  SuperSet<Type*>& getTypeSS() const { return *TypeSS; }
  
  /// deleteValue/copyValue - Interfaces to update the DSGraphs in the program.
  /// These correspond to the interfaces defined in the AliasAnalysis class.
  void deleteValue(Value *V);
  void copyValue(Value *From, Value *To);
};

// LocalDataStructures - The analysis that computes the local data structure
// graphs for all of the functions in the program.
//
// FIXME: This should be a Function pass that can be USED by a Pass, and would
// be automatically preserved.  Until we can do that, this is a Pass.
//
class LocalDataStructures : public DataStructures, public AnalysisInfoMixin<LocalDataStructures> {
  std::set<Function*> addrAnalysis;
public:
  static inline AnalysisKey Key;

  struct Result { LocalDataStructures* res; };
  Result run(Module &M, ModuleAnalysisManager& MAM);
};

// StdLibDataStructures - This analysis recognizes common standard c library
// functions and generates graphs for them.
class StdLibDataStructures : public DataStructures, public AnalysisInfoMixin<StdLibDataStructures> {
  void eraseCallsTo(Function* F);
  void processRuntimeCheck (Module & M, std::string name, unsigned arg);
  void processFunction(int x, Function *F);
  AllocIdentify::AllocIdentResult AllocWrappersAnalysis;
public:
  static inline AnalysisKey Key;

  struct Result { StdLibDataStructures* res; };
  Result run(Module &M, ModuleAnalysisManager& MAM);
};

} // End llvm namespace

#endif
