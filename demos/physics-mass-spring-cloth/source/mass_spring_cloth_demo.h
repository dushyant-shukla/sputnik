#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

// #include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
#include <graphics/api/light.h>
// #include <graphics/glcore/texture.h>
// #include <graphics/glcore/debug_draw.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <physics/particle_world.h>
#include <physics/particle_constraints.h>

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

using namespace sputnik::physics;
// using namespace sputnik::graphics::glcore;
using namespace sputnik::graphics::api;

class MassSpringClothDemoLayer : public core::Layer
{

public:
    MassSpringClothDemoLayer(const std::string& name);
    virtual ~MassSpringClothDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

    void UpdateAdditionalMass();

private:
    std::shared_ptr<Model> m_sphere;
};

unsigned const kParticleCount{12};

class MassSpringClothDemo : public sputnik::main::Application
{

public:
    MassSpringClothDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<MassSpringClothDemoLayer>(name));
    }

    ~MassSpringClothDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::MassSpringClothDemo("Mass Spring Cloth Demo");
}
