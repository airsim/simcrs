// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  void DistributionManager::sell (AIRINV::AIRINV_Service& ioAIRINV_Service,
                                  const CRSCode_T& iCRSCode,
                                  const stdair::OutboundPath& iOutboundPath,
                                  const stdair::NbOfSeats_T& iPartySize) {

    try {

      // DEBUG
      /*STDAIR_LOG_DEBUG ("A booking will be made, reported by the "
                        << iCRSCode << " CRS, for the airline "
                        << iAirlineCode << ", and for " << iPartySize
                        << " passengers.");*/

      // Make a booking
      ioAIRINV_Service.sell (iPartySize);
    
      // DEBUG
      STDAIR_LOG_DEBUG ("The booking has been made");
      
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
