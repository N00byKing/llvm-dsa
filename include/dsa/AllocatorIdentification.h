//===-- AllocatorIdentification.h - Identify alloc wrappers --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Identify malloc/free wrappers.
//===----------------------------------------------------------------------===//

#ifndef _ALLOCATORIDENTIFICATION_H
#define	_ALLOCATORIDENTIFICATION_H

#include <llvm/IR/PassManager.h>
#include <set>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/ADT/StringRef.h"

namespace llvm {
  class Function;
  class Module;
  class Instruction;

  class AllocIdentify : public AnalysisInfoMixin<AllocIdentify> {
  protected:
    std::set<std::string> allocators;
    std::set<std::string> deallocators;
    ModuleAnalysisManager* MAM;
    bool flowsFrom(Value *Dest,Value *Src);

  public:
    struct AllocIdentResult {
      std::set<std::string> allocators;
      std::set<std::string> deallocators;
      std::set<std::string>::iterator alloc_begin() {
      return allocators.begin();
      }
      std::set<std::string>::iterator alloc_end() {
        return allocators.end();
      }
      std::set<std::string>::iterator dealloc_begin() {
        return deallocators.begin();
      }
      std::set<std::string>::iterator dealloc_end() {
        return deallocators.end();
      }
    };
    using Result = AllocIdentResult;
    static inline llvm::AnalysisKey Key;
    Result run(llvm::Module& M, ModuleAnalysisManager& MAM);
  };
}

#endif	/* _ALLOCATORIDENTIFICATION_H */

