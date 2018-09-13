/******************************************************************************
* Copyright (c) 2018, Sylvain Corlay and Johan Mabille, Wolf Vollprecht and   *
* Martin Renou                                                                *
*                                                                             *
* Distributed under the terms of the BSD 3-Clause License.                    *
*                                                                             *
* The full license is in the file LICENSE, distributed with this software.    *
*******************************************************************************/

#ifndef XVOLUME_SCATTER_HPP
#define XVOLUME_SCATTER_HPP

#include <string>
#include <vector>

#include "xtl/xoptional.hpp"

#include "xthreejs/materials/xshader_material_autogen.hpp"

#include "xwidgets/xmaterialize.hpp"
#include "xwidgets/xobject.hpp"

namespace xvl
{
    /***********************
     * scatter declaration *
     ***********************/

    template <class D>
    class xscatter : public xw::xobject<D>
    {
    public:

        using vector_type = std::vector<float>;

        using base_type = xw::xobject<D>;
        using derived_type = D;

        void serialize_state(xeus::xjson&, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);
        const std::vector<xw::xjson_path_type>& buffer_paths() const;

        XPROPERTY(vector_type, derived_type, x, {0});
        XPROPERTY(vector_type, derived_type, y, {0});
        XPROPERTY(vector_type, derived_type, z, {0});
        XPROPERTY(xtl::xoptional<vector_type>, derived_type, vx, xtl::missing<vector_type>());
        XPROPERTY(xtl::xoptional<vector_type>, derived_type, vy, xtl::missing<vector_type>());
        XPROPERTY(xtl::xoptional<vector_type>, derived_type, vz, xtl::missing<vector_type>());
        XPROPERTY(xtl::xoptional<vector_type>, derived_type, selected, xtl::missing<vector_type>());
        XPROPERTY(int, derived_type, sequence_index, 0);
        XPROPERTY(float, derived_type, size, 5);
        XPROPERTY(float, derived_type, size_selected, 7);
        XPROPERTY(std::string, derived_type, color);
        XPROPERTY(std::string, derived_type, color_selected);
        XPROPERTY(std::string, derived_type, geo, "diamond");
        XPROPERTY(bool, derived_type, connected, true);
        XPROPERTY(bool, derived_type, visible, true);
        // Note: this is a Union in ipywidgets
        XPROPERTY(std::string, derived_type, texture);

        XPROPERTY(xthree::shader_material, derived_type, material);
        XPROPERTY(xthree::shader_material, derived_type, line_material);

    protected:

        xscatter();
        using base_type::base_type;

    private:

        void set_defaults();
    };

    using scatter = xw::xmaterialize<xscatter>;

    using scatter_generator = xw::xgenerator<xscatter>;

    /***************************
     * xscatter implementation *
     ***************************/

    template <class D>
    inline void xscatter<D>::serialize_state(xeus::xjson& state,
                                            xeus::buffer_sequence& buffers) const
    {
        base_type::serialize_state(state, buffers);

        using xw::set_patch_from_property;

        set_patch_from_property(x, state, buffers);
        set_patch_from_property(y, state, buffers);
        set_patch_from_property(z, state, buffers);
        set_patch_from_property(vx, state, buffers);
        set_patch_from_property(vz, state, buffers);
        set_patch_from_property(selected, state, buffers);
        set_patch_from_property(sequence_index, state, buffers);
        set_patch_from_property(size, state, buffers);
        set_patch_from_property(size_selected, state, buffers);
        set_patch_from_property(color, state, buffers);
        set_patch_from_property(color_selected, state, buffers);
        set_patch_from_property(geo, state, buffers);
        set_patch_from_property(connected, state, buffers);
        set_patch_from_property(visible, state, buffers);
    }

