/***************************************************************************
* Copyright (c) 2017, Sylvain Corlay and Johan Mabille                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XVOLUME_PLOT_HPP
#define XVOLUME_PLOT_HPP

#include <fstream>
#include <list>
#include <map>
#include <streambuf>
#include <string>
#include <vector>

#include "xtl/xoptional.hpp"

// xthreejs stuff
#ifdef XTHREE_CAMERA_HPP
#include "xthreejs/cameras/xperspective_camera_autogen.hpp"
#endif
#ifdef XTHREE_SCENE_HPP
#include "xthreejs/scenes/xscene_autogen.hpp"
#endif

#include "xwidgets/xeither.hpp"
#include "xwidgets/xwidget.hpp"

#include "xvolume_config.hpp"

#include "xscatter.hpp"
#include "xmesh.hpp"

#include "xboxed_container.hpp"

namespace xvl
{
    template <class D>
    class xvolume_figure : public xw::xwidget<D>
    {
    public:

        using base_type = xw::xwidget<D>;
        using derived_type = D;

        using camera_position_type = std::array<double, 3>;
        using limits_type = std::array<double, 2>;
        using projection_type = xtl::xoptional<std::array<double, 16>>;

        void serialize_state(xeus::xjson&, xeus::buffer_sequence&) const;
        void apply_patch(const xeus::xjson&, const xeus::buffer_sequence&);

        using scatter_container = std::vector<xvl::scatter>;
        using mesh_container = std::vector<xvl::mesh>;
        // TODO implement xvl::volume!
        using volume_container = std::vector<int>;

        XPROPERTY(double, derived_type, eye_separation, 6.4);

        // scatters = traitlets.List(traitlets.Instance(Scatter), [], allow_none=False).tag(sync=True, **ipywidgets.widget_serialization)
        // meshes = traitlets.List(traitlets.Instance(Mesh), [], allow_none=False).tag(sync=True, **ipywidgets.widget_serialization)
        // volumes = traitlets.List(traitlets.Instance(Volume), [], allow_none=False).tag(sync=True, **ipywidgets.widget_serialization)
        XPROPERTY(scatter_container, derived_type, scatters);
        XPROPERTY(mesh_container, derived_type, meshes);
        XPROPERTY(volume_container, derived_type, volumes);

        XPROPERTY(double, derived_type, animation, 1000.f);
        XPROPERTY(double, derived_type, animation_exponent, 1.f);
    
        XPROPERTY(double, derived_type, ambient_coefficient, 0.5f);
        XPROPERTY(double, derived_type, diffuse_coefficient, 0.8f);
        XPROPERTY(double, derived_type, specular_coefficient, 0.5f);
        XPROPERTY(double, derived_type, specular_exponent, 5.f);

        XPROPERTY(bool, derived_type, stereo, false);

        XPROPERTY(std::string, derived_type, camera_control, "trackball");
        // TODO implement validator for MAX/MIN
        XPROPERTY(double, derived_type, camera_fov, 45.f);

        XPROPERTY(camera_position_type, derived_type, camera_center);

        #ifdef XTHREE_CAMERA_HPP
        XPROPERTY(xthree::perspective_camera, derived_type, camera);
        #endif

        #ifdef XTHREE_SCENE_HPP
        XPROPERTY(xthree::scene, derived_type, scene);
        #endif

        XPROPERTY(int, derived_type, width, 500);
        XPROPERTY(int, derived_type, height, 400);
        XPROPERTY(int, derived_type, downscale, 1);
        XPROPERTY(int, derived_type, displayscale, 1);
        XPROPERTY(xtl::xoptional<float>, derived_type, capture_fps, xtl::missing<float>());
        XPROPERTY(int, derived_type, cube_resolution, 512);
    
        XPROPERTY(std::string, derived_type, show, "Volume");       

        // TODO set default value + set max length / min length == 2!
        XPROPERTY(limits_type, derived_type, xlim);
        XPROPERTY(limits_type, derived_type, ylim);
        XPROPERTY(limits_type, derived_type, zlim);

        XPROPERTY(projection_type, derived_type, matrix_projection);
        XPROPERTY(projection_type, derived_type, matrix_world);

        XPROPERTY(std::string, derived_type, x, "x");
        XPROPERTY(std::string, derived_type, y, "y");
        XPROPERTY(std::string, derived_type, z, "z");

        XPROPERTY(::xeus::xjson, derived_type, style);

        XPROPERTY(bool, derived_type, render_continuous, false);
        XPROPERTY(std::string, derived_type, selector, "lasso");
        XPROPERTY(std::string, derived_type, selection_mode, "replace");
        XPROPERTY(std::string, derived_type, mouse_mode, "normal");
        XPROPERTY(std::string, derived_type, panorama_mode, "no", XEITHER("no", "360", "180"));

    protected:

        xvolume_figure();
        using base_type::base_type;

    private:

        void set_defaults();

    };

    template <class D>
    inline xvolume_figure<D>::xvolume_figure()
    {
        this->set_defaults();
    }

    template <class D>
    inline void xvolume_figure<D>::set_defaults()
    {
        this->_view_name() = "FigureView";
        this->_view_module() = "ipyvolume";
        this->_model_module() = "ipyvolume";
        this->_model_name() = "FigureModel";

        // TODO remove hardcoded version string here!
        this->_model_module_version() = XIPYVOLUME_VERSION;
        this->_view_module_version() = XIPYVOLUME_VERSION;


        this->style() = {
            {"background-color", "white"},
            {"axes", 
                {
                    {"color", "black"},
                    {"visible", true},
                    {"label", {{"color", "black"}}},
                    {"ticklabel", {{"color", "black"}}}
                }
            },
            {"box", 
                {{"visible", true}}
            }
        };

        this->camera_center() = std::array<double, 3>{0, 0, 0};
        this->matrix_projection() = std::array<double, 16>{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        this->matrix_world() = std::array<double, 16>{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        this->xlim() = std::array<double, 2>{0, 1};
        this->ylim() = std::array<double, 2>{0, 1};
        this->zlim() = std::array<double, 2>{0, 1};
        #ifdef XTHREE_CAMERA_HPP
        this->camera() = xthree::perspective_camera_generator()
            .fov(46)
            .position({0, 0, 2})
            .finalize();
        #endif
        #ifdef XTHREE_SCENE_HPP
        this->scene() = xthree::scene_generator()
            .background(xtl::missing<xw::html_color>())
            .finalize();
        #endif
    }

    template <class D>
    inline void xvolume_figure<D>::apply_patch(const xeus::xjson& patch, const xeus::buffer_sequence& buffers)
    {
        using xw::set_property_from_patch;
        base_type::apply_patch(patch, buffers);
        set_property_from_patch(eye_separation, patch, buffers);
        set_property_from_patch(scatters, patch, buffers);
        set_property_from_patch(meshes, patch, buffers);
        set_property_from_patch(volumes, patch, buffers);
        set_property_from_patch(animation, patch, buffers);
        set_property_from_patch(animation_exponent, patch, buffers);
        set_property_from_patch(ambient_coefficient, patch, buffers);
        set_property_from_patch(diffuse_coefficient, patch, buffers);
        set_property_from_patch(specular_coefficient, patch, buffers);
        set_property_from_patch(specular_exponent, patch, buffers);
        set_property_from_patch(stereo, patch, buffers);
        set_property_from_patch(camera_control, patch, buffers);
        set_property_from_patch(camera_fov, patch, buffers);
        set_property_from_patch(camera_center, patch, buffers);
        set_property_from_patch(width, patch, buffers);
        set_property_from_patch(height, patch, buffers);
        set_property_from_patch(downscale, patch, buffers);
        set_property_from_patch(displayscale, patch, buffers);
        set_property_from_patch(cube_resolution, patch, buffers);
        set_property_from_patch(show, patch, buffers);
        set_property_from_patch(xlim, patch, buffers);
        set_property_from_patch(ylim, patch, buffers);
        set_property_from_patch(zlim, patch, buffers);
        set_property_from_patch(matrix_projection, patch, buffers);
        set_property_from_patch(matrix_world, patch, buffers);
        set_property_from_patch(x, patch, buffers);
        set_property_from_patch(y, patch, buffers);
        set_property_from_patch(z, patch, buffers);
        set_property_from_patch(style, patch, buffers);
        set_property_from_patch(render_continuous, patch, buffers);
        set_property_from_patch(selector, patch, buffers);
        set_property_from_patch(selection_mode, patch, buffers);
        set_property_from_patch(mouse_mode, patch, buffers);
        set_property_from_patch(panorama_mode, patch, buffers);
    }


    template <class D>
    inline void xvolume_figure<D>::serialize_state(xeus::xjson& state, xeus::buffer_sequence& buffers) const
    {
        using xw::set_patch_from_property;
        base_type::serialize_state(state, buffers);
        set_patch_from_property(eye_separation, state, buffers);
        set_patch_from_property(scatters, state, buffers);
        set_patch_from_property(meshes, state, buffers);
        set_patch_from_property(volumes, state, buffers);
        set_patch_from_property(animation, state, buffers);
        set_patch_from_property(animation_exponent, state, buffers);
        set_patch_from_property(ambient_coefficient, state, buffers);
        set_patch_from_property(diffuse_coefficient, state, buffers);
        set_patch_from_property(specular_coefficient, state, buffers);
        set_patch_from_property(specular_exponent, state, buffers);
        set_patch_from_property(stereo, state, buffers);
        set_patch_from_property(camera_control, state, buffers);
        set_patch_from_property(camera_fov, state, buffers);
        set_patch_from_property(camera_center, state, buffers);
        set_patch_from_property(width, state, buffers);
        set_patch_from_property(height, state, buffers);
        set_patch_from_property(downscale, state, buffers);
        set_patch_from_property(displayscale, state, buffers);
        set_patch_from_property(cube_resolution, state, buffers);
        set_patch_from_property(show, state, buffers);
        set_patch_from_property(xlim, state, buffers);
        set_patch_from_property(ylim, state, buffers);
        set_patch_from_property(zlim, state, buffers);
        set_patch_from_property(matrix_projection, state, buffers);
        set_patch_from_property(matrix_world, state, buffers);
        set_patch_from_property(x, state, buffers);
        set_patch_from_property(y, state, buffers);
        set_patch_from_property(z, state, buffers);
        set_patch_from_property(style, state, buffers);
        set_patch_from_property(render_continuous, state, buffers);
        set_patch_from_property(selector, state, buffers);
        set_patch_from_property(selection_mode, state, buffers);
        set_patch_from_property(mouse_mode, state, buffers);
        set_patch_from_property(panorama_mode, state, buffers);
    }

    using volume_figure = xw::xmaterialize<xvolume_figure>;

}

#endif