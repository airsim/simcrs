// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/bom/Structure.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/YieldStore.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  YieldStore::YieldStore (const Key_T& iKey,
                          Structure_T& ioYieldStoreStructure)
    : YieldStoreContent (iKey), _structure (ioYieldStoreStructure) {
    init ();
  }

  // ////////////////////////////////////////////////////////////////////
  YieldStore::~YieldStore () {
  }

  // ////////////////////////////////////////////////////////////////////
  void YieldStore::init () {
  }

}
