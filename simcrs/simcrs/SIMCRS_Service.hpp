#ifndef __SIMCRS_SVC_SIMCRS_SERVICE_HPP
#define __SIMCRS_SVC_SIMCRS_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
// StdAir
#include <stdair/STDAIR_Types.hpp>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>

namespace SIMCRS {

  // Forward declaration
  class SIMCRS_ServiceContext;

  
  /** Interface for the SIMCRS Services. */
  class SIMCRS_Service {  
  public:
    // /////////// Business Methods /////////////
    /** Register a booking (segment sell). */
    void sell (const stdair::AirlineCode_T&, const stdair::PartySize_T&);


  public:
    // ////////// Constructors and destructors //////////
    /** Constructor.
        @param std::ostream& Output log stream (for instance, std::cout)
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    SIMCRS_Service (std::ostream&, const CRSCode_T&, const stdair::Filename_T&);

    /** Destructor. */
    ~SIMCRS_Service();

    
  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    SIMCRS_Service ();
    /** Default copy constructor. */
    SIMCRS_Service (const SIMCRS_Service&);

    /** Initialise.
        @param std::ostream& Output log stream (for instance, std::cout)
        @param const CRSCode_T& Code of the owner of the distribution system.
        @param const stdair::Filename_T& Filename of the input schedule file. */
    void init (std::ostream& ioLogStream, const CRSCode_T&,
               const stdair::Filename_T&);

    /** Finalise. */
    void finalise ();


  private:
    // ///////// Service Context /////////
    /** Simcrs context. */
    SIMCRS_ServiceContext* _simcrsServiceContext;

  };
}
#endif // __SIMCRS_SVC_SIMCRS_SERVICE_HPP

