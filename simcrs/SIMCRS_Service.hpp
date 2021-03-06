#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_file.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// AIRRAC
#include <airrac/AIRRAC_Types.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
  struct CancellationStruct;
  struct SnapshotStruct;
  struct RMEventStruct;
  class JSONString;
}

namespace SIMCRS {

  /// Forward declaration
  class SIMCRS_ServiceContext;

  
  /**
   * Interface for the SIMCRS Services.
   */
  class SIMCRS_Service {  
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * A reference on an output stream is given, so that log
     * outputs can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     */
    SIMCRS_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const CRSCode_T&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, a reference on an output stream is given, so
     * that log outputs can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     */
    SIMCRS_Service (const stdair::BasLogParams&, const CRSCode_T&);

    /**
     * Constructor.
     *
     * The init() method is called; see the corresponding documentation
     * for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * it is assumed that the StdAir log service has already been
     * initialised with the proper log output stream by some other
     * methods in the calling chain (for instance, when the SIMCRS_Service
     * is itself being initialised by another library service such as
     * TVLSIM_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.   
     * @param SEVMGR::SEVMGR_ServicePtr_T Reference on the SEVMGR_Service.
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     * @param const CRSCode_T& Code of the owner of the distribution system.
     */
    SIMCRS_Service (stdair::STDAIR_ServicePtr_T, SEVMGR::SEVMGR_ServicePtr_T, 
		    const CRSCode_T&); 


    /**
     * Parse the schedule, O&D, fare and yield input files.
     *
     * The CSV files, describing the airline schedule, O&Ds, fares and yields
     * for the simulator, are parsed and instantiated in memory accordingly.
     *
     * @param const stdair::ScheduleFilePath Filename of the input schedule file.
     * @param const stdair::ODFilePath Filename of the input O&D file.
     * @param const stdair::FRAT5FilePath& Filename of the input FRAT5 file.
     * @param const stdair::FFDisutilityFilePath& Filename of the input FF disutility file.
     * @param const AIRRAC::YieldFilePath& Filename of the input yield file.
     * @param const SIMFQT::FareFilePath& Filename of the input fare file.
     */
    void parseAndLoad (const stdair::ScheduleFilePath&,
                       const stdair::ODFilePath&,
                       const stdair::FRAT5FilePath&,
                       const stdair::FFDisutilityFilePath&,
                       const AIRRAC::YieldFilePath&,
                       const SIMFQT::FareFilePath&);

    /**
     * Initialise the snapshot and RM events for the inventories.
     *
     * @param const stdair::Date_T& Start date of the simulation.
     * @param const stdair::Date_T& End date of the simulation.
     */
    void initSnapshotAndRMEvents (const stdair::Date_T& iStartDate,
                                  const stdair::Date_T& iEndDate);

    /**
     * Destructor.
     */
    ~SIMCRS_Service();

    
  public:
    // /////////////// Business Methods /////////////////
    /**
     * Construct the list of travel solutions corresponding to the
     * booking request.
     */
    stdair::TravelSolutionList_T 
    calculateSegmentPathList (const stdair::BookingRequestStruct&);
    
    /**
     * Calculate the fare of each travel solutions in the list.
     */
    void fareQuote (const stdair::BookingRequestStruct&,
                    stdair::TravelSolutionList_T&);

    /**
     * Compute the availability for each travel solution in the list.
     */
    void calculateAvailability (stdair::TravelSolutionList_T&);
    
    /**
     * Register a booking.
     */
    bool sell (const stdair::TravelSolutionStruct&, const stdair::PartySize_T&);

    /**
     * Take inventory snapshots.
     */
    void takeSnapshots (const stdair::SnapshotStruct&);

    /**
     * Play cancellation.
     */
    bool playCancellation (const stdair::CancellationStruct&);
    
    /**
     * Optimise (revenue management) an flight-date/network-date
     */
    void optimise (const stdair::RMEventStruct&);

    /**
     * Register a booking.
     *
     * @param const std::string& Key for the segment on which the sale is made
     * @param const stdair::ClassCode_T& Class code where the sale is made
     * @param const stdair::PartySize_T& Party size
     * @return bool Whether or not the sale was successfull
     */
    bool sell (const std::string& iSegmentDateKey, const stdair::ClassCode_T&,
               const stdair::PartySize_T&);
    
    /**
     * Build a sample BOM tree, and attach it to the BomRoot instance.
     *
     * As for now, the BOM sample tree is the one built by the AirInv
     * component.
     *
     * \see AIRINV::AIRINV_Master_Service and stdair::CmdBomManager for
     *      more details.
     */
    void buildSampleBom ();
  
    /**
     * Clone the persistent BOM object.
     */
    void clonePersistentBom (); 

    /**
     * Build all the complementary links in the given bom root object.
     * \note Do nothing for now.
     */
    void buildComplementaryLinks (stdair::BomRoot&); 

