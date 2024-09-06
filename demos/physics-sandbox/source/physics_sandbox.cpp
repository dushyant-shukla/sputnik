#include "physics_sandbox.hpp"
#include "phx/math_utils.hpp"
#include "phx/geometry_queries.hpp"
#include "phx/phx_utils.hpp"

#include <editor/editor.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include <algorithm>
#include <execution>
#include <ranges>
#include <chrono>

namespace sputnik::demos
{

using namespace sputnik::editor;

PhysicsSandboxDemoLayer::PhysicsSandboxDemoLayer(const std::string& name) : core::Layer{name}
{
    m_render_system = RenderSystem::getInstance();

    // Setup lihgt
    auto& light = m_render_system->getLight();
    // light.position = vec3(0.0f, 5.0f, 5.0f);
    light.position = vec3(0.0f, -115.0f, -85.0f);
    light.ambient  = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse  = vec3(1.0f, 1.0f, 1.0f);
    light.specular = vec3(1.0f, 1.0f, 1.0f);

    // setupTorus();
    // setupSuzanne();
    setupToy();
}

PhysicsSandboxDemoLayer::~PhysicsSandboxDemoLayer() {}

void PhysicsSandboxDemoLayer::setupTorus()
{
    m_suzanne = Model::LoadModel("../../data/assets/torus.glb");
    // m_suzanne = Model::LoadModel("../../data/assets/cute_triceratops.glb");
    //  m_suzanne = Model::LoadModel("../../data/assets/low_poly_rubiks_cube/game_ready_low_poly_rubiks_cube.glb");
    // m_cloth_diff_texture = std::make_shared<OglTexture2D>("../../data/assets/Material.001_baseColor.png", false);

    const auto& vertices = m_suzanne->getPositions();
    const auto& indices  = m_suzanne->getIndices();

    // phx::mad::MassAggregateBodySpec spec{
    //     .mass                       = 1.0f,
    //     .initial_velocity           = {},
    //     .acceleration               = {0.0f, -9.8f, 0.0f},
    //     .damping                    = 0.9f,
    //     .randomize_initial_velocity = false,
    //     .step                       = PhxVec3(0.075f),
    //     .nearest_neighbors          = 5,
    //     .structural_spring_coeffs   = {.ks = 2750.0f, .kd = 0.0001f},
    //     .shear_spring_coeffs        = {.ks = 2750.0f, .kd = 0.0001f},
    //     .flexion_spring_coeffs      = {.ks = 2250.0f, .kd = 0.0001f},
    //     .surface_spring_coeffs      = {.ks = 750.0f, .kd = 0.0001f},
    //     .internal_spring_coeffs     = {.ks = 750.0f, .kd = 0.0001f},
    // };

    // phx::mad::MassAggregateBodySpec spec{
    //     .mass                       = 1.0f,
    //     .initial_velocity           = {},
    //     .acceleration               = {0.0f, -9.8f, 0.0f},
    //     .damping                    = 0.9f,
    //     .randomize_initial_velocity = false,
    //     .step                       = PhxVec3(0.075f),
    //     .nearest_neighbors          = 5,
    //     .structural_spring_coeffs   = {.ks = 1500.0f, .kd = 0.0001f},
    //     .shear_spring_coeffs        = {.ks = 1500.0f, .kd = 0.0001f},
    //     .flexion_spring_coeffs      = {.ks = 1500.0f, .kd = 0.0001f},
    //     .surface_spring_coeffs      = {.ks = 1500.0f, .kd = 0.0001f},
    //     .internal_spring_coeffs     = {.ks = 750.0f, .kd = 0.0001f},
    // };

    phx::mad::MassAggregateBodySpec spec{
        .mass                       = 750.0f,
        .initial_velocity           = {},
        .acceleration               = {0.0f, -9.8f, 0.0f},
        .damping                    = 0.90f,
        .randomize_initial_velocity = false,
        .step                       = PhxVec3(0.080f),
        .nearest_neighbors          = 5,
        .structural_spring_coeffs   = {.ks = 250.0f, .kd = 0.000005f},
        .shear_spring_coeffs        = {.ks = 225.0f, .kd = 0.000005f},
        .flexion_spring_coeffs      = {.ks = 225.0f, .kd = 0.000005f},
        .surface_spring_coeffs      = {.ks = 0.125f, .kd = 0.000005f},
        .internal_spring_coeffs     = {.ks = 0.125f, .kd = 0.000005f},
    };

    const float height = 5.0f;

    // rml model matrix
    m_suzanne_model_mat = m_suzanne_model_mat.translate({0.0f, height, 0.0f});
    m_suzanne_model_mat = m_suzanne_model_mat.rotate({1.0, 0.0, 0.0}, -90.0f * kDegToRad);
    m_suzanne_model_mat = m_suzanne_model_mat.scale({0.5f});

    // glm model matrix
    m_suzanne_model_mat_glm = glm::mat4(1.0f);
    m_suzanne_model_mat_glm = glm::translate(m_suzanne_model_mat_glm, glm::vec3(0.0f, height, 0.0f));
    m_suzanne_model_mat_glm = glm::rotate(m_suzanne_model_mat_glm, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_suzanne_model_mat_glm = glm::scale(m_suzanne_model_mat_glm, glm::vec3(1.0f));

    PhxVec3Array phx_vertices;
    for(const auto& vertex : vertices)
    {
        PhxVec3 phx_vertex = m_suzanne_model_mat_glm * PhxVec4{vertex.x, vertex.y, vertex.z, 1.0f};
        m_initial_mesh_vertices.emplace_back(phx_vertex);
        phx_vertices.emplace_back(phx_vertex.x, phx_vertex.y, phx_vertex.z);
    }

    auto start                                    = std::chrono::system_clock::now();
    m_triangle_mesh                               = phx::phxCookTriangleMesh(phx_vertices, indices);
    m_triangle_mesh_copy                          = m_triangle_mesh_copy;
    auto                          finish          = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = finish - start;
    auto                          ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking triangle mesh: {} seconds | {} ms", elapsed_seconds, ms);

    // torus
    start                   = std::chrono::system_clock::now();
    m_aggregate_mass_volume = phx::phxCookMassAggregateVolume(m_triangle_mesh, spec, true, {0.0f, 1.0f, 0.0f});
    finish                  = std::chrono::system_clock::now();
    elapsed_seconds         = finish - start;
    ms                      = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking mass aggregate body: {} seconds | {} ms", elapsed_seconds, ms);
    // m_aggregate_mass_volume =
    //     phx::phxCookMassAggregateVolumeNearestNeighbor(m_triangle_mesh, spec, false, {0.0f, 1.0f, 0.0f});

    m_aggregate_mass_volume_copy = m_aggregate_mass_volume;
    m_deformable_body            = std::make_shared<phx::mad::DeformableBody>(m_triangle_mesh, m_aggregate_mass_volume);

    APPLICATION_INFO("Number of triangles processed: {}", static_cast<uint32_t>(indices.size() / 3));

    // PhxVec3Array input_vertices;
    // m_triangle_mesh->fetchVertices(input_vertices);
    // const PhxIndexArray& input_indices = m_triangle_mesh->getIndices();
    // m_vertex_buffer =
    //     std::make_shared<OglBuffer>((void*)input_vertices.data(), sizeof(PhxVec3) * input_vertices.size());
    // m_index_buffer = std::make_shared<OglBuffer>((void*)input_indices.data(), sizeof(PhxIndex) *
    // input_indices.size());

    // std::initializer_list<VertexInputAttributeSpecification> vertex_input_attributes{
    //     {.name = "position", .location = 0, .type = VertexAttributeType::Float3, .normalized = false},
    //     {.name = "normal", .location = 1, .type = VertexAttributeType::Float3, .normalized = false},
    //     {.name = "uv", .location = 2, .type = VertexAttributeType::Float2, .normalized = false},
    // };

    // VertexInputBindingSpecification vertex_input_binding{0, 8 * sizeof(float)};
    // m_vertex_array = std::make_shared<OglVertexArray>();
    // m_vertex_array->addVertexBuffer(*m_vertex_buffer.get(), vertex_input_binding, vertex_input_attributes);
    // m_vertex_array->setIndexBuffer(*m_index_buffer.get());
}

void PhysicsSandboxDemoLayer::setupSuzanne()
{
    // m_suzanne = Model::LoadModel("../../data/assets/suzanne_blender_monkey.glb");
    // m_suzanne = Model::LoadModel("../../data/assets/rock.gltf");
    m_suzanne = Model::LoadModel("../../data/assets/Box.glb");

    const auto& vertices = m_suzanne->getPositions();
    const auto& indices  = m_suzanne->getIndices();

    phx::mad::MassAggregateBodySpec spec{
        .mass                       = 250.0f,
        .initial_velocity           = {},
        .acceleration               = {0.0f, -9.8f, 0.0f},
        .damping                    = 0.90f,
        .randomize_initial_velocity = false,
        .step                       = PhxVec3(0.080f),
        .nearest_neighbors          = 5,
        .structural_spring_coeffs   = {.ks = 250.0f, .kd = 0.000005f},
        .shear_spring_coeffs        = {.ks = 225.0f, .kd = 0.000005f},
        .flexion_spring_coeffs      = {.ks = 225.0f, .kd = 0.000005f},
        .surface_spring_coeffs      = {.ks = 0.125f, .kd = 0.000005f},
        .internal_spring_coeffs     = {.ks = 0.125f, .kd = 0.000005f},
    };

    const float height = 5.0f;

    // rml model matrix
    m_suzanne_model_mat = m_suzanne_model_mat.translate({0.0f, height, 0.0f});
    m_suzanne_model_mat = m_suzanne_model_mat.rotate({1.0, 0.0, 0.0}, -90.0f * kDegToRad);
    m_suzanne_model_mat = m_suzanne_model_mat.scale({1.0f});

    // glm model matrix
    m_suzanne_model_mat_glm = glm::mat4(1.0f);
    m_suzanne_model_mat_glm = glm::translate(m_suzanne_model_mat_glm, glm::vec3(0.0f, height, 0.0f));
    m_suzanne_model_mat_glm = glm::rotate(m_suzanne_model_mat_glm, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_suzanne_model_mat_glm = glm::scale(m_suzanne_model_mat_glm, glm::vec3(1.0f));

    PhxVec3Array phx_vertices;
    for(const auto& vertex : vertices)
    {
        PhxVec3 phx_vertex = m_suzanne_model_mat_glm * PhxVec4{vertex.x, vertex.y, vertex.z, 1.0f};
        m_initial_mesh_vertices.emplace_back(phx_vertex);
        phx_vertices.emplace_back(phx_vertex.x, phx_vertex.y, phx_vertex.z);
    }

    auto start                                    = std::chrono::system_clock::now();
    m_triangle_mesh                               = phx::phxCookTriangleMesh(phx_vertices, indices);
    m_triangle_mesh_copy                          = m_triangle_mesh_copy;
    auto                          finish          = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = finish - start;
    auto                          ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking triangle mesh: {} seconds | {} ms", elapsed_seconds, ms);

    // suzanne
    start                   = std::chrono::system_clock::now();
    m_aggregate_mass_volume = phx::phxCookMassAggregateVolume(m_triangle_mesh, spec, true, {1.0f, 0.0f, 0.0f});
    // m_aggregate_mass_volume =
    //    phx::phxCookMassAggregateVolumeNearestNeighbor(m_triangle_mesh, spec, false, {1.0f, 0.0f, 0.0f});
    finish          = std::chrono::system_clock::now();
    elapsed_seconds = finish - start;
    ms              = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking mass aggregate body: {} seconds | {} ms", elapsed_seconds, ms);

    m_aggregate_mass_volume_copy = m_aggregate_mass_volume;
    m_deformable_body            = std::make_shared<phx::mad::DeformableBody>(m_triangle_mesh, m_aggregate_mass_volume);

    APPLICATION_INFO("Number of triangles processed: {}", static_cast<uint32_t>(indices.size() / 3));
    // APPLICATION_INFO("Triangle mesh has been built. BVH initialized.");
}

void PhysicsSandboxDemoLayer::setupToy()
{
    //m_suzanne            = Model::LoadModel("../../data/assets/kuma_plushie.glb");
    m_suzanne            = Model::LoadModel("../../data/assets/kuma_plushie/scene.gltf");
    m_cloth_diff_texture = std::make_shared<OglTexture2D>("../../data/assets/kuma_plushie/textures/bake_baseColor.png", false);

    const auto& vertices = m_suzanne->getPositions();
    const auto& indices  = m_suzanne->getIndices();

    phx::mad::MassAggregateBodySpec spec{
        .mass                       = 750.0f,
        .initial_velocity           = {},
        .acceleration               = {0.0f, -9.8f, 0.0f},
        .damping                    = 0.90f,
        .randomize_initial_velocity = false,
        .step                       = PhxVec3(0.065f),
        .nearest_neighbors          = 5,
        .structural_spring_coeffs   = {.ks = 250.0f, .kd = 0.0005f},
        .shear_spring_coeffs        = {.ks = 250.0f, .kd = 0.0005f},
        .flexion_spring_coeffs      = {.ks = 250.0f, .kd = 0.0005f},
        .surface_spring_coeffs      = {.ks = 0.125f, .kd = 0.000005f},
        .internal_spring_coeffs     = {.ks = 0.125f, .kd = 0.000005f},
    };

    const float height = 5.0f;

    // rml model matrix
    m_suzanne_model_mat = m_suzanne_model_mat.translate({0.0f, height, 0.0f});
    m_suzanne_model_mat = m_suzanne_model_mat.rotate({1.0, 0.0, 0.0}, 45.0f * kDegToRad);
    m_suzanne_model_mat = m_suzanne_model_mat.scale({0.25f});

    // glm model matrix
    m_suzanne_model_mat_glm = glm::mat4(1.0f);
    m_suzanne_model_mat_glm = glm::translate(m_suzanne_model_mat_glm, glm::vec3(0.0f, height, 0.0f));
    m_suzanne_model_mat_glm = glm::rotate(m_suzanne_model_mat_glm, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_suzanne_model_mat_glm = glm::scale(m_suzanne_model_mat_glm, glm::vec3(0.25f));

    PhxVec3Array phx_vertices;
    for(const auto& vertex : vertices)
    {
        PhxVec3 phx_vertex = m_suzanne_model_mat_glm * PhxVec4{vertex.x, vertex.y, vertex.z, 1.0f};
        m_initial_mesh_vertices.emplace_back(phx_vertex);
        phx_vertices.emplace_back(phx_vertex.x, phx_vertex.y, phx_vertex.z);
    }

    auto start                                    = std::chrono::system_clock::now();
    m_triangle_mesh                               = phx::phxCookTriangleMesh(phx_vertices, indices);
    m_triangle_mesh_copy                          = m_triangle_mesh_copy;
    auto                          finish          = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = finish - start;
    auto                          ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking triangle mesh: {} seconds | {} ms", elapsed_seconds, ms);

    // torus
    start                   = std::chrono::system_clock::now();
    m_aggregate_mass_volume = phx::phxCookMassAggregateVolume(m_triangle_mesh, spec, false, {0.0f, 1.0f, 0.0f});
    finish                  = std::chrono::system_clock::now();
    elapsed_seconds         = finish - start;
    ms                      = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds).count();
    APPLICATION_INFO("Time for cooking mass aggregate body: {} seconds | {} ms", elapsed_seconds, ms);
    // m_aggregate_mass_volume =
    //     phx::phxCookMassAggregateVolumeNearestNeighbor(m_triangle_mesh, spec, false, {0.0f, 1.0f, 0.0f});

    m_aggregate_mass_volume_copy = m_aggregate_mass_volume;
    m_deformable_body            = std::make_shared<phx::mad::DeformableBody>(m_triangle_mesh, m_aggregate_mass_volume);

    APPLICATION_INFO("Number of triangles processed: {}", static_cast<uint32_t>(indices.size() / 3));
}

void PhysicsSandboxDemoLayer::OnAttach() {}

void PhysicsSandboxDemoLayer::OnDetach() {}

void PhysicsSandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    if(m_reset_simulation)
    {
        resetSimulation();
    }

    if(m_simulate_physics)
    {
        double frame_time = time_step.GetSeconds();
        m_accumulated_time += frame_time;
        while(m_accumulated_time >= kStepSize)
        {
            simulatePhysics(m_total_time, kStepSize);
            m_accumulated_time -= kStepSize;
            m_total_time += kStepSize;
        }
    }

    glm::vec3 light_pos = glm::vec3(0.0f, 5.0f, 5.0f);

    // render suzanne
    if(m_draw_mesh && !m_draw_wireframe)
    {
        Material material    = material_white;
        material.shader_name = "blinn_phong";

        Material material_cloth;
        material_cloth.diff_texture = m_cloth_diff_texture;

        PhxVec3Array input_vertices;
        m_triangle_mesh->fetchVertices(input_vertices);
        m_suzanne->updatePositionBuffer((void*)input_vertices.data(), sizeof(PhxVec3) * input_vertices.size());

        //  Update normal buffer
        const auto& indices = m_triangle_mesh->getIndices();
        auto        normals = m_suzanne->getNormals();
        for(PhxSize index = 0; index < indices.size(); index += 3)
        {

            PhxIndex           i0     = indices[index];
            PhxIndex           i1     = indices[index + 1];
            PhxIndex           i2     = indices[index + 2];
            PhxVec3            v0     = input_vertices[i0];
            PhxVec3            v1     = input_vertices[i1];
            PhxVec3            v2     = input_vertices[i2];
            PhxVec3            a      = v1 - v0;
            PhxVec3            b      = v2 - v0;
            PhxVec3            normal = phx_cross(a, b);
            ramanujan::Vector3 rml_normal{normal.x, normal.y, normal.z};
            normals[i0] = normals[i0] + rml_normal;
            normals[i1] = normals[i1] + rml_normal;
            normals[i2] = normals[i2] + rml_normal;
        }
        m_suzanne->updateNormalBuffer((void*)normals.data(), sizeof(ramanujan::Vector3) * normals.size());

        PhxVec3Array updated_normals;

        // m_suzanne->draw(material_cloth, {});
        m_suzanne->draw(material_cloth, {});
    }

    if(m_draw_wireframe)
    {
        PhxVec3Array input_vertices;
        m_triangle_mesh->fetchVertices(input_vertices);
        m_suzanne->updatePositionBuffer((void*)input_vertices.data(), sizeof(PhxVec3) * input_vertices.size());

        //  Update normal buffer
        const auto& indices = m_triangle_mesh->getIndices();
        auto        normals = m_suzanne->getNormals();
        for(PhxSize index = 0; index < indices.size(); index += 3)
        {

            PhxIndex           i0     = indices[index];
            PhxIndex           i1     = indices[index + 1];
            PhxIndex           i2     = indices[index + 2];
            PhxVec3            v0     = input_vertices[i0];
            PhxVec3            v1     = input_vertices[i1];
            PhxVec3            v2     = input_vertices[i2];
            PhxVec3            a      = v1 - v0;
            PhxVec3            b      = v2 - v0;
            PhxVec3            normal = phx_cross(a, b);
            ramanujan::Vector3 rml_normal{normal.x, normal.y, normal.z};
            normals[i0] = normals[i0] + rml_normal;
            normals[i1] = normals[i1] + rml_normal;
            normals[i2] = normals[i2] + rml_normal;
        }
        m_suzanne->updateNormalBuffer((void*)normals.data(), sizeof(ramanujan::Vector3) * normals.size());

        Material material    = material_white;
        material.shader_name = "blinn_phong";

        glLineWidth(2.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_suzanne->draw(material, {});

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.0f);
    }

    // render bvh
    const auto& bvh = m_triangle_mesh->getBvh();
    if(m_draw_bvh)
    {
        const auto& root_aabb = bvh->getNodes()[0].aabb;
        // drawAABB(root_aabb, {});
        for(const auto& node : bvh->getNodes())
        {
            if(node.isLeaf())
            {
                drawAABB(node.aabb, {});
            }
        }
    }

    // render grid
    if(m_draw_grid_points)
    {
        std::vector<vec4> rml_grid_points;
        PhxSize           count             = m_aggregate_mass_volume->getParticleCount();
        PhxIndex          surface_point_idx = m_aggregate_mass_volume->getSurfacePaticleIndex();
        // for(PhxSize i = 0; i < count; i++)
        for(PhxSize i = 0; i < surface_point_idx; i++)
        {
            if(!m_aggregate_mass_volume->getIsValid(PhxIndex(i)))
            {
                PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                rml_grid_points.emplace_back(position.x, position.y, position.z, 1.0f);
            }
        }
        if(!rml_grid_points.empty())
        {
            m_render_system->drawDebugPoints(rml_grid_points, {1.0f, 0.0f, 0.0f}, mat4{}, 5.0f);
        }
    }

    // render point masses
    if(m_draw_mesh_grid_points)
    {
        std::vector<vec4> rml_point_positions;
        PhxSize           count             = m_aggregate_mass_volume->getParticleCount();
        PhxIndex          surface_point_idx = m_aggregate_mass_volume->getSurfacePaticleIndex();
        // for(PhxSize i = 0; i < count; i++)
        for(PhxSize i = 0; i < surface_point_idx; i++)
        {
            if(m_aggregate_mass_volume->getIsValid(PhxIndex(i)))
            {
                PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                rml_point_positions.emplace_back(position.x, position.y, position.z, 1.0f);
            }
        }
        if(!rml_point_positions.empty())
        {
            m_render_system->drawDebugPoints(rml_point_positions, {0.0f, 0.0f, 1.0f}, mat4{}, 5.0f);
        }
    }

    if(m_render_surface_points)
    {
        std::vector<vec4> rml_point_positions;
        PhxSize           count             = m_aggregate_mass_volume->getParticleCount();
        PhxIndex          surface_point_idx = m_aggregate_mass_volume->getSurfacePaticleIndex();
        for(PhxSize i = surface_point_idx; i < count; i++)
        {
            if(m_aggregate_mass_volume->getIsValid(PhxIndex(i)))
            {
                PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                rml_point_positions.emplace_back(position.x, position.y, position.z, 1.0f);
            }
        }
        if(!rml_point_positions.empty())
        {
            m_render_system->drawDebugPoints(rml_point_positions, material_ruby.diffuse, mat4{}, 7.5f);
        }
    }

    std::vector<vec4> lines;
    if(m_render_structural_springs)
    {
        const auto& structural_springs = m_aggregate_mass_volume->getStructuralSprings();
        for(const auto& spring : structural_springs)
        {
            PhxUvec3 coords     = m_aggregate_mass_volume->getLocalCoordinates(spring.mass_a_idx);
            PhxVec3  position_a = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            coords              = m_aggregate_mass_volume->getLocalCoordinates(spring.mass_b_idx);
            PhxVec3 position_b  = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a.x, position_a.y, position_a.z, 1.0f});
            lines.push_back({position_b.x, position_b.y, position_b.z, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_emerald.diffuse, 5.0f);
        }
    }

    if(m_render_shear_springs)
    {
        lines.clear();
        const auto& shear_spring = m_aggregate_mass_volume->getShearSprings();
        for(const auto& pair : shear_spring)
        {
            PhxUvec3 coords     = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_a_idx);
            PhxVec3  position_a = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            coords              = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_b_idx);
            PhxVec3 position_b  = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a.x, position_a.y, position_a.z, 1.0f});
            lines.push_back({position_b.x, position_b.y, position_b.z, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_blue_shine.diffuse, 5.0f);
        }
    }

    if(m_render_bend_springs)
    {
        lines.clear();
        const auto& bend_springs = m_aggregate_mass_volume->getFlexionSprings();
        for(const auto& pair : bend_springs)
        {
            PhxUvec3 coords     = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_a_idx);
            PhxVec3  position_a = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            coords              = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_b_idx);
            PhxVec3 position_b  = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a.x, position_a.y, position_a.z, 1.0f});
            lines.push_back({position_b.x, position_b.y, position_b.z, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_gold.diffuse, 5.0f);
        }
    }

    if(m_render_surface_springs)
    {
        lines.clear();
        const auto& surface_springs = m_aggregate_mass_volume->getSurfaceSprings();

        for(const auto& pair : surface_springs)
        {
            PhxUvec3 coords     = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_a_idx);
            PhxVec3  position_a = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            coords              = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_b_idx);
            PhxVec3 position_b  = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a.x, position_a.y, position_a.z, 1.0f});
            lines.push_back({position_b.x, position_b.y, position_b.z, 1.0f});
        }

        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, material_silver.diffuse, 5.0f);
        }
    }

    if(m_render_internal_springs)
    {
        lines.clear();
        const auto& internal_springs = m_aggregate_mass_volume->getInternalSprings();
        for(const auto& pair : internal_springs)
        {
            PhxUvec3 coords     = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_a_idx);
            PhxVec3  position_a = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            coords              = m_aggregate_mass_volume->getLocalCoordinates(pair.mass_b_idx);
            PhxVec3 position_b  = m_aggregate_mass_volume->getPosition(coords.x, coords.y, coords.z);
            lines.push_back({position_a.x, position_a.y, position_a.z, 1.0f});
            lines.push_back({position_b.x, position_b.y, position_b.z, 1.0f});
        }
        if(!lines.empty())
        {
            m_render_system->drawDebugLines(lines, {1.0f, 0.0f, 0.0f}, 5.0f);
        }
    }

    setupRaycastTests();
}

