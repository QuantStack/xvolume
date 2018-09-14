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
            {"data", xw::xbuffer_reference_prefix() + std::to_string(buffers.size())}
        }};
        buffers.emplace_back(property().data(), sizeof(float) * property().size());
    }
    template <class T>
    inline void set_patch_from_array_uint32(const T& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        patch[property.name()] = {{
            {"shape", std::array<size_t, 1>{ property().size() }},
            {"dtype", "uint32"},
            {"data", xw::xbuffer_reference_prefix() + std::to_string(buffers.size())}
        }};
        buffers.emplace_back(property().data(), sizeof(int) * property().size());
    }

    unsigned int index_from_2d(unsigned int nx, unsigned int ny, unsigned int x_index, unsigned int y_index)
    {
        x_index = x_index % nx;
        y_index = y_index % ny;
        return ny * x_index + y_index;
    }

    std::vector<unsigned int> generate_triangles(std::array<unsigned int, 2> shape, bool wrapx=false, bool wrapy=false)
    {
        unsigned int nx = shape[0];
        unsigned int ny = shape[1];
        unsigned int mx = wrapx ? nx : nx - 1;
        unsigned int my = wrapy ? ny : ny - 1;
        /* 3 indices per triangle, 2 triangles per rectangle
        The order of the vertices p0..3
        ^ ydir
        |
        2 3
        0 1  ---> x dir
        where [0,1,3] and [0,3,2] form the two triangles
        */
        std::vector<unsigned int> triangles(mx * my * 3 * 2);
        for(unsigned int i = 0; i < mx; i++) {
            for(unsigned int j = 0; j < mx; j++) {
                unsigned int p0 = index_from_2d(nx, ny, i + 0, j + 0);
                unsigned int p1 = index_from_2d(nx, ny, i + 1, j + 0);
                unsigned int p2 = index_from_2d(nx, ny, i + 0, j + 1);
                unsigned int p3 = index_from_2d(nx, ny, i + 1, j + 1);
                unsigned int triangle_index = (i * my + j)*3;

                // 2 triangles
                triangles[triangle_index * 2 + 0] = p0;
                triangles[triangle_index * 2 + 1] = p1;
                triangles[triangle_index * 2 + 2] = p3;

                triangles[triangle_index * 2 + 3] = p0;
                triangles[triangle_index * 2 + 4] = p3;
                triangles[triangle_index * 2 + 5] = p2;

                // lines[triangle_index * 4 + 0] = p0;
                // lines[triangle_index * 4 + 0] = p1;

            }
        }
        return triangles;
    }

    std::vector<unsigned int> generate_lines(std::array<unsigned int, 2> shape, bool wrapx=false, bool wrapy=false)
    {
        unsigned int nx = shape[0];
        unsigned int ny = shape[1];
        unsigned int mx = wrapx ? nx : nx - 1;
        unsigned int my = wrapy ? ny : ny - 1;
        /* 2 indices per triangle, 2 lines per rectangle, plus some on the edges
        The order of the vertices p0..3
        ^ ydir
        |
        2 3
        0 1  ---> x dir
        where [0,1] and [0,2] form the two lines, [1,3] and [2,3] on the edges
        */
        std::vector<unsigned int> lines(mx * my * 2 * 2 + mx * 2 + my * 2);
        for(unsigned int i = 0; i < mx; i++) {
            for(unsigned int j = 0; j < mx; j++) {
                unsigned int p0 = index_from_2d(nx, ny, i + 0, j + 0);
                unsigned int p1 = index_from_2d(nx, ny, i + 1, j + 0);
                unsigned int p2 = index_from_2d(nx, ny, i + 0, j + 1);
                unsigned int p3 = index_from_2d(nx, ny, i + 1, j + 1);
                unsigned int line_index = (i * my + j)*2;

                // 2 triangles
                lines[line_index * 2 + 0] = p0;
                lines[line_index * 2 + 1] = p1;

                lines[line_index     * 2 + 2] = p0;
                lines[line_index     * 2 + 3] = p2;

                if(i == (mx - 1)) { // right edges
                    lines[mx * my * 2 * 2 + j*2 + 0] = p1;
                    lines[mx * my * 2 * 2 + j*2 + 1] = p3;
                }
                if(j == (my - 1)) { // top edge
                    lines[mx * my * 2 * 2 + mx * 2 + i*2 + 0] = p2;
                    lines[mx * my * 2 * 2 + mx * 2 + i*2 + 1] = p3;
                }

            }
        }
        return lines;
    }

}

#endif