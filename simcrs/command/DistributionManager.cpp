// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Master_Service.hpp>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>

namespace SIMCRS {
  
  // ////////////////////////////////////////////////////////////////////
  void DistributionManager::
  getAvailability (AIRINV::AIRINV_Master_Service& ioAIRINV_Master_Service,
                   stdair::TravelSolutionList_T& ioTravelSolutionList) {
    for (stdair::TravelSolutionList_T::iterator itTS =
           ioTravelSolutionList.begin();
         itTS != ioTravelSolutionList.end(); ++itTS) {
      //stdair::TravelSolutionStruct& lCurrentTravelSolution = *itTS;
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DistributionManager::
  sell (AIRINV::AIRINV_Master_Service& ioAIRINV_Master_Service,
        const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::NbOfSeats_T& iPartySize) {

    try {
      const stdair::KeyList_T& lSegmentDateKeyList =
        iTravelSolution.getSegmentPath();
      const stdair::FareOptionStruct& lChosenFareOption =
        iTravelSolution.getChosenFareOption ();
      const stdair::ClassList_StringList_T& lClassPath =
        lChosenFareOption.getClassPath();
      stdair::ClassList_StringList_T::const_iterator itClassKeyList =
        lClassPath.begin();
      for (stdair::KeyList_T::const_iterator itKey= lSegmentDateKeyList.begin();
           itKey != lSegmentDateKeyList.end(); ++itKey, ++itClassKeyList) {
        const std::string& lSegmentDateKey = *itKey;

        // TODO: Removed this hardcode.
        std::ostringstream ostr;
        const stdair::ClassList_String_T& lClassList = *itClassKeyList;
        assert (lClassList.size() > 0);
        ostr << lClassList.at(0);
        const stdair::ClassCode_T lClassCode (ostr.str());
        ioAIRINV_Master_Service.sell (lSegmentDateKey, lClassCode, iPartySize);
      }
      
      // DEBUG
      // STDAIR_LOG_DEBUG ("The booking has been made");
      
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }
  
}
