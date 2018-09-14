/******************************************************************************
* Copyright (c) 2018, Sylvain Corlay and Johan Mabille, Wolf Vollprecht and   *
* Martin Renou                                                                *
*                                                                             *
* Distributed under the terms of the BSD 3-Clause License.                    *
*                                                                             *
* The full license is in the file LICENSE, distributed with this software.    *
*******************************************************************************/

#ifndef XVOLUME_MESH_HPP
#define XVOLUME_MESH_HPP

#include <string>
#include <vector>

#include "xwidgets/xmaterialize.hpp"
#include "xwidgets/xobject.hpp"

#include "xvolume_config.hpp"
#include "xvolume_utils.hpp"

namespace xvl
{
    /********************
     * mesh declaration *
     ********************/

    template <class D>
    class xmesh : public xw::xobject<D>
    {
    public:

        using vector_type = std::vector<float>;
        using vector_type_indices = std::vector<uint32_t>;

        using base_type = xw::xobject<D>;
        using derived_type = D;

        void serialize_state(xeus::xjson&, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);
        const std::vector<xw::xjson_path_type>& buffer_paths() const;

        XPROPERTY(vector_type, derived_type, x);
        XPROPERTY(vector_type, derived_type, y);
        XPROPERTY(vector_type, derived_type, z);
        // jupyter-volume does not like u and v to me empty lists
        // XPROPERTY(vector_type, derived_type, u);
        // XPROPERTY(vector_type, derived_type, v);
        XPROPERTY(vector_type_indices, derived_type, triangles);
        XPROPERTY(vector_type_indices, derived_type, lines);
        // should be a string or array, lets keep it simple first
        XPROPERTY(std::string, derived_type, color, "red");

        XPROPERTY(int, derived_type, sequence_index, 0);
        XPROPERTY(bool, derived_type, visible, true);

    protected:

        xmesh();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using mesh = xw::xmaterialize<xmesh>;

    using mesh_generator = xw::xgenerator<xmesh>;

    /************************
     * xmesh implementation *
     ************************/

    template <class D>
    inline void xmesh<D>::serialize_state(xeus::xjson& state,
                                          xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        using xw::set_patch_from_property;

        set_patch_from_property(x, state, buffers);
        set_patch_from_property(y, state, buffers);
        set_patch_from_property(z, state, buffers);
        // set_patch_from_property(u, state, buffers);
        // set_patch_from_property(v, state, buffers);
        set_patch_from_property(triangles, state, buffers);
        set_patch_from_property(lines, state, buffers);
        set_patch_from_property(color, state, buffers);
        set_patch_from_property(sequence_index, state, buffers);
        set_patch_from_property(visible, state, buffers);
    }

    template <class D>
    inline void xmesh<D>::apply_patch(const xeus::xjson& patch,
                                      const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        using xw::set_property_from_patch;

        set_property_from_patch(x, patch, buffers);
        set_property_from_patch(y, patch, buffers);
        set_property_from_patch(z, patch, buffers);
        // set_property_from_patch(u, patch, buffers);
        // set_property_from_patch(v, patch, buffers);
        set_property_from_patch(triangles, patch, buffers);
        set_property_from_patch(lines, patch, buffers);
        set_property_from_patch(color, patch, buffers);
        set_property_from_patch(sequence_index, patch, buffers);
        set_property_from_patch(visible, patch, buffers);
    }

    template <class D>
    inline xmesh<D>::xmesh()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xmesh<D>::set_defaults()
    {
        this->_model_name() = "MeshModel";
        this->_view_name() = "MeshView";
        this->_model_module() = "ipyvolume";
        this->_view_module() = "ipyvolume";
        // TODO remove hardcoded version string here!
        this->_model_module_version() = XIPYVOLUME_VERSION;
        this->_view_module_version() = XIPYVOLUME_VERSION;

    }

    template <class D>
    inline const std::vector<xw::xjson_path_type>& xmesh<D>::buffer_paths() const
    {
        static const std::vector<xw::xjson_path_type> default_buffer_paths = {
            {"x", "0", "data"},
            {"y", "0", "data"},
            {"z", "0", "data"},
            {"triangles", "0", "data"},
            {"lines", "0", "data"}
        };
        return default_buffer_paths;
    }

    inline void set_patch_from_property(const decltype(mesh::x)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }

    inline void set_patch_from_property(const decltype(mesh::y)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }

    inline void set_patch_from_property(const decltype(mesh::z)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }
    inline void set_patch_from_property(const decltype(mesh::triangles)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }
    inline void set_patch_from_property(const decltype(mesh::lines)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }

    template <class F>
    void fill_mesh(mesh& m, F f, std::array<unsigned int, 2> shape, std::array<float,2> xrange, std::array<float,2> yrange,
        bool lines=false, bool wrapx=false, bool wrapy=false) {
        m.triangles = generate_triangles(shape, wrapx, wrapy);
        m.lines = generate_lines(shape, wrapx, wrapy);
        unsigned int nx = shape[0];
        unsigned int ny = shape[1];
        unsigned int n = nx * ny;
        std::vector<float> x((size_t)n), y((size_t)n), z((size_t)n);
        for(int i = 0; i < nx; i++) {
            for(int j = 0; j < ny; j++) {
                float x_value = xrange[0] + (xrange[1] - xrange[0]) * i / (nx-1);
                float y_value = yrange[0] + (yrange[1] - yrange[0]) * j / (ny-1);
                float z_value = f(x_value, y_value);
                x[i * ny + j] = x_value;
                y[i * ny + j] = y_value;
                z[i * ny + j] = z_value;
            }
        }
        m.x = x;
        m.y = y;
        m.z = z;
    }

}



/*********************
 * precompiled types *
 *********************/

// #ifndef _WIN32
//     extern template class xw::xmaterialize<xvl::xmesh>;
//     extern template xw::xmaterialize<xvl::xmesh>::xmaterialize();
//     extern template class xw::xtransport<xw::xmaterialize<xvl::xmesh>>;
//     extern template class xw::xgenerator<xvl::xmesh>;
//     extern template xw::xgenerator<xvl::xmesh>::xgenerator();
//     extern template class xw::xtransport<xw::xgenerator<xvl::xmesh>>;
// #endif

#endif
