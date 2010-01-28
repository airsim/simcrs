// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// Standard Airline Object Model
#include <stdair/service/Logger.hpp>
// Simcrs
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>

namespace SIMCRS {

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext ()
    : _CRSCode (DEFAULT_CRS_CODE) {
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::
  SIMCRS_ServiceContext (const CRSCode_T& iCRSCode)
    : _CRSCode (iCRSCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::~SIMCRS_ServiceContext() {
  }
  
  // ////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMCRS_ServiceContext: " << std::endl
         << "CRS code: " << _CRSCode
         << std::endl;
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  AIRINV::AIRINV_Service& SIMCRS_ServiceContext::
  getAIRINV_Service (const stdair::AirlineCode_T& iAirlineCode) const {
    AIRINV_ServicePtr_Map_T::const_iterator itValueType =
      _airinvServiceMap.find (iAirlineCode);

    if (itValueType == _airinvServiceMap.end()) {
      STDAIR_LOG_ERROR ("Cannot find the inventory for the given airline: "
                        << iAirlineCode);
      assert (false);
    }

    return *itValueType->second;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_ServiceContext::
  addAIRINV_Service (const stdair::AirlineCode_T& iAirlineCode,
                     AIRINV_ServicePtr_T ioAIRINV_ServicePtr) {
    const bool hasInsertBeenSuccessful =
      _airinvServiceMap.insert (AIRINV_ServicePtr_Map_T::value_type (iAirlineCode, ioAIRINV_ServicePtr)).second;

    if (hasInsertBeenSuccessful == false) {
      STDAIR_LOG_ERROR ("Cannot add the inventory service handler for this airline: " << iAirlineCode);
      assert (false);
    }
  }
}
