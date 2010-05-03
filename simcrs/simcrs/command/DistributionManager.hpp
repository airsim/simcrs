#ifndef __SIMCRS_CMD_INDEXBUILDER_HPP
#define __SIMCRS_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>

// Forward declarations
namespace stdair {
  struct TravelSolutionStruct;
}

namespace SIMCRS {

  /** Command wrapping the travel request process. */
  class DistributionManager {
    friend class SIMCRS_Service;
  private:
    /** Get the availability for each travel solution in the list.. */
    static void getAvailability (const AIRINV::AIRINV_ServicePtr_Map_T&,
                          const CRSCode_T&, stdair::TravelSolutionList_T&);
    
    /** Register a booking (segment sell). */
    static void sell (const AIRINV::AIRINV_ServicePtr_Map_T&,
                      const CRSCode_T&, const stdair::TravelSolutionStruct&,
                      const stdair::NbOfSeats_T&);

  private:
    /** Constructors. */
    DistributionManager() {}
    DistributionManager(const DistributionManager&) {}
    /** Destructor. */
    ~DistributionManager() {}
  };

}
#endif // __SIMCRS_CMD_INDEXBUILDER_HPP