    template <class D>
    inline void xscatter<D>::apply_patch(const xeus::xjson& patch,
                                        const xeus::buffer_sequence& buffers)
    {
        base_type::apply_patch(patch, buffers);

        using xw::set_property_from_patch;

        set_property_from_patch(x, patch, buffers);
        set_property_from_patch(y, patch, buffers);
        set_property_from_patch(z, patch, buffers);
        set_property_from_patch(vx, patch, buffers);
        set_property_from_patch(vz, patch, buffers);
        set_property_from_patch(selected, patch, buffers);
        set_property_from_patch(sequence_index, patch, buffers);
        set_property_from_patch(size, patch, buffers);
        set_property_from_patch(size_selected, patch, buffers);
        set_property_from_patch(color, patch, buffers);
        set_property_from_patch(color_selected, patch, buffers);
        set_property_from_patch(geo, patch, buffers);
        set_property_from_patch(connected, patch, buffers);
        set_property_from_patch(visible, patch, buffers);
    }

    template <class D>
    inline xscatter<D>::xscatter()
        : base_type()
    {
        set_defaults();
    }

    template <class D>
    inline void xscatter<D>::set_defaults()
    {
        this->_model_name() = "ScatterModel";
        this->_view_name() = "ScatterView";
        this->_model_module() = "ipyvolume";
        this->_view_module() = "ipyvolume";
        // TODO remove hardcoded version string here!
        this->_model_module_version() = "~0.4.5";
        this->_view_module_version() = "~0.4.5";

        this->material() = xthree::shader_material();
        this->line_material() = xthree::shader_material();
    }

    template <class D>
    inline const std::vector<xw::xjson_path_type>& xscatter<D>::buffer_paths() const
    {
        static const std::vector<xw::xjson_path_type> default_buffer_paths = {
            {"x", "0", "buffer"}, 
            {"y", "0", "buffer"}, 
            {"z", "0", "buffer"}
        };
        return default_buffer_paths;
    }

    template <class T>
    inline void set_patch_from_array(const T& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        patch[property.name()] = {{
            {"shape", std::array<size_t, 1>{ property().size() }},
            {"dtype", "float32"},
            {"buffer", xw::xbuffer_reference_prefix() + std::to_string(buffers.size())}
        }};
        buffers.emplace_back(property().data(), sizeof(double) * property().size());

    }

    inline void set_patch_from_property(const decltype(scatter::x)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }
    inline void set_patch_from_property(const decltype(scatter::y)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }
    inline void set_patch_from_property(const decltype(scatter::z)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    {
        set_patch_from_array(property, patch, buffers);
    }
    // inline void set_patch_from_property(const decltype(scatter::vx)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    // {
    //     set_patch_from_array(property, patch, buffers);
    // }
    // inline void set_patch_from_property(const decltype(scatter::vz)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    // {
    //     set_patch_from_array(property, patch, buffers);
    // }
    // inline void set_patch_from_property(const decltype(scatter::selected)& property, xeus::xjson& patch, xeus::buffer_sequence& buffers)
    // {
    //     set_patch_from_array(property, patch, buffers);
    // }

    inline void set_property_from_patch(decltype(scatter::x)& property,
                                        const xeus::xjson& patch,
                                        const xeus::buffer_sequence& buffers)
    {
        auto it = patch.find(property.name());
        if (it != patch.end())
        {
            using value_type = typename decltype(scatter::x)::value_type;
            std::size_t index = xw::buffer_index(patch[property.name()].template get<std::string>());
            const auto& value_buffer = buffers[index];
            const double* value_buf = value_buffer.data<const double>();
            property = value_type(value_buf, value_buf + value_buffer.size());
        }
    }

}

/*********************
 * precompiled types *
 *********************/

// #ifndef _WIN32
//     extern template class xw::xmaterialize<xvl::xscatter>;
//     extern template xw::xmaterialize<xvl::xscatter>::xmaterialize();
//     extern template class xw::xtransport<xw::xmaterialize<xvl::xscatter>>;
//     extern template class xw::xgenerator<xvl::xscatter>;
//     extern template xw::xgenerator<xvl::xscatter>::xgenerator();
//     extern template class xw::xtransport<xw::xgenerator<xvl::xscatter>>;
// #endif

#endif
