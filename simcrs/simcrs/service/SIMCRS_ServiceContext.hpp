#ifndef __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
#define __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>
#include <simcrs/service/ServiceAbstract.hpp>

// Forward declarations
namespace AIRINV {
  class AIRINV_Service;
}

/** Pointer on the AIRINV Service handler. */
typedef boost::shared_ptr<AIRINV::AIRINV_Service> AIRINV_ServicePtr_T;


namespace SIMCRS {

  /** Class holding the context of the Simcrs services. */
  class SIMCRS_ServiceContext : public ServiceAbstract {
    friend class FacSimcrsServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the CRS code. */
    const CRSCode_T& getCRSCode () const {
      return _CRSCode;
    }

    /** Get a reference on the AIRINV service handler. */
    AIRINV::AIRINV_Service& getAIRINV_Service () const {
      return *_airinvService.get();
    }

    
    // ///////// Setters //////////
    /** Set the CRS code. */
    void setCRSCode (const CRSCode_T& iCRSCode) {
      _CRSCode = iCRSCode;
    }

    /** Set the pointer on the AIRINV service handler. */
    void setAIRINV_Service (AIRINV_ServicePtr_T ioAIRINV_ServicePtr) {
      _airinvService = ioAIRINV_ServicePtr;
    }

    
    // ///////// Display Methods //////////
    /** Display the short SIMCRS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMCRS_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    SIMCRS_ServiceContext ();
    SIMCRS_ServiceContext (const CRSCode_T& iCRSCode);
    SIMCRS_ServiceContext (const SIMCRS_ServiceContext&);

    /** Destructor. */
    ~SIMCRS_ServiceContext();

    
  private:
    // ///////////// Children ////////////
    /** Airline Inventory Service Handler. */
    AIRINV_ServicePtr_T _airinvService;


  private:
    // //////////// Attributes //////////////////
    /** CRS code. */
    CRSCode_T _CRSCode;
  };

}
#endif // __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
