/*!
 * \page CRSTestSuite_cpp Command-Line Test to Demonstrate How To Test the SimCRS Project
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE CRSTestSuite
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
// SimCRS
#include <simcrs/SIMCRS_Service.hpp>
#include <simcrs/config/simcrs-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("CRSTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
#if BOOST_VERSION_MACRO >= 105900
    boost_utf::unit_test_log.set_format (boost_utf::OF_XML);
#else // BOOST_VERSION_MACRO
    boost_utf::unit_test_log.set_format (boost_utf::XML);
#endif // BOOST_VERSION_MACRO
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// //////////////////////////////////////////////////////////////////////
/**
 * Perform a simple simulation
 */
const unsigned int testSimCRSHelper (const unsigned short iTestFlag,
                                     const stdair::Filename_T& iScheduleInputFilename,
                                     const stdair::Filename_T& iOnDInputFilename,
                                     const stdair::Filename_T& iFRAT5InputFilename,
                                     const stdair::Filename_T& iFFDisutilityInputFilename,
                                     const stdair::Filename_T& iYieldInputFilename,
                                     const stdair::Filename_T& iFareInputFilename,
                                     const bool isBuiltin,
                                     const unsigned int iExpectedNbOfTravelSolutions,
                                     const unsigned int iExpectedPrice) {
   
  // CRS code
  const SIMCRS::CRSCode_T lCRSCode ("1P");

  // Output log File
  std::ostringstream oStr;
  oStr << "CRSTestSuite_" << iTestFlag << ".log";
  const stdair::Filename_T lLogFilename (oStr.str());

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode);

  stdair::Date_T lPreferredDepartureDate;;
  stdair::Date_T lRequestDate;
  stdair::TripType_T lTripType;

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the default sample BOM tree
    simcrsService.buildSampleBom();

    lPreferredDepartureDate = boost::gregorian::from_string ("2010/02/08");
    lRequestDate = boost::gregorian::from_string ("2010/01/21");
    lTripType = "OW";

  } else {

    // Build the BOM tree from parsing input files
    stdair::ScheduleFilePath lScheduleFilePath (iScheduleInputFilename);
    stdair::ODFilePath lODFilePath (iOnDInputFilename);
    stdair::FRAT5FilePath lFRAT5FilePath (iFRAT5InputFilename);
    stdair::FFDisutilityFilePath lFFDisutilityFilePath (iFFDisutilityInputFilename);
    const SIMFQT::FareFilePath lFareFilePath (iFareInputFilename);
    const AIRRAC::YieldFilePath lYieldFilePath (iYieldInputFilename);
    simcrsService.parseAndLoad (lScheduleFilePath, lODFilePath,
                                lFRAT5FilePath, lFFDisutilityFilePath,
                                lYieldFilePath, lFareFilePath);

    lPreferredDepartureDate = boost::gregorian::from_string ("2011/01/31");
    lRequestDate = boost::gregorian::from_string ("2011/01/22");
    lTripType = "RI";
  }

  // Create an empty booking request structure
  const stdair::AirportCode_T lOrigin ("SIN");
  const stdair::AirportCode_T lDestination ("BKK");
  const stdair::AirportCode_T lPOS ("SIN");
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(10));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Eco");
  const stdair::PartySize_T lPartySize (3);
  const stdair::ChannelLabel_T lChannel ("IN");
  const stdair::DayDuration_T lStayDuration (7);
  const stdair::FrequentFlyer_T lFrequentFlyerType ("M");
  const stdair::Duration_T lPreferredDepartureTime (boost::posix_time::hours(10));
  const stdair::WTP_T lWTP (1000.0);
  const stdair::PriceValue_T lValueOfTime (100.0);
  const stdair::ChangeFees_T lChangeFees (true);
  const stdair::Disutility_T lChangeFeeDisutility (50);
  const stdair::NonRefundable_T lNonRefundable (true);
  const stdair::Disutility_T lNonRefundableDisutility (50);
  const stdair::BookingRequestStruct lBookingRequest (lOrigin, lDestination,
                                                      lPOS,
                                                      lPreferredDepartureDate,
                                                      lRequestDateTime,
                                                      lPreferredCabin,
                                                      lPartySize, lChannel,
                                                      lTripType, lStayDuration,
                                                      lFrequentFlyerType,
                                                      lPreferredDepartureTime,
                                                      lWTP, lValueOfTime,
                                                      lChangeFees,
                                                      lChangeFeeDisutility,
                                                      lNonRefundable,
                                                      lNonRefundableDisutility);
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.calculateSegmentPathList (lBookingRequest);
  
  // Price the travel solution
  simcrsService.fareQuote (lBookingRequest, lTravelSolutionList);

  //
  const unsigned int lNbOfTravelSolutions = lTravelSolutionList.size();
  
  // DEBUG
  std::ostringstream oMessageKeptTS;
  oMessageKeptTS << "The number of travel solutions for the booking request '"
                 << lBookingRequest.describe() << "' is actually "
                 << lNbOfTravelSolutions << ". That number is expected to be "
                 << iExpectedNbOfTravelSolutions << ".";
  STDAIR_LOG_DEBUG (oMessageKeptTS.str());

  BOOST_CHECK_EQUAL (lNbOfTravelSolutions, iExpectedNbOfTravelSolutions);

  BOOST_CHECK_MESSAGE (lNbOfTravelSolutions == iExpectedNbOfTravelSolutions,
                       oMessageKeptTS.str());
  
  /**
   * Keep only the first travel solution. Given the assumption above, it is
   * also the only one.
   */
  stdair::TravelSolutionStruct& lTravelSolution = lTravelSolutionList.front();

  /**
   * Retrieve the list of fare options (e.g., the fares for all the
   * booking classes and associated conditions).
   */
  const stdair::FareOptionList_T& lFareOptionList =
    lTravelSolution.getFareOptionList();

  /**
   * Keep/choose only the fare option (associated to the corresponding
   * given booking class) appearing at the beginning of the list,
   * whatever it be.
   *
   * \note The "chosen" fare option is not necessarily the most expensive,
   *       nor it is necessarily the least expensive.
   */
  stdair::FareOptionStruct lFareOption = lFareOptionList.front();
  lTravelSolution.setChosenFareOption (lFareOption);
  
  // DEBUG
  std::ostringstream oMessageKeptFare;
  oMessageKeptFare
    << "The price given by the fare quoter for the booking request: '"
    << lBookingRequest.describe() << "' and travel solution: '"
    << lTravelSolution.describe() << "' is actually " << lFareOption.getFare()
    << " Euros. It is expected to be " << iExpectedPrice << " Euros.";
  STDAIR_LOG_DEBUG (oMessageKeptFare.str());

  BOOST_CHECK_EQUAL (std::floor (lFareOption.getFare() + 0.5), iExpectedPrice);

  BOOST_CHECK_MESSAGE (std::floor (lFareOption.getFare() + 0.5)
                       == iExpectedPrice, oMessageKeptFare.str());

  /**
   * Make a booking (reminder: party size is 3).
   *
   * \note The booking class is given in the travel solution, through the
   *       chosen fare option, which has been set just above (on the chosen
   *       TravelSolutionStruct instance).
   */
  // DEBUG
  STDAIR_LOG_DEBUG ("A booking will now (attempted to) be made on the "
                    "travel solution '" << lTravelSolution.describe()
                    << "', for a party size of " << lPartySize << ".");
  
  const bool isSellSuccessful =
    simcrsService.sell (lTravelSolution, lPartySize);

  // Close the log file
  logOutputFile.close();

  return isSellSuccessful;

}


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (simcrs_simple_simulation_test) {
    
  // Schedule input filename
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/rds01/schedule.csv");
    
  // O&D input filename
  const stdair::Filename_T lOnDInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");

  // FRAT5 curve input file name
  const stdair::Filename_T lFRAT5InputFilename (STDAIR_SAMPLE_DIR
                                               "/frat5.csv");

  // Fare family disutility curve input file name
  const stdair::Filename_T lFFDisutilityInputFilename (STDAIR_SAMPLE_DIR
                                                       "/ffDisutility.csv");
    
  // Yield input filename
  const stdair::Filename_T lYieldInputFilename (STDAIR_SAMPLE_DIR
                                                "/rds01/yield.csv");
    
  // Fare input filename
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR
                                               "/rds01/fare.csv");

  // State whether the BOM tree should be built-in or parsed from input files
  const bool isBuiltin = false;
  
  /**
   * As of September 2012, the fare option, kept for the travel solution
   * ('SQ;12,2011-Jan-31;SIN,BKK;08:20:00'), corresponds
   * to the Y booking class and is valued to 400 Euros.
   */
  const unsigned int lExpectedPrice = 400;
  const unsigned int lExpectedNbOfTravelSolutions = 1;

  bool isSellSuccessful = false;

  BOOST_CHECK_NO_THROW (isSellSuccessful =
                        testSimCRSHelper (0,
                                          lScheduleInputFilename,
                                          lOnDInputFilename,
                                          lFRAT5InputFilename,
                                          lFFDisutilityInputFilename,
                                          lYieldInputFilename,
                                          lFareInputFilename,
                                          isBuiltin,
                                          lExpectedNbOfTravelSolutions,
                                          lExpectedPrice));

  // DEBUG
  std::ostringstream oMessageSell;
  const std::string isSellSuccessfulStr = (isSellSuccessful == true)?"Yes":"No";
  oMessageSell << "Was the sell successful? Answer: " << isSellSuccessfulStr;
  STDAIR_LOG_DEBUG (oMessageSell.str());

  BOOST_CHECK_EQUAL (isSellSuccessful, true);

  BOOST_CHECK_MESSAGE (isSellSuccessful == true, oMessageSell.str());

 
}


