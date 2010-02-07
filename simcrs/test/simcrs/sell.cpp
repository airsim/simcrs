// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
// SIMCRS
#include <simcrs/SIMCRS_Service.hpp>
#include <simcrs/config/simcrs-paths.hpp>

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  try {
    
    // CRS code
    std::string lCRSCode ("1P");
    
    // Airline code
    std::string lAirlineCode ("BA");
    
    // Number of passengers in the travelling group
    stdair::PartySize_T lPartySize = 5;
    
    // Output log File
    std::string lLogFilename ("sell.log");

    // Schedule input filename
    std::string lScheduleFilename ("../samples/schedule01.csv");
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // Open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();
    
    // Initialise the list of classes/buckets
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    SIMCRS::SIMCRS_Service simcrsService (lLogParams, lCRSCode,
                                          lScheduleFilename);

    // Make a booking
    //simcrsService.sell (lAirlineCode, lPartySize);
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;	
}
