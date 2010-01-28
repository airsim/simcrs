// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Standard Airline Object Model
#include <stdair/STDAIR_Service.hpp>
#include <stdair/service/Logger.hpp>
// Simcrs
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext () : _CRSCode (DEFAULT_CRS_CODE) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const SIMCRS_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const CRSCode_T& iCRSCode)
    : _CRSCode (iCRSCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::~SIMCRS_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_Service& SIMCRS_ServiceContext::getSTDAIR_Service() const {
    assert (_stdairService != NULL);
    return *_stdairService;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_ServiceContext::
  setSTDAIR_Service (stdair::STDAIR_Service& ioSTDAIR_Service) {
    _stdairService = stdair::STDAIR_ServicePtr_T (&ioSTDAIR_Service);
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMCRS_ServiceContext: " << std::endl
         << "CRS code: " << _CRSCode
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  AIRSCHED::AIRSCHED_Service& SIMCRS_ServiceContext::getAIRSCHED_Service () const {
    assert (_airschedService != NULL);
    return *_airschedService;
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV::AIRINV_Service& SIMCRS_ServiceContext::
  getAIRINV_Service (const stdair::AirlineCode_T& iAirlineCode) const {   
    AIRINV_ServicePtr_Map_T::const_iterator itValueType =
      _airinvServiceMap.find (iAirlineCode);

    if (itValueType == _airinvServiceMap.end()) {
      STDAIR_LOG_ERROR ("Cannot find the Inventory service (AIRINV_Service) for the "
                        "given airline: " << iAirlineCode
                        << ". By design, it should not happen.");
      assert (false);
    }

    AIRINV_ServicePtr_T oAIRINV_Service_ptr = itValueType->second;
    assert (oAIRINV_Service_ptr != NULL);
    
    return *oAIRINV_Service_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_ServiceContext::
  addAIRINV_Service (const stdair::AirlineCode_T& iAirlineCode,
                     AIRINV_ServicePtr_T ioAIRINV_ServicePtr) {
    const bool hasInsertBeenSuccessful = _airinvServiceMap.
      insert (AIRINV_ServicePtr_Map_T::value_type (iAirlineCode,
                                                   ioAIRINV_ServicePtr)).second;

    if (hasInsertBeenSuccessful == false) {
      STDAIR_LOG_ERROR ("Cannot add the inventory service handler for this airline: "
                        << iAirlineCode);
      assert (false);
    }
  }
}
