#
# SimFQT Tests
#
module_set_name (simfqt)


##
# Specify each test suite configuration

# * SimFQT Test Suite
module_test_add_suite (FQTTestSuite FQTTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
