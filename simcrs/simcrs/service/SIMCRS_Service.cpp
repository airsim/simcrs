// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Simcrs
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/basic/BasChronometer.hpp>
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/factory/FacSimcrsServiceContext.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>
#include <simcrs/service/Logger.hpp>
#include <simcrs/SIMCRS_Service.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (std::ostream& ioLogStream, const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {
    init (ioLogStream, iCRSCode);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service ()
    : _simcrsServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const SIMCRS_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::init (std::ostream& ioLogStream,
                             const CRSCode_T& iCRSCode) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = 
      FacSimcrsServiceContext::instance().create (iCRSCode);
    _simcrsServiceContext = &lSIMCRS_ServiceContext;

    // TODO: do not hardcode the airline code (e.g., take it from a
    // configuration file).
    // Initialise the AIRINV service handler
    const AIRINV::AirlineCode_T lAirlineCode = "AA";
    AIRINV_ServicePtr_T lAIRINV_Service =
      AIRINV_ServicePtr_T (new AIRINV::AIRINV_Service (ioLogStream,
                                                       lAirlineCode));
    lSIMCRS_ServiceContext.setAIRINV_Service (lAIRINV_Service);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise () {
    assert (_simcrsServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::sell (const AirlineCode_T& iAirlineCode,
                             const PartySize_T& iPartySize) {
    
    if (_simcrsServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext= *_simcrsServiceContext;

    try {
      
      // Get a reference on the AIRINV service handler
      AIRINV::AIRINV_Service& lAIRINV_Service =
        lSIMCRS_ServiceContext.getAIRINV_Service();
      
      // Retrieve the airline code
      const CRSCode_T& lCRSCode =
        lSIMCRS_ServiceContext.getCRSCode();
      
      // Delegate the booking to the dedicated command
      BasChronometer lSellChronometer;
      lSellChronometer.start();
      DistributionManager::sell (lAIRINV_Service,
                                 lCRSCode, iAirlineCode, iPartySize);
      const double lSellMeasure = lSellChronometer.elapsed();
      
      // DEBUG
      SIMCRS_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                        << lSIMCRS_ServiceContext.display());

    } catch (const std::exception& error) {
      SIMCRS_LOG_ERROR ("Exception: "  << error.what());
      throw BookingException();
    }
  }
  
}