void PhysicsSandboxDemoLayer::setupRaycastTests()
{
    glm::vec3        v0 = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3        v1 = glm::vec3(-1.0f, 3.0f, 0.0f);
    glm::vec3        v2 = glm::vec3(1.0f, 3.0f, 0.0f);
    phx::PhxTriangle triangle{v0, v1, v2};
    triangle.calculateCentroid();
    {

        std::vector<vec4> lines;
        lines.emplace_back(v0.x, v0.y, v0.z, 1.0f);
        lines.emplace_back(v1.x, v1.y, v1.z, 1.0f);
        lines.emplace_back(v1.x, v1.y, v1.z, 1.0f);
        lines.emplace_back(v2.x, v2.y, v2.z, 1.0f);
        lines.emplace_back(v2.x, v2.y, v2.z, 1.0f);
        lines.emplace_back(v0.x, v0.y, v0.z, 1.0f);
        m_render_system->drawDebugLines(lines, {1.0f, 0.0f, 0.0f}, mat4{}, 5.0f);

        phx::PhxRay ray = {};
        ray.origin      = glm::vec3(0.0f, 4.0f, 1.0f);
        ray.direction   = glm::vec3(0.0f, 0.0f, -1.0f);

        std::vector<vec4> ray_line;
        ray_line.emplace_back(ray.origin.x, ray.origin.y, ray.origin.z, 1.0f);
        ray_line.emplace_back(ray.origin.x + ray.direction.x * 2.0f,
                              ray.origin.y + ray.direction.y * 2.0f,
                              ray.origin.z + ray.direction.z * 2.0f,
                              1.0f);

        phx::PhxRaycastResult result = {};
        if(phx::raycastTriangle(ray, triangle, result))
        {
            m_render_system->drawDebugLines(ray_line, {0.0f, 1.0f, 0.0f}, mat4{}, 5.0f);
        }
        else
        {
            m_render_system->drawDebugLines(ray_line, {1.0f, 0.0f, 0.0f}, mat4{}, 5.0f);
        }
    }
}

