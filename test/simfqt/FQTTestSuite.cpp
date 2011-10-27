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
                           const stdair::Filename_T iFareInputFilename,
                           const bool isBuiltin) {

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

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the default sample BOM tree (filled with fares) for Simfqt
    simfqtService.buildSampleBom();

  } else {

    // Build the BOM tree from parsing the fare input file    
    SIMFQT::FareFilePath lFareFilePath (iFareInputFilename);
    simfqtService.parseAndLoad (lFareFilePath);
  }

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

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_NO_THROW (testFareQuoterHelper (0, lFareInputFilename, isBuiltin));
 
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::AirportPairNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_01) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError01.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (1, lFareInputFilename, isBuiltin),
                     SIMFQT::AirportPairNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::PosOrChannelNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_02) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError02.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (2, lFareInputFilename, isBuiltin),
                     SIMFQT::PosOrChannelNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FlightDateNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_03) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError03.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (3, lFareInputFilename, isBuiltin),
                     SIMFQT::FlightDateNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FlightTimeNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_04) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError04.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
  
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (4, lFareInputFilename, isBuiltin),
                     SIMFQT::FlightTimeNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FeaturesNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_05) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError05.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;

  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (5, lFareInputFilename, isBuiltin),
                     SIMFQT::FeaturesNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::AirlineNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_06) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError06.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (6, lFareInputFilename, isBuiltin),
                     SIMFQT::AirlineNotFoundException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FareFileParsingFailedException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_07) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/fareError07.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (7, lFareInputFilename, isBuiltin),
                     SIMFQT::FareFileParsingFailedException);
}

/**
 * Test an error detection functionality
 * Expected to throw SIMFQT::FareInputFileNotFoundException
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_08) {

  // Input file name
  const stdair::Filename_T lFareInputFilename (STDAIR_SAMPLE_DIR "/missingFile.csv");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_THROW (testFareQuoterHelper (8, lFareInputFilename, isBuiltin),
                     SIMFQT::FareInputFileNotFoundException);
}

/**
 * Test a simple price quotation using the default BOM tree
 * This test corresponds to the "-b" option of the batch.
 */
BOOST_AUTO_TEST_CASE (simfqt_error_pricing_test_09) {

  // Input file name
  const stdair::Filename_T lEmptyInputFilename (STDAIR_SAMPLE_DIR "/ ");

  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = true;
    
  // Try to fareQuote the sample default list of travel solutions
  BOOST_CHECK_NO_THROW(testFareQuoterHelper (9, lEmptyInputFilename, isBuiltin));
}


// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
