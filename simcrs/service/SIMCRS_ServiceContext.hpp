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
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
// AirTSP
#include <airtsp/AIRTSP_Types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>
#include <simcrs/service/ServiceAbstract.hpp>

namespace SIMCRS {

  /**
   * @brief Class holding the context of the Simcrs services.
   */
  class SIMCRS_ServiceContext : public ServiceAbstract {
    /**
     * The SIMCRS_Service class should be the sole class to get access
     * to ServiceContext content: general users do not want to bother
     * with a context interface.
     */
    friend class SIMCRS_Service;
    friend class FacSimcrsServiceContext;
    
  private:
    // ///////////////// Getters ///////////////////
    /**
     * Get the CRS code (identifier of the current SimCRS component).
     *
     * \note: this identifier is not used for now.
     */
    const CRSCode_T& getCRSCode() const {
      return _CRSCode;
    }

    /**
     * Get the pointer on the STDAIR service handler.
     */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr() const {
      return _stdairService;
    }

    /**
     * Get the STDAIR service handler.
     */
    stdair::STDAIR_Service& getSTDAIR_Service() const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

    /**
     * State whether or not SIMCRS_Service owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }  

    /**
     * Get the pointer on the SEvMgr service handler.
     */
    SEVMGR::SEVMGR_ServicePtr_T getSEVMGR_ServicePtr() const {
      return _sevmgrService;
    }

    /**
     * Get the SEvMgr service handler.
     */
    SEVMGR::SEVMGR_Service& getSEVMGR_Service() const {
      assert (_sevmgrService != NULL);
      return *_sevmgrService;
    } 

    /**
     * State whether or not SIMCRS_Service owns the SEVMGR service resources.
     */
    const bool getOwnSEVMGRServiceFlag() const {
      return _ownSEVMGRService;
    }  

    /**
     * Get the AirInv service handler.
     */
    AIRINV::AIRINV_Master_Service& getAIRINV_Service() const {
      assert (_airinvService != NULL);
      return *_airinvService;
    }

    /**
     * Get the AirTSP service handler.
     */
    AIRTSP::AIRTSP_Service& getAIRTSP_Service() const {
      assert (_airtspService != NULL);
      return *_airtspService;
    }

    /**
     * Get the SimFQT service handler.
     */
    SIMFQT::SIMFQT_Service& getSIMFQT_Service() const {
      assert (_simfqtService != NULL);
      return *_simfqtService;
    }


  private:
    // ///////////////// Setters ///////////////////
    /**
     * Set the CRS code.
     */
    void setCRSCode (const CRSCode_T& iCRSCode) {
      _CRSCode = iCRSCode;
    }

    /**
     * Set the pointer on the STDAIR service handler.
     */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                            const bool iOwnStdairService) {
      _stdairService = ioSTDAIR_ServicePtr;
      _ownStdairService = iOwnStdairService;
    }

    /**
     * Set the pointer on the SEvMgr service handler.
     */
    void setSEVMGR_Service (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
			    const bool iOwnSEVMGRService) {
      _sevmgrService = ioSEVMGR_ServicePtr;
      _ownSEVMGRService = iOwnSEVMGRService;
    }
    
    /**
     * Set the pointer on the AirInv service handler.
     */
    void setAIRINV_Service (AIRINV::AIRINV_Master_ServicePtr_T ioServicePtr) {
      _airinvService = ioServicePtr;
    }
    
    /**
     * Set the pointer on the AirTSP service handler.
     */
    void setAIRTSP_Service (AIRTSP::AIRTSP_ServicePtr_T ioServicePtr) {
      _airtspService = ioServicePtr;
    }
    
    /**
     * Set the pointer on the SimFQT service handler.
     */
    void setSIMFQT_Service (SIMFQT::SIMFQT_ServicePtr_T ioServicePtr) {
      _simfqtService = ioServicePtr;
    }
    

  private:
    // //////////////////// Display Methods /////////////////////
    /**
     * Display the short AIRINV_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full AIRINV_ServiceContext content.
     */
    const std::string display() const;
    
    /**
     * Display of the structure.
     */
    const std::string describe() const;


  private:
    /// //////////////// Constructors and destructors /////////////
    /**
     * Main constructor.
     */
    SIMCRS_ServiceContext (const CRSCode_T& iCRSCode);
    /**
     * Default constructor (not to be used).
     */
    SIMCRS_ServiceContext();
    /**
     * Copy constructor (not to be used).
     */
    SIMCRS_ServiceContext (const SIMCRS_ServiceContext&);

    /**
     * Destructor.
     */
    ~SIMCRS_ServiceContext();

    /**
     * Clear the context (cabin capacity, bucket holder).
     */
    void reset();


  private:
    /**
     * Standard Airline (StdAir) Service Handler.
     */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /**
     * State whether or not AIRINV owns the STDAIR service resources.
     */
    bool _ownStdairService;   

    /**
     * Standard event queue (SEvMgr) Service Handler.
     */
    SEVMGR::SEVMGR_ServicePtr_T _sevmgrService;  

    /**
     * State whether or not AIRINV owns the SEVMGR service resources.
     */
    bool _ownSEVMGRService;   

    /**
     * Airline Schedule Service Handler.
     */
    AIRTSP::AIRTSP_ServicePtr_T _airtspService;

    /**
     * Airline Inventory Service Handler.
     */
    AIRINV::AIRINV_Master_ServicePtr_T _airinvService;

    /**
     * Fare Quote Service Handler.
     */
    SIMFQT::SIMFQT_ServicePtr_T _simfqtService;

    
  private:
    // /////////////// Attributes //////////////////
    /**
     * CRS code (identifier of the current SimCRS component).
     *
     * \note: this identifier is not used for now.
     */
    CRSCode_T _CRSCode;
  };
  
}
#endif // __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
