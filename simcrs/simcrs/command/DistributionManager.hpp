#ifndef __SIMCRS_CMD_INDEXBUILDER_HPP
#define __SIMCRS_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>

// Forward declarations
namespace AIRINV {
  class AIRINV_Service;
}

namespace SIMCRS {

  /** Command wrapping the travel request process. */
  class DistributionManager {
    friend class SIMCRS_Service;
  private:

    /** Register a booking (segment sell). */
    static void sell (AIRINV::AIRINV_Service&,
                      const CRSCode_T&, const AirlineCode_T&,
                      const PartySize_T&);

  private:
    /** Constructors. */
    DistributionManager() {}
    DistributionManager(const DistributionManager&) {}
    /** Destructor. */
    ~DistributionManager() {}
  };

}
#endif // __SIMCRS_CMD_INDEXBUILDER_HPP
