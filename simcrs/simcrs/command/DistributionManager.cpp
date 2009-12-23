// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <vector>
#include <exception>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/service/Logger.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  void DistributionManager::sell (const CRSCode_T& iCRSCode,
                                  const AirlineCode_T& iAirlineCode,
                                  const PartySize_T& iPartySize) {

    try {

      // DEBUG
      SIMCRS_LOG_DEBUG ("A booking has been made by the " << iCRSCode
                        << " CRS, for the airline "
                        << iAirlineCode << " for " << iPartySize
                        << " passengers.");
    
    } catch (const std::exception& lStdError) {
      SIMCRS_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
