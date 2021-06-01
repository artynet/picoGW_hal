# -- Minimum required version
cmake_minimum_required (VERSION 3.2)

# -- Project name
project (picogw_hal)

# -- Various includes
include (CMakePackageConfigHelpers)
include (GNUInstallDirs)
#include (CheckFunctionExists)

# -- set c99 standard default
set(CMAKE_C_STANDARD 99)

# -- options for shared lib (defaults off)
option(picogw_hal_build_shared_libs "build as a shared library" OFF)
set(BUILD_SHARED_LIBS ${picogw_hal_build_shared_libs})

# -- Required to build
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# -- Versioning with git tag
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  execute_process(
    COMMAND git describe --tags --always
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE "picogw_hal_VERSION"
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(picogw_hal_VERSION STREQUAL "")
    set(picogw_hal_VERSION 0)
  endif(picogw_hal_VERSION STREQUAL "")
  message( STATUS "Git full version: ${picogw_hal_VERSION}" )
  execute_process(
    COMMAND /bin/bash -c "git describe --tags --abbrev=0 | cut --delimiter='v' --fields=2"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VARIABLE "picogw_hal_VERSION_SHORT"
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(picogw_hal_VERSION_SHORT STREQUAL "")
    set(picogw_hal_VERSION_SHORT 0)
  endif(picogw_hal_VERSION_SHORT STREQUAL "")
  message( STATUS "Git version: ${picogw_hal_VERSION_SHORT}" )
else(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  set(picogw_hal_VERSION_SHORT 0)
  set(picogw_hal_VERSION 0)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
if (NOT (${CMAKE_INSTALL_PREFIX} STREQUAL "/usr" ) )
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

# -- add the core library
add_subdirectory(libloragw)

# -- add util_boot
add_subdirectory(util_boot)

# -- add util_chip_id
add_subdirectory(util_chip_id)

# -- add util_pkt_logger
add_subdirectory(util_pkt_logger)

# -- add util_com_stress
add_subdirectory(util_com_stress)

# -- add util_tx_continuous
add_subdirectory(util_tx_continuous)

# -- add util_tx_test
# add_subdirectory(util_tx_test)
