#ifndef __SIMCRS_SIMCRS_TYPES_HPP
#define __SIMCRS_SIMCRS_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>

namespace SIMCRS {

  // ///////// Exceptions ///////////
  class RootException : public std::exception {
  };

  class NonInitialisedServiceException : public RootException {
  };

  class MemoryAllocationException : public RootException {
  };

  class ObjectNotFoundException : public RootException {
  };

  class SQLDatabaseException : public RootException {
  };

  class SQLDatabaseConnectionImpossibleException : public SQLDatabaseException {
  };

  class BookingException : public RootException {
  };


  // /////////////// Log /////////////
  /** Level of logs. */
  namespace LOG {
    typedef enum {
      CRITICAL = 0,
      ERROR,
      NOTIFICATION,
      WARNING,
      DEBUG,
      VERBOSE,
      LAST_VALUE
    } EN_LogLevel;
  }


  // //////// Type definitions /////////
  /** CRS code. */
  typedef std::string CRSCode_T;

  /** Airline code. */
  typedef std::string AirlineCode_T;

  /** Number of passengers (in a group) for a booking. */
  typedef unsigned short PartySize_T;
  
}
#endif // __SIMCRS_SIMCRS_TYPES_HPP

