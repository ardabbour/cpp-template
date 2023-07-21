include(CTest)

# Get googletest using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest URL https://github.com/google/googletest/archive/v1.13.0.tar.gz)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

# Set the test source files
set(TEST_SRC_FILES test_add.cc test_subtract.cc)

# Create a set of target names based on the set of test source file names
string(REPLACE ".cc" "" TEST_SRC_FILES_BASENAMES "${TEST_SRC_FILES}")
set(TEST_SRC_FILES_TARGETS ${TEST_SRC_FILES_BASENAMES})
set(TEST_LINK_LIBS ${PROJECT_NAME} GTest::gtest GTest::gtest_main)

# For each target name, build the test
foreach(TEST_NAME ${TEST_SRC_FILES_TARGETS})
  add_executable(${TEST_NAME} ${TEST_NAME}.cc)
  target_link_libraries(${TEST_NAME} PRIVATE ${TEST_LINK_LIBS})
  add_test(${TEST_NAME} ${TEST_NAME})
  gtest_discover_tests(${TEST_NAME})
endforeach()
