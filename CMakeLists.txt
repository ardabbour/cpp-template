# Define minimum CMake version
# ##############################################################################
cmake_minimum_required(VERSION 3.24)
# ##############################################################################

# Define project properties
# ##############################################################################
project(
  MyProject
  VERSION 1.0.0
  DESCRIPTION "MyProject is a C++ library for adding and subtracting numbers."
  HOMEPAGE_URL "https://github.com/ardabbour/cpp-template"
  LANGUAGES CXX)
# ##############################################################################

# Common settings
# ##############################################################################
# Default to release with debug information
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE
      "RelWithDebInfo"
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Turn on all complier warning flags (except for warnings as errors)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wdeprecated)
endif()
# ##############################################################################

# Find the required dependencies (if needed)
# ##############################################################################
# cmake-format: off
# # Add our library-finding helpers
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies)

# # On debian systems, this is a useful install location
# list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/.local)

# find_package(Dep1)
# find_package(Dep2)
# cmake-format: on
# ##############################################################################

# Set the source and include files and the libraries to be linked with
# ##############################################################################
set(SRC_FILES src/Add.cc src/Subtract.cc)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
# set(LINK_LIBS Dep1::Dep1Sub1 Dep1::Dep1Sub2 Dep2::Dep2Sub1 Dep2::Dep2Sub2)
# ##############################################################################

# Use the standard GNU install directories
# ##############################################################################
include(GNUInstallDirs)
# ##############################################################################

# Build the library
# ##############################################################################
option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" OFF)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIRS}>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBS})
# ##############################################################################

# Build the executable(s)
# ##############################################################################
add_executable(${PROJECT_NAME}Adder src/Adder.cc)
target_include_directories(${PROJECT_NAME}Adder PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}Adder PRIVATE ${PROJECT_NAME})

add_executable(${PROJECT_NAME}Subtractor src/Subtractor.cc)
target_include_directories(${PROJECT_NAME}Subtractor PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}Subtractor PRIVATE ${PROJECT_NAME})
# ##############################################################################

# Use Doxygen to build the documentation
# ##############################################################################
option(BUILD_DOCUMENTATION "Builds documentation with Doxygen" ON)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_FILE_PATTERNS *cc *.h *.md)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_REFERENCED_BY_RELATION YES)
    set(DOXYGEN_REFERENCES_RELATION YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    doxygen_add_docs(
      doxygen ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/include ALL
      COMMENT "Generate HTML and man page documentation")
  else()
    message("Failed to build the documentation because Doxygen was not found.")
  endif()
endif()
# ##############################################################################

# Use Google Test to build the tests
# ##############################################################################
option(BUILD_TESTING "Builds tests with Google Test" ON)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  set(INSTALL_GMOCK OFF)
  set(INSTALL_GTEST OFF)
  add_subdirectory(tests)
endif()
# ##############################################################################

# Install the library, executables, and header
# ##############################################################################
install(
  TARGETS ${PROJECT_NAME} ${PROJECT_NAME}Adder ${PROJECT_NAME}Subtractor
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# ##############################################################################

# Create and install cmake helpers
# ##############################################################################
set(CMAKE_HELPERS_LOCATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_HELPERS_LOCATION})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_HELPERS_LOCATION})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_HELPERS_LOCATION})
# ##############################################################################

# Add an uninstall target
# ##############################################################################
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
  add_custom_target(
    uninstall
    COMMENT "Deletes files listed in the install manifest."
    COMMAND ${CMAKE_COMMAND} -P
            ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
# ##############################################################################

# Use CPack to create a package
# ##############################################################################
set(CPACK_PACKAGE_VENDOR "MyCompany")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "MyProject is a C++ library for adding and subtracting numbers.")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_CONTACT "ardabbour@gmail.com")
include(CPack)
# ##############################################################################