void PhysicsSandboxDemoLayer::simulatePhysics(const double& total_time, const double& step_size)
{
    m_deformable_body->update(total_time, step_size);
    // integrateExplicitEuler(total_time, step_size);
    integrateSemiImplicitEuler(total_time, step_size);
    // integrateVerlet(total_time, step_size);
}

void PhysicsSandboxDemoLayer::integrateExplicitEuler(const double& total_time, const double& step_size)
{
    PhxSize                num_particles = m_aggregate_mass_volume->getParticleCount();
    std::ranges::iota_view indexes((size_t)0, num_particles);
    std::for_each(
        std::execution::par_unseq,
        indexes.begin(),
        indexes.end(),
        [&](const auto& i)
        {
            PhxBool is_fixed      = m_aggregate_mass_volume->getIsFixed(PhxIndex(i));
            PhxBool is_valid      = m_aggregate_mass_volume->getIsValid(PhxIndex(i));
            PhxBool is_mass_valid = m_aggregate_mass_volume->getInverseMass(PhxIndex(i)) > phx::kPhxEpsilon;
            PhxVec3 old_position  = m_aggregate_mass_volume->getPosition(PhxIndex(i));
            if(!is_fixed && is_valid && is_mass_valid)
            {
                PhxVec3 velocity = m_aggregate_mass_volume->getVelocity(PhxIndex(i));
                PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                position += velocity *
                            (float)(std::pow(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)), step_size) - 1.0f) /
                            std::log(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)));
                PhxVec3 total_acceleration = m_aggregate_mass_volume->getAcceleration(PhxIndex(i));
                total_acceleration += m_aggregate_mass_volume->getAccumulatedForce(PhxIndex(i)) *
                                      m_aggregate_mass_volume->getInverseMass(PhxIndex(i));
                velocity += total_acceleration * (float)step_size;
                velocity *= std::pow(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)), step_size);
                m_aggregate_mass_volume->setVelocity(PhxIndex(i), velocity);
                m_aggregate_mass_volume->setAccumulatedForce(PhxIndex(i), {0.0f, 0.0f, 0.0f});
                // if(CMP_FLOAT_EQ(position.y, -30.0f))
                if(position.y < phx::kPhxEpsilon)
                {
                    // APPLICATION_INFO("Particle[{}] Position: [{},{},{}]", i, position.x, position.y, position.z);
                    position.y = old_position.y;
                }
                if(m_snap_to_ground)
                {
                    if(phx_abs(position.x) < 0.5f)
                    {
                        position.y = position.y * 0.5f;
                    }
                    // APPLICATION_INFO("Particle [{}] Snapped Position: [{},{},{}]",
                    //                  i,
                    //                  position.x,
                    //                  position.y,
                    //                  position.z);
                }
                m_aggregate_mass_volume->setPosition(PhxIndex(i), position);
            }
        });
}

