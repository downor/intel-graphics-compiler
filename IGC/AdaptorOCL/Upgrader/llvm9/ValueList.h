//===-- Bitcode/Reader/ValueList.h - Number values --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class gives values and types Unique ID's.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_BITCODE_READER_VALUELIST_H
#define LLVM_LIB_BITCODE_READER_VALUELIST_H

#include "llvm/IR/ValueHandle.h"
#include <utility>
#include <vector>
#include "Probe.h"

namespace llvm {

class Constant;
class LLVMContext;
class Type;
class Value;

class BitcodeReaderValueList {
  std::vector<WeakTrackingVH> ValuePtrs;

  /// Struct containing fully-specified copies of the type of each
  /// value. When pointers are opaque, this will be contain non-opaque
  /// variants so that restructuring instructions can determine their
  /// type correctly even if being loaded from old bitcode where some
  /// types are implicit.
  std::vector<Type *> FullTypes;

  /// As we resolve forward-referenced constants, we add information about them
  /// to this vector.  This allows us to resolve them in bulk instead of
  /// resolving each reference at a time.  See the code in
  /// ResolveConstantForwardRefs for more information about this.
  ///
  /// The key of this vector is the placeholder constant, the value is the slot
  /// number that holds the resolved value.
  using ResolveConstantsTy = std::vector<std::pair<Constant *, unsigned>>;
  ResolveConstantsTy ResolveConstants;
  LLVMContext &Context;

  /// Maximum number of valid references. Forward references exceeding the
  /// maximum must be invalid.
  unsigned RefsUpperBound;

public:
  BitcodeReaderValueList(LLVMContext &C, size_t RefsUpperBound)
      : Context(C),
        RefsUpperBound(std::min((size_t)std::numeric_limits<unsigned>::max(),
                                RefsUpperBound)) {}

  ~BitcodeReaderValueList() {
    IGC_ASSERT(ResolveConstants.empty() && "Constants not resolved?");
  }

  // vector compatibility methods
  unsigned size() const { return ValuePtrs.size(); }
  void resize(unsigned N) {
    ValuePtrs.resize(N);
    FullTypes.resize(N);
  }
  void push_back(Value *V, Type *Ty) {
    ValuePtrs.emplace_back(V);
    FullTypes.emplace_back(Ty);
  }

  void clear() {
    IGC_ASSERT(ResolveConstants.empty() && "Constants not resolved?");
    ValuePtrs.clear();
    FullTypes.clear();
  }

  Value *operator[](unsigned i) const {
    IGC_ASSERT(i < ValuePtrs.size());
    return ValuePtrs[i];
  }

  Value *back() const { return ValuePtrs.back(); }
  void pop_back() {
    ValuePtrs.pop_back();
    FullTypes.pop_back();
  }
  bool empty() const { return ValuePtrs.empty(); }

  void shrinkTo(unsigned N) {
    IGC_ASSERT(N <= size() && "Invalid shrinkTo request!");
    ValuePtrs.resize(N);
    FullTypes.resize(N);
  }

  Constant *getConstantFwdRef(unsigned Idx, Type *Ty);
  Value *getValueFwdRef(unsigned Idx, Type *Ty, Type **FullTy = nullptr);

  void assignValue(Value *V, unsigned Idx, Type *FullTy);

  /// Once all constants are read, this method bulk resolves any forward
  /// references.
  void resolveConstantForwardRefs();
};

} // end namespace llvm

#endif // LLVM_LIB_BITCODE_READER_VALUELIST_H
