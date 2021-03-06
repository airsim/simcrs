// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
// Standard Airline Object Model
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp> 
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
// Airline Inventory
#include <airinv/AIRINV_Master_Service.hpp>
// Airline Schedule
#include <airtsp/AIRTSP_Service.hpp>
// Fare Quote
#include <simfqt/SIMFQT_Service.hpp>
// SimCRS
#include <simcrs/basic/BasConst_SIMCRS_Service.hpp>
#include <simcrs/command/DistributionManager.hpp>
#include <simcrs/factory/FacSimcrsServiceContext.hpp>
#include <simcrs/service/SIMCRS_ServiceContext.hpp>
#include <simcrs/SIMCRS_Service.hpp>

namespace SIMCRS {

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service() : _simcrsServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const SIMCRS_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the StdAir service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Add the StdAir service context to the SimCRS service context
    // \note SIMCRS owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SimFQT service.
    initSIMFQTService();
    
    // Initalise the AirTSP service.
    initAIRTSPService();
    
    // Initalise the AirInv service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::SIMCRS_Service (const stdair::BasLogParams& iLogParams,
                                  const stdair::BasDBParams& iDBParams,
                                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext (iCRSCode);
    
    // Add the StdAir service context to the SIMCRS service context
    // \note SIMCRS owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SIMFQT service.
    initSIMFQTService();
    
    // Initalise the AIRTSP service.
    initAIRTSPService();
    