void PhysicsSandboxDemoLayer::integrateSemiImplicitEuler(const double& total_time, const double& step_size)
{
    PhxSize                num_particles = m_aggregate_mass_volume->getParticleCount();
    std::ranges::iota_view indexes((size_t)0, num_particles);
    std::for_each(std::execution::par_unseq,
                  indexes.begin(),
                  indexes.end(),
                  [&](const auto& i)
                  {
                      PhxBool is_fixed = m_aggregate_mass_volume->getIsFixed(PhxIndex(i));
                      PhxBool is_valid = m_aggregate_mass_volume->getIsValid(PhxIndex(i));
                      // PhxBool is_valid      = true;
                      PhxBool is_mass_valid = m_aggregate_mass_volume->getInverseMass(PhxIndex(i)) > phx::kPhxEpsilon;
                      PhxVec3 old_position  = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                      if(!is_fixed && is_valid && is_mass_valid)
                      {
                          PhxVec3 total_acceleration = m_aggregate_mass_volume->getAcceleration(PhxIndex(i));
                          total_acceleration += m_aggregate_mass_volume->getAccumulatedForce(PhxIndex(i)) *
                                                m_aggregate_mass_volume->getInverseMass(PhxIndex(i));
                          PhxVec3 velocity = m_aggregate_mass_volume->getVelocity(PhxIndex(i));
                          velocity += total_acceleration * (float)step_size;
                          // velocity *= std::pow(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)), step_size);
                          velocity *= m_aggregate_mass_volume->getDampingValue(PhxIndex(i));
                          // if(phx_magnitude_sq(velocity) > 49.0f)
                          //{
                          //     velocity = phx_normalize(velocity) * 7.0f;
                          // }
                          PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                          // position += velocity *
                          //             (float)(std::pow(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)),
                          //             step_size) - 1.0f) /
                          //             std::log(m_aggregate_mass_volume->getDampingValue(PhxIndex(i)));
                          position += velocity * static_cast<float>(step_size);
                          m_aggregate_mass_volume->setVelocity(PhxIndex(i), velocity);
                          m_aggregate_mass_volume->setAccumulatedForce(PhxIndex(i), {0.0f, 0.0f, 0.0f});
                          // if(CMP_FLOAT_EQ(position.y, -30.0f))
                          if(position.y < phx::kPhxEpsilon)
                          {
                              // APPLICATION_INFO("Particle[{}] Position: [{},{},{}]", i, position.x, position.y,
                              // position.z);
                              position.y = old_position.y;
                          }
                          if(m_snap_to_ground)
                          {
                              // position.y = position.y * 0.75f;
                              position.y = position.y * 0.50f;
                              // APPLICATION_INFO("Particle [{}] Snapped Position: [{},{},{}]",
                              //                  i,
                              //                  position.x,
                              //                  position.y,
                              //                  position.z);
                          }
                          m_aggregate_mass_volume->setPosition(PhxIndex(i), position);
                      }
                  });
}

