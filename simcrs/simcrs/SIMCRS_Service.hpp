#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>

namespace SIMCRS {

  // Forward declaration
  class SIMCRS_ServiceContext;

  
  /** Interface for the SIMCRS Services. */
  class SIMCRS_Service {  
  private:
    // ///////// Service Context /////////
    /** Simcrs context. */
    SIMCRS_ServiceContext* _simcrsServiceContext;


  public:
    // ////////// Constructors and destructors //////////
    /** Constructor.
        @param std::ostream& Output log stream (for instance, std::cout)
        @param CRSCode_T& Code of the owner distribution system. */
    SIMCRS_Service (std::ostream&, const CRSCode_T&, const std::string&);

    /** Destructor. */
    ~SIMCRS_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    SIMCRS_Service ();
    /** Default copy constructor. */
    SIMCRS_Service (const SIMCRS_Service&);

    /** Initialise. */
    void init (std::ostream& ioLogStream, const CRSCode_T&, const std::string&);

    /** Finalise. */
    void finalise ();

  
  public:
    // /////////// Business Methods /////////////
    /** Register a booking (segment sell). */
    void sell (const AirlineCode_T&, const PartySize_T&);

  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP
