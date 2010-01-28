#ifndef __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
#define __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// Boost
#include <boost/shared_ptr.hpp>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>
#include <simcrs/service/ServiceAbstract.hpp>

// Forward declarations
namespace stdair {
  class STDAIR_Service;
}

namespace AIRINV {
  class AIRINV_Service;
}

namespace AIRSCHED {
  class AIRSCHED_Service;
}

/** Pointer on the AIRINV Service handler. */
typedef boost::shared_ptr<AIRINV::AIRINV_Service> AIRINV_ServicePtr_T;

/** Pointer on the AIRSCHED Service handler. */
typedef boost::shared_ptr<AIRSCHED::AIRSCHED_Service> AIRSCHED_ServicePtr_T;

/** Typedef which defines a map of airline codes and the corresponding
    airline inventories. */
typedef std::map<const stdair::AirlineCode_T,
                 AIRINV_ServicePtr_T> AIRINV_ServicePtr_Map_T;


namespace SIMCRS {

  /** Pointer on the STDAIR Service handler. */
  typedef boost::shared_ptr<stdair::STDAIR_Service> STDAIR_ServicePtr_T;

  
  /** Class holding the context of the Simcrs services. */
  class SIMCRS_ServiceContext : public ServiceAbstract {
    /** The SIMCRS_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class SIMCRS_Service;
    friend class FacSimcrsServiceContext;
    
  private:
    /// //////////////// Constructors and destructors /////////////
    /** Constructor. */
    SIMCRS_ServiceContext (const CRSCode_T& iCRSCode);
    /** Default constructor (not to be used). */
    SIMCRS_ServiceContext ();
    /** Default copy constructor (not to be used). */
    SIMCRS_ServiceContext (const SIMCRS_ServiceContext&);

    /** Destructor. */
    ~SIMCRS_ServiceContext();

    
  private:
    // ///////////////// Getters ///////////////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_Service& getSTDAIR_Service () const;
    
    /** Get the CRS code. */
    const CRSCode_T& getCRSCode () const {
      return _CRSCode;
    }

    /** Get a reference on the AIRINV service handler which corresponds to
        the given airline code. */
    AIRINV::AIRINV_Service& getAIRINV_Service (const stdair::AirlineCode_T&) const;

    /** Get a reference on the AIRSCHED service handler. */
    AIRSCHED::AIRSCHED_Service& getAIRSCHED_Service () const;
    

    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }
    
    /** Set the CRS code. */
    void setCRSCode (const CRSCode_T& iCRSCode) {
      _CRSCode = iCRSCode;
    }

    /** Add the pointer on the AIRINV service handler to the dedicated list. */
    void addAIRINV_Service (const stdair::AirlineCode_T& iAirlineCode,
                            AIRINV_ServicePtr_T ioAIRINV_ServicePtr);

    /** Set the pointer on the AIRSCHED service handler. */
    void setAIRSCHED_Service (AIRSCHED_ServicePtr_T ioAIRSCHED_ServicePtr) {
      _airschedService = ioAIRSCHED_ServicePtr;
    }

    
    // ///////// Display Methods //////////
    /** Display the short SIMCRS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMCRS_ServiceContext content. */
    const std::string display() const;

    
  private:
    // ///////////// Children ////////////
    /** Airline Schedule Service Handler. */
    AIRSCHED_ServicePtr_T _airschedService;

    /** Airline Inventory Service Handler map. */
    AIRINV_ServicePtr_Map_T _airinvServiceMap;


  private:
    // ///////////// Children ////////////
    /** Standard Airline (StdAir) Service Handler. */
    STDAIR_ServicePtr_T _stdairService;

    
  private:
    // //////////// Attributes //////////////////
    /** CRS code. */
    CRSCode_T _CRSCode;
  };
  
}
#endif // __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
