#
# AvlCal Tests
#
module_set_name (avlcal)


##
# Specify each test suite configuration

# * AvlCalTestSuite Test Suite
module_test_add_suite (AvlCalTestSuite AvlCalTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
