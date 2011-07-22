/*!
 * \page FQTTestSuite_cpp Command-Line Test to Demonstrate How To Test the SimFQT Project
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
#define BOOST_TEST_MODULE FQTTestSuite
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
// SimFQT
#include <simfqt/SIMFQT_Service.hpp>
#include <simfqt/config/simfqt-paths.hpp>

namespace boost_utf = boost::unit_test;

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    static std::ofstream _test_log ("FQTTestSuite_utfresults.xml");
    boost_utf::unit_test_log.set_stream (_test_log);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// //////////////////////////////////////////////////////////////////////
/**
 * Build and fareQuote a default list of travel solutions
 */
void testFareQuoterHelper (const unsigned short iTestFlag,
                           const stdair::Filename_T iFareInputFilename) {

  // Output log File
  std::ostringstream oStr;
  oStr << "FQTTestSuite_" << iTestFlag << ".log";
  const stdair::Filename_T lLogFilename (oStr.str());

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the SimFQT service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG,
                                         logOutputFile);
  
  // Initialise the Simfqt service object
  SIMFQT::SIMFQT_Service simfqtService (lLogParams);

  // Build the BOM tree from parsing the fare input file
  simfqtService.parseAndLoad (iFareInputFilename);

  // Build a sample list of travel solutions and a booking request.
  stdair::TravelSolutionList_T lTravelSolutionList;
  simfqtService.buildSampleTravelSolutions (lTravelSolutionList);
  stdair::BookingRequestStruct lBookingRequest =
    simfqtService.buildBookingRequest();

  // Try to fareQuote the sample list of travel solutions
  simfqtService.quotePrices (lBookingRequest, lTravelSolutionList);

  // Close the log file
  logOutputFile.close();

}

// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple price quotation
 */
BOOST_AUTO_TEST_CASE (simfqt_simple_pricing_test) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_NO_THROW (testFareQuoterHelper (0, lFareInputFilename));
 
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::AirportPairNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_01) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError01.csv");

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (1, lFareInputFilename),
                     SIMFQT::AirportPairNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::PosOrChannelNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_02) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError02.csv");

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (2, lFareInputFilename),
                     SIMFQT::PosOrChannelNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FlightDateNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_03) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError03.csv");

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (3, lFareInputFilename),
                     SIMFQT::FlightDateNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FlightTimeNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_04) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError04.csv");
  
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (4, lFareInputFilename),
                     SIMFQT::FlightTimeNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FeaturesNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_05) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError05.csv");

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (5, lFareInputFilename),
                     SIMFQT::FeaturesNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::AirlineNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_06) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError06.csv");
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (6, lFareInputFilename),
                     SIMFQT::AirlineNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FareFileParsingFailedException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_07) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError07.csv");
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (7, lFareInputFilename),
                     SIMFQT::FareFileParsingFailedException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FareInputFileNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_08) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/missingFile.csv");
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (8, lFareInputFilename),
                     SIMFQT::FareInputFileNotFoundException);
}


// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
