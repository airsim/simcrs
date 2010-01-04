// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/service/Logger.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  void DistributionManager::sell (AIRINV::AIRINV_Service& ioAIRINV_Service,
                                  const CRSCode_T& iCRSCode,
                                  const stdair::AirlineCode_T& iAirlineCode,
                                  const stdair::PartySize_T& iPartySize) {

    try {

      // DEBUG
      SIMCRS_LOG_DEBUG ("A booking will be made, reported by the "
                        << iCRSCode << " CRS, for the airline "
                        << iAirlineCode << ", and for " << iPartySize
                        << " passengers.");

      // Make a booking
      ioAIRINV_Service.sell (iPartySize);
    
      // DEBUG
      SIMCRS_LOG_DEBUG ("The booking has been made");
      
    } catch (const std::exception& lStdError) {
      SIMCRS_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