    /**
     * Build a sample list of travel solutions.
     *
     * As of now (March 2011), that list is made of the following
     * travel solutions:
     * <ul>
     *  <li>BA9</li>
     *  <li>LHR-SYD</li>
     *  <li>2011-06-10</li>
     *  <li>Q</li>
     *  <li>WTP: 900</li>
     *  <li>Change fee: 20; Non refundable; Saturday night stay</li>
     * </ul>
     *
     * \see stdair::CmdBomManager for more details.
     *
     * @param TravelSolutionList_T& Sample list of travel solution structures.
     *        It should be given empty. It is altered with the returned sample.
     */
    void buildSampleTravelSolutions (stdair::TravelSolutionList_T&);

    /**
     * Build a sample booking request structure.
     *
     * As of now (March 2011), the sample booking request is made of the
     * following parameters:
     * <ul>
     *  <li>Return trip (inbound): LHR-SYD (POS: LHR, Channel: DN), </li>
     *  <li>Departing 10-JUN-2011 around 8:00, staying 7 days</li>
     *  <li>Requested on 15-MAY-2011 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * As of now (March 2011), the CRS-related booking request is made
     * of the following parameters:
     * <ul>
     *  <li>Return trip (inbound): SIN-BKK (POS: SIN, Channel: IN), </li>
     *  <li>Departing 30-JAN-2010 around 10:00, staying 7 days</li>
     *  <li>Requested on 22-JAN-2010 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * \see stdair::CmdBomManager for more details.
     *
     * @param const bool isForCRS Whether the sample booking request is for CRS.
     * @return BookingRequestStruct& Sample booking request structure.
     */
    stdair::BookingRequestStruct
    buildSampleBookingRequest (const bool isForCRS = false);


  public:
    // //////////////// Export support methods /////////////////
    /**
     * Dispatch the JSon command string to the AirInv service.
     * (Only AirInv has json export commands for now).
     * 
     * @param const stdair::JSONString& Input string which contained the JSon
     *        command string.
     * @return std::string Output string in which the asking objects are
     *         logged/dumped with a JSon format.
     */
    std::string jsonHandler (const stdair::JSONString&) const;

  public:
    // //////////////// Display support methods /////////////////
    /**
     * Recursively display (dump in the returned string) the objects
     * of the BOM tree.
     *
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay() const;

    /**
     * Display (dump in the returned string) the full list of travel
     * solution structures.
     *
     * @return std::string Output string in which the list of travel
     *        solutions is logged/dumped.
     */
    std::string csvDisplay (const stdair::TravelSolutionList_T&) const;

    /**
     * Display the list of flight-dates (contained within the BOM tree)
     * corresponding to the parameters given as input.
     *
     * @param const AirlineCode& Airline for which the flight-dates should be
     *        displayed. If set to "all" (the default), all the inventories
     *        will be displayed.
     * @param const FlightNumber_T& Flight number for which all the departure
     *        dates should be displayed. If set to 0 (the default),
     *        all the flight numbers will be displayed.
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string list (const stdair::AirlineCode_T& iAirlineCode = "all",
                      const stdair::FlightNumber_T& iFlightNumber = 0) const;

    /**
     * Recursively display (dump in the returned string) the flight-date
     * corresponding to the parameters given as input.
     *
     * @param const stdair::AirlineCode_T& Airline code of the flight to display
     * @param const stdair::FlightNumber_T& Flight number of the
     *        flight to display.
     * @param const stdair::Date_T& Departure date of the flight to display.
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay (const stdair::AirlineCode_T&,
                            const stdair::FlightNumber_T&,
                            const stdair::Date_T& iDepartureDate) const;


  private:
    // /////// Construction and Destruction helper methods ///////
    /**
     * Default constructor. It should not be used.
     */
    SIMCRS_Service();

    /**
     * Default copy constructor. It should not be used.
     */
    SIMCRS_Service (const SIMCRS_Service&);

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&,
                                                   const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot object,
     * is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output log
     *        stream.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Initialise the AirTSP service (including the log service).
     */
    void initAIRTSPService();
    
    /**
     * Initialise the SimFQT service (including the log service).
     */
    void initSIMFQTService();

    /**
     * Initialise the AirInv service (including the log service).
     */
    void initAIRINVService();

    /**
     * Attach the StdAir service (holding the log and database services) to
     * the SIMCRS_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the StdAir service.
     * @param const bool State whether or not SimCRS owns the StdAir service
     *        resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T,
                           const bool iOwnStdairService);  

    /**
     * Attach the SEVMGR service to the SIMCRS_Service.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Reference on the SEVMGR service.
     */
    void addSEVMGRService (SEVMGR::SEVMGR_ServicePtr_T,
			   const bool iOwnSEVMGRService);
    
    /**
     * Initialise the (SimCRS) service context (i.e., the
     * SIMCRS_ServiceContext object).
     *
     * @param const CRSCode_T& Code of the owner of the distribution system.
     */
    void initServiceContext (const CRSCode_T&);

    /**
     * Initialise the (SimCRS) service context (i.e., the SIMCRS_ServiceContext
     * object).
     */
    void initSimcrsService();

    /**
     * Finalise.
     */
    void finalise();


  private:
    // ///////// Service Context /////////
    /**
     * SimCRS context.
     */
    SIMCRS_ServiceContext* _simcrsServiceContext;
  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP
