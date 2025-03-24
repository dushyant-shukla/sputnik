#include "physics_rb_sandbox.hpp"
#include "phx/phx_math_utils.hpp"

#include <editor/editor.hpp>

namespace sputnik::demos
{

using namespace sputnik::editor;

// clang-format off
const float kCubeVertices[] = {
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,			0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,			0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,		-1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,         1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,			0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,			0.0f, 1.0f
		};
// clang-format on

PhysicsRigidBodySandboxDemoLayer::PhysicsRigidBodySandboxDemoLayer(const std::string& name)
    : core::Layer{name}
    , m_gravity_fgen(PhxVec3(0.0f, -9.8f, 0.0f))
{
    m_vertex_buffer = std::make_shared<OglBuffer>((void*)kCubeVertices, sizeof(kCubeVertices));

    std::initializer_list<VertexInputAttributeSpecification> vertex_input_attributes{
        {.name = "position", .location = 0, .type = VertexAttributeType::Float3, .normalized = false},
        {.name = "normal", .location = 1, .type = VertexAttributeType::Float3, .normalized = false},
        {.name = "uv", .location = 2, .type = VertexAttributeType::Float2, .normalized = false},
    };

    VertexInputBindingSpecification vertex_input_binding{0, 8 * sizeof(float)};

    m_vertex_array = std::make_shared<OglVertexArray>();
    m_vertex_array->addVertexBuffer(*m_vertex_buffer.get(), vertex_input_binding, vertex_input_attributes);

    auto  render_system = RenderSystem::getInstance();
    auto& light         = render_system->getLight();
    light.position      = vec3(0.0f, 5.0f, 5.0f);
    light.ambient       = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse       = vec3(1.0f, 1.0f, 1.0f);
    light.specular      = vec3(1.0f, 1.0f, 1.0f);

    m_box_rb.setMass(2.5f);
    m_box_rb.setDamping(0.8f, 0.08f);
    m_box_rb.setPosition(PhxVec3(0.0f, 10.0f, 0.0f));
    m_box_rb.setVelocity(PhxVec3(0.0f, 0.0f, 0.0f));
    m_box_rb.setRotation(PhxVec3(0.0f, 0.0f, 0.0f));
    m_box_rb.setOrientation(1.0f, 0.0f, 0.0f, 0.0f);
    // m_box_rb.setAcceleration(PhxVec3(0.0f, -9.81f, 0.0f));
    m_box_rb.setAwake();
    m_box_rb.setCanSleep(false);
    m_box_rb.setInertiaTensorWithHalfSizesAndMass(PhxVec3(2.0f, 1.0f, 1.0f), 2.5f);
    m_box_rb.calculateDerivedData();

    m_phx_world.addRigidBody(&m_box_rb);
}

PhysicsRigidBodySandboxDemoLayer::~PhysicsRigidBodySandboxDemoLayer() {}

void PhysicsRigidBodySandboxDemoLayer::OnAttach() {}

void PhysicsRigidBodySandboxDemoLayer::OnDetach() {}

void PhysicsRigidBodySandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    RenderSystem* render_system = RenderSystem::getInstance();

    if(m_simulate_physics)
    {
        // const auto& force = 7.0f * phx::phxGenerateRandomUnitVector();
        // m_box_rb.addForceAtBodyPoint(force, {1.0f, 3.5f, 1.0f});

        double frame_time = time_step.GetSeconds();
        m_accumulated_time += frame_time;
        while(m_accumulated_time >= kStepSize)
        {
            simulatePhysics(m_total_time, kStepSize);
            m_accumulated_time -= kStepSize;
            m_total_time += kStepSize;
        }
    }

    m_vertex_array->bind();

    {
        const auto& rb_world_trfs = m_box_rb.getWorldTransform();
        mat4        model         = {};
        model.right    = {rb_world_trfs[0][0], rb_world_trfs[0][1], rb_world_trfs[0][2], rb_world_trfs[0][3]};
        model.up       = {rb_world_trfs[1][0], rb_world_trfs[1][1], rb_world_trfs[1][2], rb_world_trfs[1][3]};
        model.forward  = {rb_world_trfs[2][0], rb_world_trfs[2][1], rb_world_trfs[2][2], rb_world_trfs[2][3]};
        model.position = {rb_world_trfs[3][0], rb_world_trfs[3][1], rb_world_trfs[3][2], rb_world_trfs[3][3]};

        mat4 model1 = {};
        model1      = model1.translate({0.0f, 3.0f, 0.0f});
        // model1      = model1.scale({0.25f});

        // render_system->drawTriangles(36, material_ruby, model1);
        render_system->drawTriangles(36, material_ruby, model);
    }

    m_vertex_array->unbind();
}

void PhysicsRigidBodySandboxDemoLayer::OnEvent() {}

void PhysicsRigidBodySandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Physics Sandbox"))
    {
        Editor::drawWidgetCheckbox("Simulate Physics", m_simulate_physics, 90.0f, "#simulate_physics");
        // Editor::drawWidgetCheckbox("Reset Simulation", m_reset_simulation, 90.0f, "#reset_simulation");
        // Editor::drawWidgetCheckbox("Render BVH", m_draw_bvh, 90.0f, "#bvh");
        // Editor::drawWidgetCheckbox("Render Mesh", m_draw_mesh, 90.0f, "#mesh");
        // Editor::drawWidgetCheckbox("Render Wireframe", m_draw_wireframe, 90.0f, "#polygon_mode");
        // Editor::drawWidgetCheckbox("Render 3D Grid", m_draw_grid_points, 90.0f, "#3d_grid");
        // Editor::drawWidgetCheckbox("Render Mesh Grid Points", m_draw_mesh_grid_points, 90.0f, "#mesh_grid_points");
        // Editor::drawWidgetCheckbox("Render Surface Points", m_render_surface_points, 90.0f, "#surface_points");
        // Editor::drawWidgetCheckbox("Render Structural Springs", m_render_structural_springs, 90.0f, "#structural");
        // Editor::drawWidgetCheckbox("Render Shear Springs", m_render_shear_springs, 90.0f, "#shear");
        // Editor::drawWidgetCheckbox("Render Bend Springs", m_render_bend_springs, 90.0f, "#bend");
        // Editor::drawWidgetCheckbox("Render Surface Springs", m_render_surface_springs, 90.0f, "#surface_springs");
        // Editor::drawWidgetCheckbox("Render Internal Springs", m_render_internal_springs, 90.0f, "#internal_springs");
        // Editor::drawWidgetCheckbox("Snap to Ground", m_snap_to_ground, 90.0f, "#snap_to_ground");
    }
    ImGui::End();
}

void PhysicsRigidBodySandboxDemoLayer::drawAABB(const phx::PhxAABB& aabb, const mat4& model) {}

void PhysicsRigidBodySandboxDemoLayer::setupRaycastTests() {}

void PhysicsRigidBodySandboxDemoLayer::simulatePhysics(const double& total_time, const double& step_size)
{
    m_phx_world.startFrame();

    //const auto& force = 2.0f * phx::phxGenerateRandomUnitVector();
    //const auto& force = phx::phxGenerateRandomUnitVector();
    const PhxVec3 force = {0.5f, 0.25f, 0.4f};
    m_box_rb.addForceAtBodyPoint(force, {1.0f, 3.5f, 1.0f});

    m_phx_world.runPhysics(static_cast<PhxReal>(step_size));
}

void PhysicsRigidBodySandboxDemoLayer::resetSimulation() {}

} // namespace sputnik::demos
