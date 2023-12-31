#pragma once

#include "../glcore/gl_texture.h"
#include <vector.hpp>

namespace sputnik::graphics::api
{

using namespace ramanujan;
using namespace ramanujan::experimental;
using namespace sputnik::graphics::gl;

struct Material
{
    std::string                   shader_name{"blinn_phong"};
    vec3                          ambient{1.0f, 1.0f, 1.0f};
    vec3                          diffuse{1.0f, 1.0f, 1.0f};
    vec3                          specular{1.0f, 1.0f, 1.0f};
    float                         shininess{32.0f};
    float                         alpha{1.0f};

    // Todo:: Get rid of the ogl specific stuff
    std::shared_ptr<OglTexture2D> diff_texture{nullptr};
    std::shared_ptr<OglTexture2D> spec_texture{nullptr};
};

static inline Material material_white{.ambient   = {1.0f, 1.0f, 1.0f},
                                      .diffuse   = {1.0f, 1.0f, 1.0f},
                                      .specular  = {1.0f, 1.0f, 1.0f},
                                      .shininess = 1.0f};

static inline Material material_silver{.ambient   = {0.19225f, 0.19225f, 0.19225f},
                                       .diffuse   = {0.50754f, 0.50754f, 0.50754f},
                                       .specular  = {0.508273f, 0.508273f, 0.508273f},
                                       .shininess = 0.4f};

static inline Material material_gold{.ambient   = {0.24725f, 0.1995f, 0.0745f},
                                     .diffuse   = {0.75164f, 0.60648f, 0.22648f},
                                     .specular  = {0.628281f, 0.555802f, 0.366065f},
                                     .shininess = 0.4f};

static inline Material material_pearl{.ambient   = {0.25f, 0.20725f, 0.20725f},
                                      .diffuse   = {1.0f, 0.829f, 0.829f},
                                      .specular  = {0.296648f, 0.296648f, 0.296648f},
                                      .shininess = 0.088f};

static inline Material material_chrome{.ambient   = {0.25f, 0.25f, 0.25f},
                                       .diffuse   = {0.4f, 0.4f, 0.4f},
                                       .specular  = {0.774597f, 0.774597f, 0.774597f},
                                       .shininess = 0.088f};

static inline Material material_ruby{.ambient   = {0.1745f, 0.01175f, 0.01175f},
                                     .diffuse   = {0.61424f, 0.04136f, 0.04136f},
                                     .specular  = {0.727811f, 0.626959f, 0.626959f},
                                     .shininess = 0.6f};

static inline Material material_emerald{.ambient   = {0.0215f, 0.1745f, 0.0215f},
                                        .diffuse   = {0.07568f, 0.61424f, 0.07568f},
                                        .specular  = {0.633f, 0.727811f, 0.633f},
                                        .shininess = 0.6f};

static inline Material material_rubber_red{.ambient   = {0.05f, 0.0f, 0.0f},
                                           .diffuse   = {0.5f, 0.4f, 0.4f},
                                           .specular  = {0.7f, 0.04f, 0.04f},
                                           .shininess = 0.078125f};

} // namespace sputnik::graphics::api