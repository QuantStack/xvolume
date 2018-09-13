/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XVOLUME_CONFIG_HPP
#define XVOLUME_CONFIG_HPP

// Visual C++ declspec macros
#ifdef _WIN32
    #ifdef XVOLUME_EXPORTS
        #define XVOLUME_API __declspec(dllexport)
    #else
        #define XVOLUME_API __declspec(dllimport)
    #endif
#else
    #define XVOLUME_API
#endif

// Project version
#define XVOLUME_VERSION_MAJOR 0
#define XVOLUME_VERSION_MINOR 0
#define XVOLUME_VERSION_PATCH 0

// Binary version
#define XVOLUME_BINARY_CURRENT 1
#define XVOLUME_BINARY_REVISION 0
#define XVOLUME_BINARY_AGE 1

// Semver requirement for ipyvolume
#define XIPYVOLUME_VERSION_MAJOR 0
#define XIPYVOLUME_VERSION_MINOR 4
#define XIPYVOLUME_VERSION_PATCH 5

// Composing the version strings from major, minor and patch
#define XVOLUME_CONCATENATE(A, B) XVOLUME_CONCATENATE_IMPL(A, B)
#define XVOLUME_CONCATENATE_IMPL(A, B) A##B
#define XVOLUME_STRINGIFY(a) XVOLUME_STRINGIFY_IMPL(a)
#define XVOLUME_STRINGIFY_IMPL(a) #a

#define XIPYVOLUME_VERSION ~XVOLUME_STRINGIFY(XVOLUME_CONCATENATE(XBQPLOT_VERSION_MAJOR,   \
                    XVOLUME_CONCATENATE(.,XVOLUME_CONCATENATE(XBQPLOT_VERSION_MINOR,      \
                                      XVOLUME_CONCATENATE(.,XBQPLOT_VERSION_PATCH)))))

#ifdef __CLING__
#include "xplot_config_cling.hpp"
#endif

#endif
