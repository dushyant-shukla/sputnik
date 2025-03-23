#include "physics_rb_sandbox.hpp"

namespace sputnik::demos
{

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

PhysicsRigidBodySandboxDemoLayer::PhysicsRigidBodySandboxDemoLayer(const std::string& name) : core::Layer{name}
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
}

PhysicsRigidBodySandboxDemoLayer::~PhysicsRigidBodySandboxDemoLayer() {}

void PhysicsRigidBodySandboxDemoLayer::OnAttach() {}

void PhysicsRigidBodySandboxDemoLayer::OnDetach() {}

void PhysicsRigidBodySandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    RenderSystem* render_system = RenderSystem::getInstance();

    m_vertex_array->bind();

    {
        mat4 model = {};
        model      = model.translate({0.0f, 3.0f, 0.0f});
        model      = model.scale({0.25f});
        render_system->drawTriangles(36, material_ruby, model);
    }

    m_vertex_array->unbind();
}

void PhysicsRigidBodySandboxDemoLayer::OnEvent() {}

void PhysicsRigidBodySandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step) {}

void PhysicsRigidBodySandboxDemoLayer::drawAABB(const phx::PhxAABB& aabb, const mat4& model) {}

void PhysicsRigidBodySandboxDemoLayer::setupRaycastTests() {}

void PhysicsRigidBodySandboxDemoLayer::simulatePhysics(const double& total_time, const double& step_size) {}

void PhysicsRigidBodySandboxDemoLayer::resetSimulation() {}

} // namespace sputnik::demos
