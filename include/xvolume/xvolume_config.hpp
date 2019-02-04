/******************************************************************************
* Copyright (c) 2018, Sylvain Corlay and Johan Mabille, and Wolf Vollprecht   *
*                                                                             *
* Distributed under the terms of the BSD 3-Clause License.                    *
*                                                                             *
* The full license is in the file LICENSE, distributed with this software.    *
*******************************************************************************/

#ifndef XVOLUME_CONFIG_HPP
#define XVOLUME_CONFIG_HPP

#include <string>

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
#define XVOLUME_BINARY_CURRENT 0
#define XVOLUME_BINARY_REVISION 0
#define XVOLUME_BINARY_AGE 0

// Semver requirement for jupyter-leaflet
#define XJUPYTER_VOLUME_VERSION_MAJOR 0
#define XJUPYTER_VOLUME_VERSION_MINOR 5
#define XJUPYTER_VOLUME_VERSION_PATCH 2

// Composing the version strings from major, minor and patch
#define XVOLUME_CONCATENATE(A, B) XVOLUME_CONCATENATE_IMPL(A, B)
#define XVOLUME_CONCATENATE_IMPL(A, B) A##B
#define XVOLUME_STRINGIFY(a) XVOLUME_STRINGIFY_IMPL(a)
#define XVOLUME_STRINGIFY_IMPL(a) #a

#define XJUPYTER_VOLUME_VERSION XVOLUME_STRINGIFY(XVOLUME_CONCATENATE(XJUPYTER_VOLUME_VERSION_MAJOR,   \
                            XVOLUME_CONCATENATE(.,XVOLUME_CONCATENATE(XJUPYTER_VOLUME_VERSION_MINOR,   \
                                                XVOLUME_CONCATENATE(.,XJUPYTER_VOLUME_VERSION_PATCH)))))

inline std::string jupyter_volume_semver()
{
    return std::string("^") + XJUPYTER_VOLUME_VERSION;
}

#ifdef __CLING__
#include "xvolume_config_cling.hpp"
#endif

#endif
