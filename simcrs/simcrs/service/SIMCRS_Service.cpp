// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
// Standard Airline Object Model
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/InventoryTypes.hpp>
#include <stdair/bom/BomSource.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
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

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service () : _simcrsServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const SIMCRS_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iCRSCode);

    // Retrieve the Simcrs service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::BasDBParams& iDBParams,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                  const CRSCode_T& iCRSCode,
                  const stdair::Filename_T& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initServiceContext (const CRSCode_T& iCRSCode) {
    // Initialise the service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = 
      FacSimcrsServiceContext::instance().create (iCRSCode);
    _simcrsServiceContext = &lSIMCRS_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (SIMCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::init (const stdair::Filename_T& iScheduleInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iScheduleInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The schedule input file, '" << iScheduleInputFilename
                        << "', can not be retrieved on the file-system");
      throw FileNotFoundException();
    }

    // Initialise the children AirSched service context
    initAIRSCHEDService (iScheduleInputFilename);

    // Initialise the children AirInv service context
    initAIRINVServices ();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initAIRSCHEDService (const stdair::Filename_T& iScheduleInputFilename) {
    
    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);

    // Initialise the AIRSCHED service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    AIRSCHED_ServicePtr_T lAIRSCHED_Service_ptr =
      boost::make_shared<AIRSCHED::AIRSCHED_Service> (lSTDAIR_Service_ptr,
                                                      iScheduleInputFilename);

    // Store the AirSched service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRSCHED_Service (lAIRSCHED_Service_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initAIRINVServices () {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Retrieve, from the StdAir service context, the root of the BOM tree
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();
    
    // Retrieve the list of Inventory objects: one per airline
    const stdair::InventoryList_T& lInventoryList = lBomRoot.getInventoryList();

    // Browse the inventory list and initialise the corresponding
    // AirInv services.
    for (stdair::InventoryList_T::iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      stdair::Inventory& lCurrentInv = *itInv;
      const stdair::AirlineCode_T& lAirlineCode = lCurrentInv.getAirlineCode();

      // Build an AirInv service instance, tracked by a Boost SmartPointer
      AIRINV_ServicePtr_T lAIRINV_Service_ptr =
        AIRINV_ServicePtr_T (new AIRINV::AIRINV_Service (lSTDAIR_Service_ptr,
                                                         lAirlineCode));

      // TODO: Understand why the following does not compile
      // boost::make_shared<AIRINV::AIRINV_Service> (lSTDAIR_Service_ptr,
      //                                             lAirlineCode, lCurrentInv);

      // Store the AirInv service object within the (SimCRS) service context
      lSIMCRS_ServiceContext.addAIRINV_Service (lAirlineCode,
                                                lAIRINV_Service_ptr);
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise () {
    assert (_simcrsServiceContext != NULL);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::TravelSolutionList_T SIMCRS_Service::
  getTravelSolutions (const stdair::BookingRequestStruct& iBookingRequest) {
     
    if (_simcrsServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext= *_simcrsServiceContext;

    stdair::TravelSolutionList_T oTravelSolutionList;
    
    try {
      
      // Get a reference on the AIRSCHED service handler
      AIRSCHED_ServicePtr_T lAIRSCHED_Service_ptr =
        lSIMCRS_ServiceContext.getAIRSCHED_Service();
      assert (lAIRSCHED_Service_ptr != NULL);
            
      // Delegate the booking to the dedicated service
      stdair::BasChronometer lTravelSolutionRetrievingChronometer;
      lTravelSolutionRetrievingChronometer.start();
      lAIRSCHED_Service_ptr->getTravelSolutions (oTravelSolutionList,
                                                 iBookingRequest);
      
      // DEBUG 
      // const double lTravelSolutionRetrievingMeasure =
      //  lTravelSolutionRetrievingChronometer.elapsed();
      // STDAIR_LOG_DEBUG ("Travel solution retrieving: "
      //                   << lTravelSolutionRetrievingMeasure << " - "
      //                   << lSIMCRS_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw BookingException();
    }

    return oTravelSolutionList;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  getAvailability (stdair::TravelSolutionList_T& ioTravelSolutionList) {
    if (_simcrsServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext= *_simcrsServiceContext;

    try {
      // Retrieve the CRS code
      const CRSCode_T& lCRSCode =
        lSIMCRS_ServiceContext.getCRSCode();

      // TODO: optimise this part.
      // Retrieve the map/list of AIRINV_Services
      const AIRINV_ServicePtr_Map_T& lAIRINV_ServiceMap =
        lSIMCRS_ServiceContext.getAIRINV_ServiceMap ();
      
      // Delegate the action to the dedicated command
      stdair::BasChronometer lAvailabilityRetrievalChronometer;
      lAvailabilityRetrievalChronometer.start();
      DistributionManager::getAvailability (lAIRINV_ServiceMap,
                                            lCRSCode, ioTravelSolutionList);
      // const double lAvailabilityRetrievalMeasure =
      //   lAvailabilityRetrievalChronometer.elapsed();
            
    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw AvailabilityRetrievalException();
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::sell(const stdair::TravelSolutionStruct& iTravelSolution,
                            const stdair::PartySize_T& iPartySize) {
    
    if (_simcrsServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext= *_simcrsServiceContext;

    try {
      // Retrieve the CRS code
      const CRSCode_T& lCRSCode =
        lSIMCRS_ServiceContext.getCRSCode();

      // TODO: optimise this part.
      // Retrieve the map/list of AIRINV_Services
      const AIRINV_ServicePtr_Map_T& lAIRINV_ServiceMap =
        lSIMCRS_ServiceContext.getAIRINV_ServiceMap ();
      
      // Delegate the booking to the dedicated command
      stdair::BasChronometer lSellChronometer;
      lSellChronometer.start();
      DistributionManager::sell (lAIRINV_ServiceMap,
                                 lCRSCode, iTravelSolution, iPartySize);
      
      // DEBUG
      // const double lSellMeasure = lSellChronometer.elapsed();
      // STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
      //                   << lSIMCRS_ServiceContext.display());
      
    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw BookingException();
    }
  }
  
}