void PhysicsSandboxDemoLayer::integrateImplicitEuler(const double& total_time, const double& step_size) {}

void PhysicsSandboxDemoLayer::integrateVerlet(const double& total_time, const double& step_size)
{
    PhxSize                num_particles = m_aggregate_mass_volume->getParticleCount();
    std::ranges::iota_view indexes((size_t)0, num_particles);
    std::for_each(std::execution::par_unseq,
                  indexes.begin(),
                  indexes.end(),
                  [&](const auto& i)
                  {
                      PhxBool is_fixed      = m_aggregate_mass_volume->getIsFixed(PhxIndex(i));
                      PhxBool is_valid      = m_aggregate_mass_volume->getIsValid(PhxIndex(i));
                      PhxBool is_mass_valid = m_aggregate_mass_volume->getInverseMass(PhxIndex(i)) > phx::kPhxEpsilon;
                      if(!is_fixed && is_valid && is_mass_valid)
                      {
                          PhxVec3 position      = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                          PhxVec3 temp_position = position;
                          PhxVec3 prev_position = m_aggregate_mass_volume->getPrevPosition(PhxIndex(i));

                          PhxVec3 new_position = position + (position - prev_position) * (1.0f - 0.01f) +
                                                 m_aggregate_mass_volume->getAcceleration(PhxIndex(i)) *
                                                     (float)step_size * (float)step_size;
                          m_aggregate_mass_volume->setPrevPosition(PhxIndex(i), temp_position);
                          m_aggregate_mass_volume->setPosition(PhxIndex(i), new_position);
                          m_aggregate_mass_volume->setAccumulatedForce(PhxIndex(i), {0.0f, 0.0f, 0.0f});
                      }
                  });
}

void PhysicsSandboxDemoLayer::resetSimulation()
{
    m_total_time       = 0.0;
    m_accumulated_time = 0.0;
    m_simulate_physics = false;

    m_triangle_mesh         = m_triangle_mesh_copy;
    m_aggregate_mass_volume = m_aggregate_mass_volume_copy;
    m_deformable_body       = std::make_shared<phx::mad::DeformableBody>(m_triangle_mesh, m_aggregate_mass_volume);
    // m_deformable_body->reset();

    m_reset_simulation = false;
}

void PhysicsSandboxDemoLayer::OnEvent() {}

void PhysicsSandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Physics Sandbox"))
    {
        Editor::drawWidgetCheckbox("Simulate Physics", m_simulate_physics, 90.0f, "#simulate_physics");
        Editor::drawWidgetCheckbox("Reset Simulation", m_reset_simulation, 90.0f, "#reset_simulation");
        Editor::drawWidgetCheckbox("Render BVH", m_draw_bvh, 90.0f, "#bvh");
        Editor::drawWidgetCheckbox("Render Mesh", m_draw_mesh, 90.0f, "#mesh");
        Editor::drawWidgetCheckbox("Render Wireframe", m_draw_wireframe, 90.0f, "#polygon_mode");
        Editor::drawWidgetCheckbox("Render 3D Grid", m_draw_grid_points, 90.0f, "#3d_grid");
        Editor::drawWidgetCheckbox("Render Mesh Grid Points", m_draw_mesh_grid_points, 90.0f, "#mesh_grid_points");
        Editor::drawWidgetCheckbox("Render Surface Points", m_render_surface_points, 90.0f, "#surface_points");
        Editor::drawWidgetCheckbox("Render Structural Springs", m_render_structural_springs, 90.0f, "#structural");
        Editor::drawWidgetCheckbox("Render Shear Springs", m_render_shear_springs, 90.0f, "#shear");
        Editor::drawWidgetCheckbox("Render Bend Springs", m_render_bend_springs, 90.0f, "#bend");
        Editor::drawWidgetCheckbox("Render Surface Springs", m_render_surface_springs, 90.0f, "#surface_springs");
        Editor::drawWidgetCheckbox("Render Internal Springs", m_render_internal_springs, 90.0f, "#internal_springs");
        Editor::drawWidgetCheckbox("Snap to Ground", m_snap_to_ground, 90.0f, "#snap_to_ground");
    }
    ImGui::End();
}

void PhysicsSandboxDemoLayer::drawAABB(const phx::PhxAABB& aabb, const mat4& model)
{
    glm::vec3 aabb_min = aabb.min;
    glm::vec3 aabb_max = aabb.max;

    std::vector<ramanujan::experimental::vec4> points;

    points.emplace_back(aabb_min.x, aabb_min.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_min.x, aabb_max.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_min.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_min.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_max.x, aabb_min.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_max.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_max.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_max.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_max.x, aabb_min.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_min.y, aabb_min.z, 1.0f);

    points.emplace_back(aabb_max.x, aabb_min.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_max.y, aabb_min.z, 1.0f);

    points.emplace_back(aabb_max.x, aabb_max.y, aabb_max.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_max.y, aabb_min.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_max.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_max.y, aabb_min.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_min.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_max.x, aabb_min.y, aabb_min.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_min.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_min.x, aabb_min.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_max.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_min.x, aabb_max.y, aabb_max.z, 1.0f);

    points.emplace_back(aabb_min.x, aabb_min.y, aabb_min.z, 1.0f);
    points.emplace_back(aabb_min.x, aabb_max.y, aabb_min.z, 1.0f);

    m_render_system->drawDebugLines(points, {0.0f, 0.0f, 1.0f}, model, 2.5f);
}

} // namespace sputnik::demos

