// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/BomList.hpp>
#include <stdair/bom/BomMap.hpp>
#include <stdair/bom/AirlineFeatureSet.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/factory/FacBomContent.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Airline Schedule
#include <airsched/AIRSCHED_Service.hpp>
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
  SIMCRS_Service (std::ostream& ioLogStream, const CRSCode_T& iCRSCode,
                  const std::string& iScheduleInputFilename)
    : _simcrsServiceContext (NULL) {
    init (ioLogStream, iCRSCode, iScheduleInputFilename);
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
                             const CRSCode_T& iCRSCode,
                             const std::string& iScheduleInputFilename) {
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

    // Initialise the AIRSCHED service handler
    AIRSCHED_ServicePtr_T lAIRSCHED_Service = 
      AIRSCHED_ServicePtr_T (new AIRSCHED::AIRSCHED_Service (ioLogStream));
    lSIMCRS_ServiceContext.setAIRSCHED_Service (lAIRSCHED_Service);

    // TODO: do not hardcode the initialisation phase of the schedule
    // Initialise the schedule
    // Create a dummy AirlineFeature object for the test.
    stdair::AirlineFeatureSet& lAirlineFeatureSet =
      stdair::FacBomContent::instance().create<stdair::AirlineFeatureSet>();
    stdair::AirlineFeatureKey_T lAirlineFeatureKey (lAirlineCode);

    stdair::AirlineFeature& lAirlineFeature = stdair::FacBomContent::instance().
      create<stdair::AirlineFeature> (lAirlineFeatureSet, lAirlineFeatureKey);

    const stdair::Date_T lStartAnalysisDate (2000, 1, 1);

    stdair::BomRoot& lBomRoot =
      AIRSCHED::AIRSCHED_Service::generateInventories (iScheduleInputFilename,
                                                       lAirlineFeatureSet,
                                                       lStartAnalysisDate);
    // Display the all the inventories.
    // Browse the BomRoot.
    const stdair::InventoryList_T lInventoryList = lBomRoot.getInventoryList ();
    for (stdair::InventoryList_T::iterator itInv = lInventoryList.begin();
         itInv != lInventoryList.end(); ++itInv) {
      const stdair::Inventory& lCurrentInventory = *itInv;
      SIMCRS_LOG_DEBUG ("Inventory: " << lCurrentInventory.toString());

      // Browse the Inventory.
      const stdair::FlightDateList_T lFDList =
        lCurrentInventory.getFlightDateList ();
      for (stdair::FlightDateList_T::iterator itFD = lFDList.begin();
           itFD != lFDList.end(); ++itFD) {
        const stdair::FlightDate& lCurrentFD = *itFD;
        SIMCRS_LOG_DEBUG ("Flight-date: " << lCurrentFD.toString());

        // Browse the FlightDate.
        const stdair::LegDateList_T lLDList = lCurrentFD.getLegDateList();
        for (stdair::LegDateList_T::iterator itLD = lLDList.begin();
             itLD != lLDList.end(); ++itLD) {
          const stdair::LegDate& lCurrentLD = *itLD;
          SIMCRS_LOG_DEBUG ("Leg-date: " << lCurrentLD.toString());
          
          // Browse the LegDate.
          const stdair::LegCabinList_T lLCList = lCurrentLD.getLegCabinList();
          for (stdair::LegCabinList_T::iterator itLC = lLCList.begin();
               itLC != lLCList.end(); ++itLC) {
            const stdair::LegCabin& lCurrentLC = *itLC;
            SIMCRS_LOG_DEBUG ("Leg-cabin: " << lCurrentLC.toString());
          }
        }
        
        const stdair::SegmentDateList_T lSDList =
          lCurrentFD.getSegmentDateList();
        for (stdair::SegmentDateList_T::iterator itSD = lSDList.begin();
             itSD != lSDList.end(); ++itSD) {
          const stdair::SegmentDate& lCurrentSD = *itSD;
          SIMCRS_LOG_DEBUG ("Segment-date: " << lCurrentSD.toString());

          // Browse the SegmentDate.
          const stdair::SegmentCabinList_T lSCList =
            lCurrentSD.getSegmentCabinList();
          for (stdair::SegmentCabinList_T::iterator itSC = lSCList.begin();
               itSC != lSCList.end(); ++itSC) {
            const stdair::SegmentCabin& lCurrentSC = *itSC;
            SIMCRS_LOG_DEBUG ("Segment-cabin: " << lCurrentSC.toString());

            // Browse the SegmentCabin
            const stdair::BookingClassList_T lBCList =
              lCurrentSC.getBookingClassList ();
            for (stdair::BookingClassList_T::iterator itBC = lBCList.begin();
                   itBC != lBCList.end(); ++itBC) {
              const stdair::BookingClass& lCurrentBC = *itBC;
              SIMCRS_LOG_DEBUG ("Booking class: " << lCurrentBC.toString());
            }
          }
        }
      }
    }

    lAIRSCHED_Service->setBomRoot (lBomRoot);
    
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
