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
  SIMCRS_ServiceContext::SIMCRS_ServiceContext ()
    : _ownStdairService (false), _ownSEVMGRService (true),
      _CRSCode (DEFAULT_CRS_CODE) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const SIMCRS_ServiceContext&)
    : _ownStdairService (false), _ownSEVMGRService (true) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::SIMCRS_ServiceContext (const CRSCode_T& iCRSCode)
    : _ownSEVMGRService (true),_CRSCode (iCRSCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_ServiceContext::~SIMCRS_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMCRS_ServiceContext [" << _CRSCode
         << "] - Owns StdAir service: " << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMCRS_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_ServiceContext::reset() {

    // The shared_ptr<>::reset() method drops the refcount by one.
    // If the count result is dropping to zero, the resource pointed to
    // by the shared_ptr<> will be freed.
    
    // Reset the stdair shared pointer
    _stdairService.reset();

    // Reset the simfqt shared pointer
    _simfqtService.reset();
    
     // Reset the airsched shared pointer
    _airschedService.reset();

    // Reset the airinv shared pointer 
    _airinvService.reset();

    // Reset the sevmgr shared pointer
    _sevmgrService.reset();
  }

}
