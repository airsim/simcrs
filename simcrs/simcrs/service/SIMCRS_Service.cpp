// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// Standard Airline Object Model
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/bom/AirlineFeatureSet.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/factory/FacBomContent.hpp>
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Airline Schedule
#include <airsched/AIRSCHED_Service.hpp>
// Simcrs
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/factory/FacSimcrsServiceContext.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>
#include <simcrs/SIMCRS_Service.hpp>

namespace SIMCRS {

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service () : _simcrsServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const SIMCRS_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {
    // Initialise the context
    init (iCRSCode, iScheduleInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {
    // Set the log file
    logInit (iLogParams);

    // Initialise the (remaining of the) context
    init (iCRSCode, iScheduleInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::logInit (const stdair::BasLogParams& iLogParams) {
    stdair::Logger::init (iLogParams);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::init (const CRSCode_T& iCRSCode,
                             const stdair::Filename_T& iScheduleInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The schedule input file, '" << iScheduleInputFilename
                        << "', can not be retrieved on the file-system");
      throw FileNotFoundException();
    }

    // Initialise the context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = 
      FacSimcrsServiceContext::instance().create (iCRSCode);
    _simcrsServiceContext = &lSIMCRS_ServiceContext;

    
    // /////////////// Airline Inventory Management (AirInv) /////////////
    // TODO: do not hardcode the airline code (e.g., take it from a
    // configuration file).
    // Initialise the AIRINV service handler
    const stdair::AirlineCode_T lAirlineCode = "BA";

    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    AIRINV_ServicePtr_T lAIRINV_Service =
      AIRINV_ServicePtr_T (new AIRINV::AIRINV_Service (lAirlineCode));

    // Store the AirInv service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRINV_Service (lAIRINV_Service);


    // ////////////// Airline Schedule Management (AirSched) /////////////
    // TODO: do not hardcode the initialisation phase of the schedule
    // Initialise the schedule
    stdair::AirlineFeatureSet& lAirlineFeatureSet =
      stdair::FacBomContent::instance().create<stdair::AirlineFeatureSet>();

    // TODO: do not harcode the airline code for the AirlineFeature object
    stdair::AirlineFeatureKey_T lAirlineFeatureKey (lAirlineCode);
    stdair::AirlineFeature& lAirlineFeature = stdair::FacBomContent::
      instance().create<stdair::AirlineFeature> (lAirlineFeatureKey);
    stdair::FacBomContent::
      linkWithParent<stdair::AirlineFeature> (lAirlineFeature,
                                              lAirlineFeatureSet);

    // TODO: do not hardcode the start analysis date
    const stdair::Date_T lStartAnalysisDate (2000, boost::gregorian::Jan, 1);

    // Initialise the AIRSCHED service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    AIRSCHED_ServicePtr_T lAIRSCHED_Service = 
      AIRSCHED_ServicePtr_T (new AIRSCHED::
                             AIRSCHED_Service (lAirlineFeatureSet,
                                               lStartAnalysisDate,
                                               iScheduleInputFilename));

    // Store the AirSched service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRSCHED_Service (lAIRSCHED_Service);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise () {
    assert (_simcrsServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::sell (const stdair::AirlineCode_T& iAirlineCode,
                             const stdair::PartySize_T& iPartySize) {
    
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
      stdair::BasChronometer lSellChronometer;
      lSellChronometer.start();
      DistributionManager::sell (lAIRINV_Service,
                                 lCRSCode, iAirlineCode, iPartySize);
      const double lSellMeasure = lSellChronometer.elapsed();
      
      // DEBUG
      STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                        << lSIMCRS_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw BookingException();
    }
  }
  
}
