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
#define XVOLUME_BINARY_CURRENT 	0
#define XVOLUME_BINARY_REVISION 0
#define XVOLUME_BINARY_AGE 		0

#define XIPYVOLUME_VERSION "~0.5.2-dev"

#ifdef __CLING__
	#include "xvolume_config_cling.hpp"
#endif

#endif
