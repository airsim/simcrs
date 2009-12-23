// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
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
      
      // Retrieve the airline code
      const CRSCode_T& lCRSCode =
        lSIMCRS_ServiceContext.getCRSCode();
      
      // Delegate the booking to the dedicated command
      BasChronometer lSellChronometer;
      lSellChronometer.start();
      DistributionManager::sell (lCRSCode, iAirlineCode, iPartySize);
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
