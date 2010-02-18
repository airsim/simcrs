// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/OutboundPath.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  void DistributionManager::
  sell (const AIRINV::AIRINV_ServicePtr_Map_T& iAIRINV_ServiceMap,
        const CRSCode_T& iCRSCode,
        const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::NbOfSeats_T& iPartySize) {

    try {
      const stdair::OutboundPath& lOutboundPath =
        iTravelSolution.getOutboundPath();
      // TODO: optimise this part.
      // Browse the list/map of AIRINV_Services to see if the airline
      // takes part in the sale, and forward the sale procedure to
      // theses airlines.
      for (AIRINV::AIRINV_ServicePtr_Map_T::const_iterator itINV =
             iAIRINV_ServiceMap.begin();
           itINV != iAIRINV_ServiceMap.end(); ++itINV) {
        const stdair::AirlineCode_T lCurrentAirlineCode = itINV->first;

        const bool isAirlineFlown =
          lOutboundPath.isAirlineFlown (lCurrentAirlineCode);
        if (isAirlineFlown == true) {
          // DEBUG
          // STDAIR_LOG_DEBUG ("A booking will be made, reported by the "
          //                   << iCRSCode << " CRS, for the airline "
          //                   << lCurrentAirlineCode << ", and for "
          //                   << iPartySize << " passengers.");
          
          AIRINV::AIRINV_ServicePtr_T lAIRINV_Service_ptr = itINV->second;

          assert (lAIRINV_Service_ptr != NULL);
          lAIRINV_Service_ptr->sell (iTravelSolution, iPartySize);
        }
      }
    
      // DEBUG
      // STDAIR_LOG_DEBUG ("The booking has been made");
      
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
