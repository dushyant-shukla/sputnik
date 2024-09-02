#include "physics_sandbox.hpp"
#include "phx/math_utils.hpp"
#include "phx/geometry_queries.hpp"
#include "phx/phx_utils.hpp"

#include <editor/editor.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace sputnik::demos
{

using namespace sputnik::editor;

PhysicsSandboxDemoLayer::PhysicsSandboxDemoLayer(const std::string& name) : core::Layer{name}
{
    m_render_system = RenderSystem::getInstance();

    // Setup lihgt
    auto& light    = m_render_system->getLight();
    light.position = vec3(0.0f, 5.0f, 5.0f);
    light.ambient  = vec3(1.0f, 1.0f, 1.0f);
    light.diffuse  = vec3(1.0f, 1.0f, 1.0f);
    light.specular = vec3(1.0f, 1.0f, 1.0f);

    m_suzanne            = Model::LoadModel("../../data/assets/suzanne_blender_monkey.glb");
    const auto& vertices = m_suzanne->getPositions();
    const auto& indices  = m_suzanne->getIndices();

    // setupDeformableBody();

    // glm::mat4 model = glm::mat4(1.0f);
    // model           = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model           = glm::scale(model, glm::vec3(2.0f));
    //  model           = glm::mat4(1.0f);

    // m_triangle_mesh =
    //     std::make_shared<phx::PhxTriangleMesh>(static_cast<uint32_t>(indices.size() / 3)); // 3 indices per triangle
    // for(uint32_t i = 0; i < indices.size(); i += 3)
    //{
    //     uint32_t         index = indices[i];
    //     phx::PhxTriangle triangle{.a = glm::vec3(vertices[index].x, vertices[index].y, vertices[index].z),
    //                               .b = glm::vec3(vertices[index + 1].x, vertices[index + 1].y, vertices[index +
    //                               1].z), .c = glm::vec3(vertices[index + 2].x, vertices[index + 2].y, vertices[index
    //                               + 2].z)};
    //     triangle.calculateCentroid();
    //     m_triangle_mesh->addTriangle(triangle);
    // }
    // m_triangle_mesh->buildAccelerationStructure();

    // const auto& bvh       = m_triangle_mesh->getBvh();
    // const auto& root_aabb = bvh->getNodes()[0].aabb;
    // generate3DGrid(m_grid_points, root_aabb.min, root_aabb.max, glm::vec3(0.075f));

    phx::mad::MassAggregateBodySpec spec{.mass                       = 0.75f,
                                         .initial_velocity           = {},
                                         .acceleration               = {},
                                         .damping                    = 0.0f,
                                         .randomize_initial_velocity = false,
                                         .step                       = PhxVec3(0.075f),
                                         .structural_spring_coeffs   = {},
                                         .shear_spring_coeffs        = {},
                                         .flexion_spring_coeffs      = {}};

    PhxVec3Array phx_vertices;
    for(const auto& vertex : vertices)
    {
        phx_vertices.emplace_back(vertex.x, vertex.y, vertex.z);
    }

    m_triangle_mesh         = phx::phxCookTriangleMesh(phx_vertices, indices);
    m_aggregate_mass_volume = phx::phxCookMassAggregateVolume(m_triangle_mesh, spec);
    m_deformable_body       = std::make_shared<phx::mad::DeformableBody>(m_triangle_mesh, m_aggregate_mass_volume);

    APPLICATION_INFO("Number of triangles: {}", static_cast<uint32_t>(indices.size() / 3));
    APPLICATION_INFO("Triangle mesh has been built. BVH initialized.");
}

PhysicsSandboxDemoLayer::~PhysicsSandboxDemoLayer() {}

void PhysicsSandboxDemoLayer::OnAttach() {}

void PhysicsSandboxDemoLayer::OnDetach() {}

void PhysicsSandboxDemoLayer::OnUpdate(const core::TimeStep& time_step)
{
    glm::vec3 light_pos = glm::vec3(0.0f, 5.0f, 5.0f);

    // render suzanne
    mat4 model = {};
    if(!m_disable_mesh_rendering)
    {
        // model                = model.rotate({1.0, 0.0, 0.0}, -90.0f * kDegToRad);
        // model                = model.scale({1.0f});
        model                = mat4{};
        Material material    = material_white;
        material.shader_name = "blinn_phong";

        if(m_draw_wireframe)
        {
            glLineWidth(2.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        m_suzanne->draw(material, model);
        if(m_draw_wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glLineWidth(1.0f);
        }
    }

    // render bvh
    const auto& bvh = m_triangle_mesh->getBvh();
    if(m_draw_bvh)
    {
        const auto& root_aabb = bvh->getNodes()[0].aabb;

        drawAABB(root_aabb, {});

        // drawAABB(bvh->getNodes()[1].aabb);
        // drawAABB(bvh->getNodes()[2].aabb);

        for(const auto& node : bvh->getNodes())
        {
            if(node.isLeaf())
            {
                // drawAABB(node.aabb, model);
                drawAABB(node.aabb, {});
                // APPLICATION_INFO("Leaf node prim count: {}", node.num_primitives);
                // break;
            }
        }
    }

    // render grid
    if(m_draw_grid_points)
    {
        // m_draw_mesh_grid_points = false;
        std::vector<vec4> rml_grid_points;

        // std::for_each(m_grid_points.begin(),
        //               m_grid_points.end(),
        //               [&rml_grid_points](const glm::vec3& point)
        //               { rml_grid_points.emplace_back(point.x, point.y, point.z, 1.0f); });

        PhxSize count = m_aggregate_mass_volume->getParticleCount();
        for(PhxSize i = 0; i < count; i++)
        {
            PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
            rml_grid_points.emplace_back(position.x, position.y, position.z, 1.0f);
        }
        m_render_system->drawDebugPoints(rml_grid_points, {1.0f, 0.0f, 0.0f}, model, 5.0f);
    }

    // render point masses
    if(m_draw_mesh_grid_points)
    {
        // m_draw_grid_points = false;
        // if(!m_point_positions.empty())
        //{
        //    std::vector<vec4> rml_point_positions;
        //    std::for_each(m_point_positions.begin(),
        //                  m_point_positions.end(),
        //                  [&rml_point_positions](const glm::vec3& point)
        //                  { rml_point_positions.emplace_back(point.x, point.y, point.z, 1.0f); });
        //    m_render_system->drawDebugPoints(rml_point_positions, {0.0f, 0.0f, 1.0f}, model, 5.0f);
        //}

        std::vector<vec4> rml_point_positions;
        PhxSize           count = m_aggregate_mass_volume->getParticleCount();
        for(PhxSize i = 0; i < count; i++)
        {
            if(m_aggregate_mass_volume->getIsValid(PhxIndex(i)))
            {
                PhxVec3 position = m_aggregate_mass_volume->getPosition(PhxIndex(i));
                rml_point_positions.emplace_back(position.x, position.y, position.z, 1.0f);
            }
        }
        m_render_system->drawDebugPoints(rml_point_positions, {0.0f, 0.0f, 1.0f}, model, 5.0f);
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

        // m_render_system->drawDebugPoints({{ray.origin.x, ray.origin.y, ray.origin.z, 1.0f}},
        //                                  {0.0f, 1.0f, 0.0f},
        //                                  mat4{},
        //                                  5.0f);
    }

    {
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
    }
}

void PhysicsSandboxDemoLayer::setupDeformableBody()
{
    // const auto& vertices = m_suzanne->getPositions();
    // const auto& indices  = m_suzanne->getIndices();
    // m_deformable_body    = std::make_shared<phx::mad::DeformableBody>(static_cast<uint32_t>(indices.size() / 3));
    // for(uint32_t i = 0; i < indices.size(); i += 3)
    //{
    //     uint32_t      index = indices[i];
    //     phx::PhxTriangle triangle{.a = glm::vec3(vertices[index].x, vertices[index].y, vertices[index].z),
    //                            .b = glm::vec3(vertices[index + 1].x, vertices[index + 1].y, vertices[index + 1].z),
    //                            .c = glm::vec3(vertices[index + 2].x, vertices[index + 2].y, vertices[index + 2].z)};
    //     triangle.calculateCentroid();
    //     m_deformable_body->addTriangle(triangle);
    // }
    // m_deformable_body->setup();
}

void PhysicsSandboxDemoLayer::OnEvent() {}

void PhysicsSandboxDemoLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    if(ImGui::Begin("Physics Sandbox"))
    {
        Editor::drawWidgetCheckbox("Render BVH", m_draw_bvh, 90.0f, "#bvh");
        Editor::drawWidgetCheckbox("Render Mesh", m_disable_mesh_rendering, 90.0f, "#mesh");
        Editor::drawWidgetCheckbox("Render Wireframe", m_draw_wireframe, 90.0f, "#polygon_mode");
        Editor::drawWidgetCheckbox("Render 3D Grid", m_draw_grid_points, 90.0f, "#3d_grid");
        Editor::drawWidgetCheckbox("Render Mesh Grid Points", m_draw_mesh_grid_points, 90.0f, "#mesh_grid_points");
        Editor::drawWidgetCheckbox("Render Structural Springs", m_render_structural_springs, 90.0f, "#structural");
        Editor::drawWidgetCheckbox("Render Shear Springs", m_render_shear_springs, 90.0f, "#shear");
        Editor::drawWidgetCheckbox("Render Bend Springs", m_render_bend_springs, 90.0f, "#bend");
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

void PhysicsSandboxDemoLayer::generate3DGrid(std::vector<glm::vec3>& out_grid_points,
                                             const glm::vec3&        min,
                                             const glm::vec3&        max,
                                             const glm::vec3&        step)
{
    static bool one_ray_added = false;
    static int  i             = 0;
    for(float x = min.x; x <= max.x; x += step.x)
    {
        for(float y = min.y; y <= max.y; y += step.y)
        {
            for(float z = min.z; z <= max.z; z += step.z)
            {
                ++i;
                glm::vec3   random_direction = phx::phxGenerateRandomUnitVector();
                phx::PhxRay ray              = {};
                ray.origin                   = glm::vec3(x, y, z);
                // ray.direction              = glm::normalize(random_direction);
                ray.direction = glm::vec3(1.0f, 0.0f, 0.0f);

                // std::vector<vec4> ray_line;
                // ray_line.emplace_back(ray.origin.x, ray.origin.y, ray.origin.z, 1.0f);
                // ray_line.emplace_back(ray.origin.x + ray.direction.x * 2.0f,
                //                       ray.origin.y + ray.direction.y * 2.0f,
                //                       ray.origin.z + ray.direction.z * 2.0f,
                //                       1.0f);

                // m_render_system->drawDebugLines(ray_line, {1.0f, 0.0f, 0.0f}, mat4{}, 5.0f);

                // if(!one_ray_added && i == 5445)
                {
                    std::vector<phx::PhxRaycastResult> results;
                    if(phx::phxRaycast(ray, *m_triangle_mesh.get(), results, phx::PhxQueryMode::AllHits))
                    {
                        if(results.size() % 2 != 0) // Odd number of hits
                        {
                            // point is inside the mesh
                            m_point_positions.emplace_back(x, y, z);
                        }
                        APPLICATION_INFO("Raytrace result: size: {}", results.size());
                    }

                    out_grid_points.emplace_back(x, y, z);

                    m_mesh_intersection_rays.emplace_back(ray.origin.x, ray.origin.y, ray.origin.z, 1.0f);
                    m_mesh_intersection_rays.emplace_back(ray.origin.x + ray.direction.x * 5.0f,
                                                          ray.origin.y + ray.direction.y * 5.0f,
                                                          ray.origin.z + ray.direction.z * 5.0f,
                                                          1.0f);
                    one_ray_added = true;
                }
            }
        }
    }

    APPLICATION_INFO("Raytrace result: candidate points: {}", i);
    APPLICATION_INFO("Raytrace result: mesh points: {}", m_point_positions.size());

    // PhxInt num_x = static_cast<PhxInt>((max.x - min.x) / step.x);
    // PhxInt num_y = static_cast<PhxInt>((max.y - min.y) / step.y);
    // PhxInt num_z = static_cast<PhxInt>((max.z - min.z) / step.z);
    // APPLICATION_INFO("Raytrace result: calculated candidate points: {}", num_x * num_y * num_z);
}

} // namespace sputnik::demos