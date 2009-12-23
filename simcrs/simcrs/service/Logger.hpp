#ifndef __SIMCRS_SVC_LOGGER_HPP
#define __SIMCRS_SVC_LOGGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
// SIMCRS
#include <simcrs/SIMCRS_Types.hpp>

// /////////////// LOG MACROS /////////////////
#define SIMCRS_LOG_CORE(iLevel, iToBeLogged) \
  { std::ostringstream ostr; ostr << iToBeLogged; \
    SIMCRS::Logger::instance().log (iLevel, __LINE__, __FILE__, ostr.str()); }

#define SIMCRS_LOG_CRITICAL(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::CRITICAL, iToBeLogged)

#define SIMCRS_LOG_ERROR(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::ERROR, iToBeLogged)

#define SIMCRS_LOG_NOTIFICATION(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::NOTIFICATION, iToBeLogged)

#define SIMCRS_LOG_WARNING(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::WARNING, iToBeLogged)

#define SIMCRS_LOG_DEBUG(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::DEBUG, iToBeLogged)

#define SIMCRS_LOG_VERBOSE(iToBeLogged) \
  SIMCRS_LOG_CORE (SIMCRS::LOG::VERBOSE, iToBeLogged)
// /////////// (END OF) LOG MACROS /////////////


namespace SIMCRS {

  /** Class holding the stream for logs. 
      <br>Note that the error logs are seen as standard output logs, 
      but with a higher level of visibility. */
  class Logger {
    // Friend classes
    friend class FacSupervisor;
  public:
    
    /** Main log entry. */
    template <typename T>
    void log (const LOG::EN_LogLevel iLevel, const int iLineNumber,
              const std::string& iFileName, const T& iToBeLogged) {
      if (iLevel <= _level) {
        assert (_logStream != NULL);
        *_logStream << iFileName << ":" << iLineNumber
                    << ": " << iToBeLogged << std::endl;
      }
    }
    
    /** Get the log level. */
    LOG::EN_LogLevel getLogLevel();
    
    /** get the log stream. */
    std::ostream& getLogStream();
    
    /** Set the logger parameters (level and stream). */
    void setLogParameters (const LOG::EN_LogLevel iLogLevel, 
                           std::ostream& ioLogStream);
    
    /** Returns a current Logger instance.*/
    static Logger& instance();
    
  private:
    /** Default constructors are private so that only the required 
        constructor can be used. */
    Logger ();
    Logger (const Logger&);
    Logger (const LOG::EN_LogLevel iLevel, std::ostream& ioLogStream);
    /** Destructor. */
    ~Logger ();
    
  private:
    /** Log level. */
    LOG::EN_LogLevel _level;
    
    /** Stream dedicated to the logs. */
    std::ostream* _logStream;
    
    /** Instance object.*/
    static Logger* _instance;
  };
  
}
#endif // __SIMCRS_SVC_LOGGER_HPP