    // Initalise the AIRINV service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::
  SIMCRS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
		  SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_Service_ptr,
                  const CRSCode_T& iCRSCode)
    : _simcrsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iCRSCode);

    // Store the STDAIR service object within the (AIRINV) service context
    // \note AirInv does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService); 

    //Add the SEvMgr service to the TRADEMGEN service context. 
    const bool doesNotOwnSEVMGRService = false;
    addSEVMGRService (ioSEVMGR_Service_ptr, doesNotOwnSEVMGRService);

    // Initalise the SIMFQT service.
    initSIMFQTService();
    
    // Initalise the AIRTSP service.
    initAIRTSPService();
    
    // Initalise the AIRINV service.
    initAIRINVService();
    
    // Initialise the (remaining of the) context
    initSimcrsService();
  }

  // ////////////////////////////////////////////////////////////////////
  SIMCRS_Service::~SIMCRS_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::finalise() {
    assert (_simcrsServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _simcrsServiceContext->reset();
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
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Store the StdAir service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                              iOwnStdairService);
  }  

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  addSEVMGRService (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_Service_ptr,
		    const bool iOwnSEVMGRService) {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Store the STDAIR service object within the (TRADEMGEN) service context
    lSIMCRS_ServiceContext.setSEVMGR_Service (ioSEVMGR_Service_ptr,
					      iOwnSEVMGRService);
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SIMCRS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the StdAir service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    return lSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T SIMCRS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the STDAIR service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    
    return lSTDAIR_Service_ptr;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initAIRTSPService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the AIRTSP service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    AIRTSP::AIRTSP_ServicePtr_T lAIRTSP_Service_ptr = 
      boost::make_shared<AIRTSP::AIRTSP_Service> (lSTDAIR_Service_ptr);
    
    // Store the AIRTSP service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRTSP_Service (lAIRTSP_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initSIMFQTService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the SIMFQT service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SIMFQT::SIMFQT_ServicePtr_T lSIMFQT_Service_ptr = 
      boost::make_shared<SIMFQT::SIMFQT_Service> (lSTDAIR_Service_ptr);
    
    // Store the SIMFQT service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setSIMFQT_Service (lSIMFQT_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initAIRINVService() {

    // Retrieve the SimCRS service context
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMCRS_ServiceContext.getSTDAIR_ServicePtr(); 

    /**
     * Initialise the AIRINV service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    AIRINV::AIRINV_Master_ServicePtr_T lAIRINV_Master_Service_ptr;
    const bool ownSEVMGRService = 
      lSIMCRS_ServiceContext.getOwnSEVMGRServiceFlag();
    if (ownSEVMGRService == false) { 
      // Retrieve the SEVMGR service
      SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
	lSIMCRS_ServiceContext.getSEVMGR_ServicePtr();
      assert (lSEVMGR_Service_ptr != NULL);
      lAIRINV_Master_Service_ptr = 
	boost::make_shared<AIRINV::AIRINV_Master_Service> (lSTDAIR_Service_ptr, 
							   lSEVMGR_Service_ptr);
    } else {
      lAIRINV_Master_Service_ptr = 
	boost::make_shared<AIRINV::AIRINV_Master_Service> (lSTDAIR_Service_ptr);
    }
    assert (lAIRINV_Master_Service_ptr != NULL);
    
    // Store the AIRINV service object within the (SimCRS) service context
    lSIMCRS_ServiceContext.setAIRINV_Service (lAIRINV_Master_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::initSimcrsService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  parseAndLoad (const stdair::ScheduleFilePath& iScheduleInputFilepath,
                const stdair::ODFilePath& iODInputFilepath,
                const stdair::FRAT5FilePath& iFRAT5InputFilepath,
                const stdair::FFDisutilityFilePath& iFFDisutilityInputFilepath,
                const AIRRAC::YieldFilePath& iYieldInputFilepath,
                const SIMFQT::FareFilePath& iFareInputFilepath) {
 
    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the SimCRS service context and whether it owns the Stdair
    // service
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    const bool doesOwnStdairService =
      lSIMCRS_ServiceContext.getOwnStdairServiceFlag(); 

    // Retrieve the StdAir service object from the (SimCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    /**
     * 1. Delegate the complementary building of objects and links by the
     *    appropriate levels/components.
     */
    /**
     * Let the schedule manager (i.e., the AirTSP component) parse
     * the schedules and O&Ds. AirTSP holds the flight-periods (aka schedule)
     * only, not the flight-dates (aka the inventory).
     */
    AIRTSP::AIRTSP_Service& lAIRTSP_Service =
      lSIMCRS_ServiceContext.getAIRTSP_Service();
    lAIRTSP_Service.parseAndLoad (iScheduleInputFilepath);

    /**
     * Let the inventory manager (i.e., the AirInv component) parse
     * the schedules, O&Ds and yields. From the schedules and O&Ds,
     * AirInv builds the flight-dates (aka the inventory).
     * The flight-periods (aka schedule) are then dropped.
     */
    AIRINV::AIRINV_Master_Service& lAIRINV_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    lAIRINV_Service.parseAndLoad (iScheduleInputFilepath, iODInputFilepath,
                                  iFRAT5InputFilepath,
                                  iFFDisutilityInputFilepath,
                                  iYieldInputFilepath);

    /**
     * Let the pricing component to build the fare rule structures.
     */
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
    lSIMFQT_Service.parseAndLoad (iFareInputFilepath); 

    /**
     * 2. Build the complementary objects/links for the current component (here,
     *    SimCRS).
     */  
    buildComplementaryLinks (lPersistentBomRoot);  
    
    /**
     * 3. Have SimCRS clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (SimCRS here)
     */  
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::buildSampleBom() {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the SimCRS service context and whether it owns the Stdair
    // service
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    const bool doesOwnStdairService =
      lSIMCRS_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (SimCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (SimCRS here).
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.buildSampleBom();
    }

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components.
     */
    /**
     * Let the schedule manager (i.e., the AirTSP component) build
     * the schedules and O&Ds. AirTSP holds the flight-periods (aka schedule)
     * only, not the flight-dates (aka the inventory).
     */
    AIRTSP::AIRTSP_Service& lAIRTSP_Service =
      lSIMCRS_ServiceContext.getAIRTSP_Service();
    lAIRTSP_Service.buildSampleBom();

    /**
     * Let the inventory manager (i.e., the AirInv component) build
     * the schedules, O&Ds and yields. From the schedules and O&Ds,
     * AirInv builds the flight-dates (aka the inventory).
     * The flight-periods (aka schedule) are then dropped.
     */
    AIRINV::AIRINV_Master_Service& lAIRINV_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    lAIRINV_Service.buildSampleBom();

    /**
     * Let the pricing component to build the fare rules.
     */
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
    lSIMFQT_Service.buildSampleBom();

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    SimCRS).
     */  
    buildComplementaryLinks (lPersistentBomRoot);

    /**
     * 4. Have SimCRS clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (SimCRS here)
     */
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }
  } 

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::clonePersistentBom () {   

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the SimCRS service context and whether it owns the Stdair
    // service
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    const bool doesOwnStdairService =
      lSIMCRS_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (SimCRS) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();
 
    /**
     * 1. Have StdAir clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TravelCCM here)
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.clonePersistentBom ();
    } 

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components.
     */
    /**
     * Let the schedule manager (i.e., the AirTSP component) build
     * the schedules and O&Ds. AirTSP holds the flight-periods (aka schedule)
     * only, not the flight-dates (aka the inventory).
     */
    AIRTSP::AIRTSP_Service& lAIRTSP_Service =
      lSIMCRS_ServiceContext.getAIRTSP_Service();
    lAIRTSP_Service.clonePersistentBom ();

    /**
     * Let the inventory manager (i.e., the AirInv component) build
     * the schedules, O&Ds and yields. From the schedules and O&Ds,
     * AirInv builds the flight-dates (aka the inventory).
     * The flight-periods (aka schedule) are then dropped.
     */
    AIRINV::AIRINV_Master_Service& lAIRINV_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    lAIRINV_Service.clonePersistentBom ();

    /**
     * Let the pricing component to build the fare rules.
     */
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
    lSIMFQT_Service.clonePersistentBom ();    
  
    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    SimCRS).
     */  
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();
    buildComplementaryLinks (lBomRoot);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::buildComplementaryLinks (stdair::BomRoot& ioBomRoot) {
    // Currently, no more things to do by TravelCCM at that stage.
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  buildSampleTravelSolutions(stdair::TravelSolutionList_T& ioTravelSolutionList){

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    lSTDAIR_Service.buildSampleTravelSolutions (ioTravelSolutionList);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct SIMCRS_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // ////////////////////////////////////////////////////////////////////
  bool SIMCRS_Service::sell (const std::string& iSegmentDateKey,
                             const stdair::ClassCode_T& iClassCode,
                             const stdair::PartySize_T& iPartySize) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    return lAIRINV_Master_Service.sell (iSegmentDateKey, iClassCode,
                                        iPartySize);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  jsonHandler (const stdair::JSONString& iJSONString) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    return lAIRINV_Master_Service.jsonHandler (iJSONString);
    
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  initSnapshotAndRMEvents (const stdair::Date_T& iStartDate,
                           const stdair::Date_T& iEndDate) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.initSnapshotAndRMEvents (iStartDate, iEndDate);
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::csvDisplay() const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service(); 
    const stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay(lBomRoot);
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  csvDisplay (const stdair::TravelSolutionList_T& ioTravelSolutionList) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    // Retrieve the StdAir service object from the (SimCRS) service context
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    stdair::STDAIR_Service& lSTDAIR_Service =
      lSIMCRS_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.csvDisplay (ioTravelSolutionList);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  list (const stdair::AirlineCode_T& iAirlineCode,
        const stdair::FlightNumber_T& iFlightNumber) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    // Delegate the BOM display to the dedicated service
    return lAIRINV_Master_Service.list (iAirlineCode, iFlightNumber);
  }

  // ////////////////////////////////////////////////////////////////////
  std::string SIMCRS_Service::
  csvDisplay (const stdair::AirlineCode_T& iAirlineCode,
              const stdair::FlightNumber_T& iFlightNumber,
              const stdair::Date_T& iDepartureDate) const {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    // Delegate the BOM display to the dedicated service
    return lAIRINV_Master_Service.csvDisplay (iAirlineCode, iFlightNumber,
                                              iDepartureDate);
  }
   
  // ////////////////////////////////////////////////////////////////////
  stdair::TravelSolutionList_T SIMCRS_Service::
  calculateSegmentPathList(const stdair::BookingRequestStruct& iBookingRequest){
     
    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service "
                                                    "has not been initialised");
    }
    assert (_simcrsServiceContext != NULL);

    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    stdair::TravelSolutionList_T oTravelSolutionList;
    
    // Get a reference on the AIRTSP service handler
    AIRTSP::AIRTSP_Service& lAIRTSP_Service =
      lSIMCRS_ServiceContext.getAIRTSP_Service();
    
    // Delegate the booking to the dedicated service
    stdair::BasChronometer lTravelSolutionRetrievingChronometer;
    lTravelSolutionRetrievingChronometer.start();

    lAIRTSP_Service.buildSegmentPathList (oTravelSolutionList,
                                            iBookingRequest);
      
    // DEBUG
    const double lSegmentPathRetrievingMeasure =
      lTravelSolutionRetrievingChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Travel solution retrieving: "
                      << lSegmentPathRetrievingMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return oTravelSolutionList;
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  fareQuote (const stdair::BookingRequestStruct& iBookingRequest,
             stdair::TravelSolutionList_T& ioTravelSolutionList) {
     
    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;
    
    // Get a reference on the SIMFQT service handler
    SIMFQT::SIMFQT_Service& lSIMFQT_Service =
      lSIMCRS_ServiceContext.getSIMFQT_Service();
      
    // Delegate the action to the dedicated command
    stdair::BasChronometer lFareQuoteRetrievalChronometer;
    lFareQuoteRetrievalChronometer.start();

    lSIMFQT_Service.quotePrices (iBookingRequest, ioTravelSolutionList);

    // DEBUG
    const double lFareQuoteRetrievalMeasure =
      lFareQuoteRetrievalChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Fare Quote retrieving: " << lFareQuoteRetrievalMeasure
                      << " - " << lSIMCRS_ServiceContext.display());
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  calculateAvailability (stdair::TravelSolutionList_T& ioTravelSolutionList) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the availability retrieval to the dedicated command
    stdair::BasChronometer lAvlChronometer;
    lAvlChronometer.start();

    DistributionManager::calculateAvailability (lAIRINV_Master_Service,
                                                ioTravelSolutionList);
    
    // DEBUG
    const double lAvlMeasure = lAvlChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Availability retrieval: " << lAvlMeasure << " - "
                      << lSIMCRS_ServiceContext.display());
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool SIMCRS_Service::
  sell (const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::PartySize_T& iPartySize) {
    bool hasSaleBeenSuccessful = false;

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSellChronometer;
    lSellChronometer.start();

    hasSaleBeenSuccessful = DistributionManager::sell (lAIRINV_Master_Service,
                                                       iTravelSolution,
                                                       iPartySize);

    // DEBUG
    STDAIR_LOG_DEBUG ("Made a sell of " << iPartySize
                      << " persons on the following travel solution: "
                      << iTravelSolution.describe()
		      << " with the chosen fare option: "
		      << iTravelSolution.getChosenFareOption().describe()
                      << ". Successful? " << hasSaleBeenSuccessful);
      
    // DEBUG
    const double lSellMeasure = lSellChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Booking sell: " << lSellMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return hasSaleBeenSuccessful;
  }

  
  // ////////////////////////////////////////////////////////////////////
  bool SIMCRS_Service::
  playCancellation (const stdair::CancellationStruct& iCancellation) {
    bool hasCancellationBeenSuccessful = false;

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the CRS code
    //const CRSCode_T& lCRSCode = lSIMCRS_ServiceContext.getCRSCode();

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();
    
    // Delegate the booking to the dedicated command
    stdair::BasChronometer lCancellationChronometer;
    lCancellationChronometer.start();

    hasCancellationBeenSuccessful =
      DistributionManager::playCancellation (lAIRINV_Master_Service,
                                             iCancellation);
                                             
    // DEBUG
    STDAIR_LOG_DEBUG ("Made a cancellation of " << iCancellation.describe());
      
    // DEBUG
    const double lCancellationMeasure = lCancellationChronometer.elapsed();
    STDAIR_LOG_DEBUG ("Booking cancellation: " << lCancellationMeasure << " - "
                      << lSIMCRS_ServiceContext.display());

    return hasCancellationBeenSuccessful;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::takeSnapshots (const stdair::SnapshotStruct& iSnapshot) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.takeSnapshots (iSnapshot);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMCRS_Service::
  optimise (const stdair::RMEventStruct& iRMEvent) {

    // Retrieve the SimCRS service context
    if (_simcrsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimCRS service has "
                                                    "not been initialised");
    }
    assert (_simcrsServiceContext != NULL);
    SIMCRS_ServiceContext& lSIMCRS_ServiceContext = *_simcrsServiceContext;

    // Retrieve the AIRINV Master service.
    AIRINV::AIRINV_Master_Service& lAIRINV_Master_Service =
      lSIMCRS_ServiceContext.getAIRINV_Service();

    lAIRINV_Master_Service.optimise (iRMEvent);
  }
}
