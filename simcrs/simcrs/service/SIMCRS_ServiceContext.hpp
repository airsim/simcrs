#ifndef __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
#define __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Simcrs
#include <simcrs/SIMCRS_Types.hpp>
#include <simcrs/service/ServiceAbstract.hpp>

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

    
    // ///////// Setters //////////
    /** Set the CRS code. */
    void setCRSCode (const CRSCode_T& iCRSCode) {
      _CRSCode = iCRSCode;
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
    // //////////// Attributes //////////////////
    /** CRS code. */
    CRSCode_T _CRSCode;
  };

}
#endif // __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