/**
 * Test a simple simulation with a default BOM tree
 */
BOOST_AUTO_TEST_CASE (simcrs_simple_default_bom_simulation_test) {

  // State whether the BOM tree should be built-in or parsed from input files
  const bool isBuiltin = true;

  /**
   * As of September 2012, the fare option, kept for the travel solution
   * ('SQ;747,2011-Feb-08;SIN,BKK;06:35:00'), corresponds
   * to the M booking class and is valued to 900 Euros.
   */
  const unsigned int lExpectedPrice = 900;
  const unsigned int lExpectedNbOfTravelSolutions = 1;

  bool isSellSuccessful = false;

  BOOST_CHECK_NO_THROW (isSellSuccessful =
                        testSimCRSHelper (1,
                                          " ", " ", " ", " ", " ", " ",
                                          isBuiltin,
                                          lExpectedNbOfTravelSolutions,
                                          lExpectedPrice));

  // DEBUG
  std::ostringstream oMessageSell;
  const std::string isSellSuccessfulStr = (isSellSuccessful == true)?"Yes":"No";
  oMessageSell << "Was the sell successful? Answer: " << isSellSuccessfulStr;
  STDAIR_LOG_DEBUG (oMessageSell.str());

  BOOST_CHECK_EQUAL (isSellSuccessful, true);

  BOOST_CHECK_MESSAGE (isSellSuccessful == true, oMessageSell.str());

 
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

