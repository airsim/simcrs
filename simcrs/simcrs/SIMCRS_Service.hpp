#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/bom/OutboundPathTypes.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>

// Forward declarations
namespace stdair {
  class OutboundPath;
  struct BookingRequestStruct;
}

namespace SIMCRS {

  // Forward declaration
  class SIMCRS_ServiceContext;

  
  /** Interface for the SIMCRS Services. */
  class SIMCRS_Service {  
  public:
    // /////////// Business Methods /////////////
    /** Get a list of travel solutions corresponding to the booking request. */
    stdair::OutboundPathLightList_T getTravelSolutions (const stdair::BookingRequestStruct&);
    
    /** Register a booking (segment sell). */
    void sell (const stdair::OutboundPath&, const stdair::PartySize_T&);


  public:
    // ////////// Constructors and destructors //////////
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
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    SIMCRS_Service (const CRSCode_T&, const stdair::Filename_T&);

    /** Destructor. */
    ~SIMCRS_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    SIMCRS_Service ();
    /** Default copy constructor. */
    SIMCRS_Service (const SIMCRS_Service&);

    /** Initialise the log. */
    void logInit (const stdair::BasLogParams&);

    /** Initialise.
        <br>The CSV file, describing the airline schedules for the
        simulator, is parsed and the inventories are generated accordingly.
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void init (const CRSCode_T& iCRSCode, const stdair::Filename_T&);

    /** Finalise. */
    void finalise ();


  private:
    // ///////// Service Context /////////
    /** Simcrs context. */
    SIMCRS_ServiceContext* _simcrsServiceContext;

  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP

