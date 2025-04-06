#pragma once

#include <transform.h>

namespace sputnik::graphics::core
{

using namespace ramanujan;

class CCDSolver
{

public:
    CCDSolver();

    unsigned int Size();
    void         Resize(unsigned int new_size);

    Transform& operator[](unsigned int index);
    Transform  GetGlobalTransform(unsigned int index);

    unsigned int GetNumSteps();
    void         GetNumSteps(unsigned int num_steps);

    double GetThreshold();
    void   SetThreshold(float value);

    bool Solve(const Transform& target);

    // Some debug methods
    void FetchPoints(std::vector<ramanujan::experimental::vec4>& points);
    void FetchLines(std::vector<ramanujan::experimental::vec4>& points);

protected:
    // Local transforms of the joint hierarchy that make up the IK chain.
    // Joint at every index is the child of the joint preceding it.
    std::vector<Transform> m_ik_chain;

    // Limiting the number of iterations to avoid potential infinite loop
    unsigned int m_num_steps;

    // A small delta that can be used to control how close the end-effector (last joint in the IK chain) has to be to
    // the target before thc chain is considered solved.
    double m_threshold;
};

} // namespace sputnik::graphics::core