////////////////////////////////////////////////////////////////////////////
//
// A raycast test to create 100 random rays and test them against a triangle
//
//{
// glm::mat4 model = glm::mat4(1.0f);
// model           = glm::translate(model, glm::vec3(0.0f, 2.0f, 1.0f));
// std::vector<vec4> points;
// for(size_t i = 0; i < 100; i++)
//{
//     glm::vec3 random_direction = phx::generateRandomUnitVector();
//     points.push_back(vec4(random_direction.x, random_direction.y, random_direction.z, 1.0f));

//    phx::Ray ray              = {};
//    ray.origin                = glm::vec3(0.0f, 2.0f, 1.0f);
//    ray.direction             = random_direction;
//    phx::RayCastResult result = {};
//    if(phx::raycastTriangle(ray, triangle, result))
//    {
//        std::vector<vec4> line;
//        line.push_back({ray.origin.x, ray.origin.y, ray.origin.z, 1.0});
//        line.push_back({result.point.x, result.point.y, result.point.z, 1.0});
//        m_render_system->drawDebugLines(line, {0.0f, 1.0f, 0.0f}, mat4{}, 5.0f);
//    }
//    else
//    {
//        m_render_system->drawDebugPoints(
//            {vec4(random_direction.x, random_direction.y, random_direction.z, 1.0f)},
//            {0.0f, 0.0f, 1.0f},
//            model,
//            5.0f);
//    }

//    std::vector<phx::RayCastResult> results;
//    if(phx::raycastTriangleMesh(ray, *m_triangle_mesh.get(), results))
//    {
//        phx::RayCastResult result = results[0];
//        std::vector<vec4>  line;
//        line.push_back({ray.origin.x, ray.origin.y, ray.origin.z, 1.0});
//        line.push_back({result.point.x, result.point.y, result.point.z, 1.0});
//        m_render_system->drawDebugLines(line, {0.0f, 1.0f, 0.0f}, mat4{}, 5.0f);
//    }
//}
// m_render_system->drawDebugPoints(points, {0.0f, 0.0f, 1.0f}, model, 5.0f);

// m_render_system->drawDebugLines(m_mesh_intersection_rays, {0.0f, 1.0f, 1.0f}, mat4{}, 3.0f);
//}