#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/TravelSolutionTypes.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>

// Forward declarations
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct TravelSolutionStruct;
  struct BookingRequestStruct;
}

namespace SIMCRS {

  // Forward declaration
  class SIMCRS_ServiceContext;

  
  /** Interface for the SIMCRS Services. */
  class SIMCRS_Service {  
  public:
    // ////////// Constructors and destructors //////////
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>A reference on an output stream is given, so that log
        outputs can be directed onto that stream.
        <br>Moreover, database connection parameters are given, so that a
        session can be created on the corresponding database.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    SIMCRS_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                    const CRSCode_T&, const stdair::Filename_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    SIMCRS_Service (const stdair::BasLogParams&,
                    const CRSCode_T&, const stdair::Filename_T&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the SIMCRS_Service
        is itself being initialised by another library service such as
        DSIM_Service).
        @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    SIMCRS_Service (stdair::STDAIR_ServicePtr_T,
                    const CRSCode_T&, const stdair::Filename_T&);

    /** Destructor. */
    ~SIMCRS_Service();

    
  public:
    // /////////// Business Methods /////////////
    /** Get a list of travel solutions corresponding to the booking request. */
    stdair::TravelSolutionList_T getTravelSolutions (const stdair::BookingRequestStruct&);

    /** Get the availability for each travel solution in the list.. */
    void getAvailability (stdair::TravelSolutionList_T&);
    
    /** Register a booking. */
    void sell (const stdair::TravelSolutionStruct&, const stdair::PartySize_T&);


  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. It should not be used. */
    SIMCRS_Service ();
    /** Default copy constructor. It should not be used. */
    SIMCRS_Service (const SIMCRS_Service&);

    /** Initialise the (SIMCRS) service context (i.e., the
        SIMCRS_ServiceContext object).
        @param const CRSCode_T& Code of the owner of the distribution system. */
    void initServiceContext (const CRSCode_T&);

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access. */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&);
    
    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log
               stream. */
    void initStdAirService (const stdair::BasLogParams&);
    
    /** Initialise.
        <br>The CSV file, describing the airline schedules for the
        simulator, is parsed and the inventories are generated accordingly.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void init (const stdair::Filename_T& iScheduleInputFilename);

    /** Initialise the AIRSCHED service with the given schedule file.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void initAIRSCHEDService (const stdair::Filename_T& iScheduleInputFilename);

    /** Initialise the list of AIRINV services.
        <br>After having initialised the AIRSCHED service, parse the list of
        airline inventories and build a AIRINV service for each airline. */
    void initAIRINVServices ();

    /** Finalise. */
    void finalise ();


  private:
    // ///////// Service Context /////////
    /** Simcrs context. */
    SIMCRS_ServiceContext* _simcrsServiceContext;

  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP

