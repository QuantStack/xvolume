############################################################################
# Copyright (c) 2017, Sylvain Corlay, Johan Mabille and Loic Gouarin       #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.8)
project(xvolume)

message(STATUS "Forcing tests build type to Release")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

set(XVOLUME_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(XVOLUME_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Configuration
# =============

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(XVOLUME_INSTALL_LIBRARY_DIR "\"${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}\"")

configure_file (
    "${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_config_cling.hpp.in"
    "${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_config_cling.hpp"
)

# Versionning
# ===========

file(STRINGS "${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_config.hpp" xvolume_version_defines
     REGEX "#define XVOLUME_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${xvolume_version_defines})
    if(ver MATCHES "#define XVOLUME_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XVOLUME_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(XVOLUME_VERSION
    ${XVOLUME_VERSION_MAJOR}.${XVOLUME_VERSION_MINOR}.${XVOLUME_VERSION_PATCH})
message(STATUS "xvolume version: v${XVOLUME_VERSION}")

# Binary version
# See the following URL for explanations about the binary versionning
# https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html#Updating-version-info
file(STRINGS "${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_config.hpp" xvolume_version_defines
    REGEX "#define XVOLUME_BINARY_(CURRENT|REVISION|AGE)")
foreach(ver ${xvolume_version_defines})
    if(ver MATCHES "#define XVOLUME_BINARY_(CURRENT|REVISION|AGE) +([^ ]+)$")
        set(XVOLUME_BINARY_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(XVOLUME_BINARY_VERSION
    ${XVOLUME_BINARY_CURRENT}.${XVOLUME_BINARY_REVISION}.${XVOLUME_BINARY_AGE})
message(STATUS "xvolume binary version: v${XVOLUME_BINARY_VERSION}")

# Dependencies
# ============

find_package(cppzmq 4.3.0 REQUIRED)
find_package(xtl 0.4 REQUIRED)
find_package(xeus 0.14.1 REQUIRED)
find_package(xwidgets 0.13.1 REQUIRED)

# Source files
# ============

set(XVOLUME_HEADERS
    ${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_figure.hpp
    ${XVOLUME_INCLUDE_DIR}/xvolume/xmesh.hpp
    ${XVOLUME_INCLUDE_DIR}/xvolume/xscatter.hpp
    ${XVOLUME_INCLUDE_DIR}/xvolume/xvolume_utils.hpp
    ${XVOLUME_INCLUDE_DIR}/xvolume/xboxed_container.hpp
)

set(XVOLUME_SOURCES
    ${XVOLUME_SOURCE_DIR}/xvolume.cpp
)

# Output
# ======

add_library(xvolume SHARED ${XVOLUME_SOURCES} ${XVOLUME_HEADERS})
target_include_directories(xvolume PUBLIC $<BUILD_INTERFACE:${XVOLUME_INCLUDE_DIR}>
                                        $<INSTALL_INTERFACE:include>)

target_link_libraries(xvolume
    PUBLIC xtl
    PUBLIC xeus
    PUBLIC xwidgets)

set_target_properties(xvolume PROPERTIES
                      PUBLIC_HEADER "${XVOLUME_HEADERS}"
                      COMPILE_DEFINITIONS "XVOLUME_EXPORTS"
                      PREFIX ""
                      VERSION ${XVOLUME_BINARY_VERSION}
                      SOVERSION ${XVOLUME_BINARY_CURRENT}
                      OUTPUT_NAME "libxvolume")

# Compilation flags
# =================

include(CheckCXXCompilerFlag)
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)
OPTION(DISABLE_ARCH_NATIVE "disable -march=native flag" OFF)

set_target_properties(xvolume PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED 14)

target_compile_features(xvolume PRIVATE cxx_std_14)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(xvolume PUBLIC -Wunused-parameter -Wextra -Wreorder)
    if (DISABLE_ARCH_NATIVE)
        target_compile_options(xvolume PUBLIC -mtune=generic)
    else()
        target_compile_options(xvolume PUBLIC -march=native)
    endif()

    # Enable link time optimization and set the default symbol
    # visibility to hidden (very important to obtain small binaries)
    if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
        # Check for Link Time Optimization support
        # (GCC/Clang)
        CHECK_CXX_COMPILER_FLAG("-flto" HAS_LTO_FLAG)
        if (HAS_LTO_FLAG)
            target_compile_options(xvolume PUBLIC -flto)
        endif()

        # Intel equivalent to LTO is called IPO
        if (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
            CHECK_CXX_COMPILER_FLAG("-ipo" HAS_IPO_FLAG)
            if (HAS_IPO_FLAG)
                target_compile_options(xvolume PUBLIC -ipo)
            endif()
        endif()
    endif()
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

# if(MSVC)
#     target_compile_definitions(xvolume PUBLIC -DNOMINMAX)
#     target_compile_options(xvolume PUBLIC /DGUID_WINDOWS /MP /bigobj)
#     target_compile_options(xvolume PUBLIC /wd4251 /wd 4996)
#     # Allows to compile in debug without requiring each dependencies to
#     # be compiled in debug
#     if(${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
#         set(CMAKE_CXX_FLAGS_DEBUG "/DNDEBUG /MD /Zi /Ob0 /Od /RTC1")
#         message(STATUS "Overriding CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
#     endif()
# endif()

# Tests
# =====

OPTION(BUILD_TESTS "xvolume test suite" OFF)
OPTION(DOWNLOAD_GTEST "build gtest from downloaded sources" OFF)

if(DOWNLOAD_GTEST OR GTEST_SRC_DIR)
    set(BUILD_TESTS ON)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# Installation
# ============

set(XVOLUME_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for xvolumeConfig.cmake")

install(TARGETS xvolume
        EXPORT ${PROJECT_NAME}-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xvolume)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${XVOLUME_CMAKECONFIG_INSTALL_DIR})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${XVOLUME_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
              DESTINATION ${XVOLUME_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
DESTINATION ${XVOLUME_CMAKECONFIG_INSTALL_DIR})
