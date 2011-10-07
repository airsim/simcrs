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
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (simcrs_simple_simulation_test) {

  // CRS code
  const SIMCRS::CRSCode_T lCRSCode ("1P");
    
  // Schedule input filename
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/rds01/schedule.csv");
    
  // O&D input filename
  const stdair::Filename_T lOnDInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");
    
  // Yield input filename
  const stdair::Filename_T lYieldInputFilename (STDAIR_SAMPLE_DIR
                                                "/rds01/yield.csv");
    
  // Fare input filename
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR
                                               "/rds01/fare.csv");
    
  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lScheduleInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lOnDInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lOnDInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lYieldInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lYieldInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lFareInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lFareInputFilename
                       << "' input file can not be open and read");

  // Output log File
  const stdair::Filename_T lLogFilename ("CRSTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode);

  // Build the BOM tree from parsing input files
  const SIMFQT::FareFilePath lFareFilePath (lFareInputFilename);
  const AIRRAC::YieldFilePath lYieldFilePath (lYieldInputFilename);
  simcrsService.parseAndLoad (lScheduleInputFilename, lOnDInputFilename,
                              lYieldFilePath, lFareFilePath);

  // Create an empty booking request structure
  // TODO: fill the booking request structure from the input parameters
  const stdair::AirportCode_T lOrigin ("SIN");
  const stdair::AirportCode_T lDestination ("BKK");
  const stdair::AirportCode_T lPOS ("SIN");
  const stdair::Date_T lPreferredDepartureDate(2011, boost::gregorian::Jan, 31);
  const stdair::Date_T lRequestDate (2011, boost::gregorian::Jan, 22);
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(10));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Eco");
  const stdair::PartySize_T lPartySize (3);
  const stdair::ChannelLabel_T lChannel ("IN");
  const stdair::TripType_T lTripType ("RI");
  const stdair::DayDuration_T lStayDuration (7);
  const stdair::FrequentFlyer_T lFrequentFlyerType ("M");
  const stdair::Duration_T lPreferredDepartureTime (boost::posix_time::hours(10));
  const stdair::WTP_T lWTP (1000.0);
  const stdair::PriceValue_T lValueOfTime (100.0);
  const stdair::BookingRequestStruct lBookingRequest (lOrigin, lDestination,
                                                      lPOS,
                                                      lPreferredDepartureDate,
                                                      lRequestDateTime,
                                                      lPreferredCabin,
                                                      lPartySize, lChannel,
                                                      lTripType, lStayDuration,
                                                      lFrequentFlyerType,
                                                      lPreferredDepartureTime,
                                                      lWTP, lValueOfTime);
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.calculateSegmentPathList (lBookingRequest);
  
  // Price the travel solution
  simcrsService.fareQuote (lBookingRequest, lTravelSolutionList);

  //
  const unsigned int lNbOfTravelSolutions = lTravelSolutionList.size();

  // \todo change the expected number of travel solutions to the actual number
  const unsigned int lExpectedNbOfTravelSolutions = 1;
  
  // DEBUG
  std::ostringstream oMessageKeptTS;
  oMessageKeptTS << "The number of travel solutions for the booking request '"
                 << lBookingRequest.describe() << "' is actually "
                 << lNbOfTravelSolutions << ". That number is expected to be "
                 << lExpectedNbOfTravelSolutions << ".";
  STDAIR_LOG_DEBUG (oMessageKeptTS.str());

  BOOST_CHECK_EQUAL (lNbOfTravelSolutions, lExpectedNbOfTravelSolutions);

  BOOST_CHECK_MESSAGE (lNbOfTravelSolutions == lExpectedNbOfTravelSolutions,
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

  /**
   * As of August 2011, the fare option, kept for the travel solution
   * given above ('SQ;12,2011-Jan-31;SIN,BKK;08:20:00'), corresponds
   * to the Y booking class and is valued to 400 Euros.
   */
  const unsigned int lExpectedPrice = 400;
  
  // DEBUG
  std::ostringstream oMessageKeptFare;
  oMessageKeptFare
    << "The price given by the fare quoter for the booking request: '"
    << lBookingRequest.describe() << "' and travel solution: '"
    << lTravelSolution.describe() << "' is actually " << lFareOption.getFare()
    << " Euros. It is expected to be " << lExpectedPrice << " Euros.";
  STDAIR_LOG_DEBUG (oMessageKeptFare.str());

  BOOST_CHECK_EQUAL (std::floor (lFareOption.getFare() + 0.5), lExpectedPrice);

  BOOST_CHECK_MESSAGE (std::floor (lFareOption.getFare() + 0.5)
                       == lExpectedPrice, oMessageKeptFare.str());

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
  //BOOST_CHECK_NO_THROW ();

  // DEBUG
  std::ostringstream oMessageSell;
  const std::string isSellSuccessfulStr = (isSellSuccessful == true)?"Yes":"No";
  oMessageSell << "Was the sell successful? Answer: " << isSellSuccessfulStr;
  STDAIR_LOG_DEBUG (oMessageSell.str());

  BOOST_CHECK_EQUAL (isSellSuccessful, true);

  BOOST_CHECK_MESSAGE (isSellSuccessful == true, oMessageSell.str());

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

