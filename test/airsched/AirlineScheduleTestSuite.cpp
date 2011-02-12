/*!
 * \page AirlineScheduleTestSuite_cpp Command-Line Test to Demonstrate How To Test the AirSched Project
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE InventoryTestSuite
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/AIRSCHED_Service.hpp>
#include <airsched/config/airsched-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("AirlineScheduleTestSuite_utfresults.xml");

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
 * Test a simple inventory sell
 */
BOOST_AUTO_TEST_CASE (airsched_simple_inventory_sell) {

  // Input file name
  const stdair::Filename_T lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                                   "/schedule03.csv");

  // Output log File
  const stdair::Filename_T lLogFilename ("AirlineScheduleTestSuite.log");

  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lScheduleInputFilename
                       << "' input file can not be open and read");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  AIRSCHED::AIRSCHED_Service airschedService (lLogParams,
                                              lScheduleInputFilename);

  // Create an empty booking request structure
  // TODO: fill the booking request structure from the input parameters
  const stdair::AirportCode_T lOrigin ("NCE");
  const stdair::AirportCode_T lDestination ("BKK");
  const stdair::AirportCode_T lPOS ("NCE");
  const stdair::Date_T lPreferredDepartureDate(2007, boost::gregorian::Apr, 21);
  const stdair::Date_T lRequestDate (2007, boost::gregorian::Mar, 21);
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(8));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Bus");
  const stdair::PartySize_T lPartySize (3);
  const stdair::ChannelLabel_T lChannel ("DF");
  const stdair::TripType_T lTripType ("RO");
  const stdair::DayDuration_T lStayDuration (5);
  const stdair::FrequentFlyer_T lFrequentFlyerType ("NONE");
  const stdair::Duration_T lPreferredDepartureTime (boost::posix_time::hours(10));
  const stdair::WTP_T lWTP (2000.0);
  const stdair::PriceValue_T lValueOfTime (20.0);
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
    
  //
  stdair::SegmentPathList_T lSegmentPathList;
  airschedService.buildSegmentPathList (lSegmentPathList, lBookingRequest);
  const unsigned int lNbOfTravelSolutions = lSegmentPathList.size();
  
  // TODO: change the expected number of travel solutions to the actual number
  const unsigned int lExpectedNbOfTravelSolutions = 4;
  
  // DEBUG
  STDAIR_LOG_DEBUG ("Number of travel solutions for the booking request '"
                    << lBookingRequest.describe() << "': "
                    << lNbOfTravelSolutions << ". It is expected to be "
                    << lExpectedNbOfTravelSolutions << ".");

  BOOST_CHECK_EQUAL (lNbOfTravelSolutions, lExpectedNbOfTravelSolutions);

  BOOST_CHECK_MESSAGE(lNbOfTravelSolutions == lExpectedNbOfTravelSolutions,
                      "The number of travel solutions for the booking request '"
                      << lBookingRequest.describe() << "' is equal to "
                      << lNbOfTravelSolutions << ", but it should be equal to "
                      << lExpectedNbOfTravelSolutions);

  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

