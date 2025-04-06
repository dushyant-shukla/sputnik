#pragma once

#include <transform.h>
#include <vector3.h>

namespace sputnik::graphics::core
{

using namespace ramanujan;

/*!
 * @brief FABRIK: Forward And Backward Reaching Inverse Kinematic.
 *
 * This is a popular inverse kinematic algorithm as it tends to converge on the target faster than CCD.
 * CCD IK tends to curl, while FABRIK tends to stretch, resulting in more natual results for humanoid animations.
 *
 */
class FabrikSolver
{

public:
    FabrikSolver();

    bool Solve(const Transform& target);

    unsigned int Size();
    void         Resize(unsigned int new_size);
    unsigned int GetNumSteps();
    void         SetNumSteps(unsigned int num_steps);
    float        GetThreshold();
    void         SetThreshold(float threshold);
    Transform    GetLocalTransform(unsigned int joint_index);
    void         SetLocalTransform(unsigned int joint_index, const Transform& transform);
    Transform    GetGlobalTransform(unsigned int joint_index);
    Transform&   operator[](unsigned int index);

    // Some debug methods
    void FetchPoints(std::vector<ramanujan::experimental::vec4>& points);
    void FetchLines(std::vector<ramanujan::experimental::vec4>& points);

protected:
    void LocalTransformsToGlobalPositions();

    /*!
     * @brief Sets the first joint to be at the base, and adjusts the rest of the IK chain to keep it intact.
     * This method might leave the end-effector at the target if the chain is solvable and has sufficient iterations.
     *
     * @param base Base's position in world space
     */
    void IterateForward(const Vector3& base);

    /*!
     * @brief Sets the end-effector to be the target, and adjusts the rest of the IK chain to keep it intact.
     * This method always leaves the end-effector at the target, and the initial joint is probably no longer at the
     * base.
     *
     * @param target Target's position in world space
     */
    void IterateBackward(const Vector3& target);

    /*!
     * @brief This method converts the global tranforms of joints in the IK chain into local transforms.
     */
    void GlobalPositionsToLocalTransforms();

protected:
    /*!
     * @brief The IK chain containing local transforms for joints.
     */
    std::vector<Transform> m_local_ik_chain;
    unsigned int           m_num_steps;
    float                  m_threshold;

    /*!
     * @brief Stores positions of joints (in world transforms) in the IK chain.
     */
    std::vector<Vector3> m_global_ik_chain;

    /*!
     * @brief Stores distances (in global transforms) of joints from their parents in the IK chain. This means that a
     * joint at base of the IK chain has a distance of 0, otherwise, the distance at index i is the distance between the
     * joints i and (i-1).
     */
    std::vector<float> m_distances;
};

} // namespace sputnik::graphics::core
