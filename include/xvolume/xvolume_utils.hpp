/******************************************************************************
* Copyright (c) 2018, Sylvain Corlay and Johan Mabille, Wolf Vollprecht and   *
* Martin Renou                                                                *
*                                                                             *
* Distributed under the terms of the BSD 3-Clause License.                    *
*                                                                             *
* The full license is in the file LICENSE, distributed with this software.    *
*******************************************************************************/

#ifndef XVOLUME_UTILS_HPP
#define XVOLUME_UTILS_HPP

#include "xeus/xjson.hpp"

namespace xvl
{
    /***********
     * helpers *
     ***********/

    template <class T>
    inline void set_patch_from_array(const T& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        patch[property.name()] = {{
            {"shape", std::array<size_t, 1>{ property().size() }},
            {"dtype", "float32"},
            {"buffer", xw::xbuffer_reference_prefix() + std::to_string(buffers.size())}
        }};
        buffers.emplace_back(property().data(), sizeof(float) * property().size());
    }
}

#endif