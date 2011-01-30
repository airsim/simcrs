/*!
 * \page CRSTestSuite_cpp Command-Line Test to Demonstrate How To Test the SimCRS Project
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_MODULE CRSTestSuite
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// SimCRS
#include <simcrs/SIMCRS_Service.hpp>
#include <simcrs/config/simcrs-paths.hpp>

namespace boost_utf = boost::unit_test;

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() : _test_log ("CRSTestSuite_results.xml")  {
    boost_utf::unit_test_log.set_stream (_test_log);
    boost_utf::unit_test_log.set_format (boost_utf::XML);
  }
  /** Destructor. */
  ~UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (std::cout);
  }
  /** Log file */  
  std::ofstream _test_log;
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
  const std::string lCRSCode ("1P");
    
  // Schedule input filename
  const std::string lScheduleInputFilename (STDAIR_SAMPLE_DIR
                                            "/schedule01.csv");
    
  // O&D input filename
  const std::string lODInputFilename (STDAIR_SAMPLE_DIR "/ond01.csv");
    
  // Fare input filename
  const std::string lFareInputFilename (STDAIR_SAMPLE_DIR "/fare01.csv");
    
  // Check that the file path given as input corresponds to an actual file
  bool doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lScheduleInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lScheduleInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lODInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lODInputFilename
                       << "' input file can not be open and read");

  // Check that the file path given as input corresponds to an actual file
  doesExistAndIsReadable =
    stdair::BasFileMgr::doesExistAndIsReadable (lFareInputFilename);
  BOOST_CHECK_MESSAGE (doesExistAndIsReadable == true,
                       "The '" << lFareInputFilename
                       << "' input file can not be open and read");

  // Output log File
  const std::string lLogFilename ("CRSTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Initialise the list of classes/buckets
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode,
                                        lScheduleInputFilename,
                                        lODInputFilename, lFareInputFilename);

  // Create an empty booking request structure
  // TODO: fill the booking request structure from the input parameters
  const stdair::AirportCode_T lOrigin ("SIN");
  const stdair::AirportCode_T lDestination ("BKK");
  const stdair::AirportCode_T lPOS ("SIN");
  const stdair::Date_T lPreferredDepartureDate(2010, boost::gregorian::Jan, 30);
  const stdair::Date_T lRequestDate (2010, boost::gregorian::Jan, 22);
  const stdair::Duration_T lRequestTime (boost::posix_time::hours(10));
  const stdair::DateTime_T lRequestDateTime (lRequestDate, lRequestTime);
  const stdair::CabinCode_T lPreferredCabin ("Eco");
  const stdair::NbOfSeats_T lPartySize (1);
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
  const stdair::SegmentPathList_T lSegmentPath;
  
  // Price the travel solution
  stdair::TravelSolutionList_T lTravelSolutionList =
    simcrsService.getFareQuote (lBookingRequest, lSegmentPath);

  //
  const unsigned int lNbOfTravelSolutions = lTravelSolutionList.size();

  // TODO: change the expected number of travel solutions to the actual number
  const unsigned int lExpectedNbOfTravelSolutions = 0;
  
  // DEBUG
  STDAIR_LOG_DEBUG ("Number of travel solutions for the booking request '"
                    << lBookingRequest.describe() << "': "
                    << lNbOfTravelSolutions << ". It is expected to be "
                    << lExpectedNbOfTravelSolutions);

  BOOST_CHECK_EQUAL (lNbOfTravelSolutions, lExpectedNbOfTravelSolutions);

  BOOST_CHECK_MESSAGE(lNbOfTravelSolutions == lExpectedNbOfTravelSolutions,
                      "The number of travel solutions for the booking request '"
                      << lBookingRequest.describe() << "' is equal to "
                      << lNbOfTravelSolutions << ", but it should be equal to "
                      << lExpectedNbOfTravelSolutions);
  
  /* TODO: uncomment as soon as travel solutions are found for that
     booking request
  //
  const stdair::TravelSolutionStruct& lTravelSolution =
    lTravelSolutionList.front();

  //  
  const unsigned int lExpectedPrice = 1000;
  
  // DEBUG
  STDAIR_LOG_DEBUG ("The price given by the fare quoter for '"
                    << lTravelSolution.describe() << "' is: "
                    << lTravelSolution.getFare() << " Euros, and should be "
                    << lExpectedPrice);

  BOOST_CHECK_EQUAL (std::floor (lTravelSolution.getFare() + 0.5),
                     lExpectedPrice);

  BOOST_CHECK_MESSAGE (std::floor (lTravelSolution.getFare() + 0.5)
                       == lExpectedPrice,
                       "The price given by the fare quoter for '"
                       << lTravelSolution.describe() << "' is: "
                       << lTravelSolution.getFare() << " Euros, and should be "
                       << lExpectedPrice);
  */

  // Make a booking
  // const std::string lAirlineCode ("SV");
  // const stdair::PartySize_T lPartySize = 5;
  // BOOST_CHECK_NO_THROW (simcrsService.sell (lAirlineCode, lPartySize));

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

